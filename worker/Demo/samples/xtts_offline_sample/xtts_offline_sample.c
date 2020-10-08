/*
* 语音合成（Text To Speech，TTS）技术能够自动将任意文字实时转换为连续的
* 自然语音，是一种能够在任何时间、任何地点，向任何人提供语音信息服务的
* 高效便捷手段，非常符合信息时代海量数据、动态更新和个性化查询的需求。
*/

#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../../include/qtts.h"
#include "../../include/msp_cmn.h"
#include "../../include/msp_errors.h"
#include "../../include/msp_types.h"
// #include <linux/soundcard.h>
#include "xtts_offline_sample.h"

#define Audio_Device "/dev/dsp"
#define Sample_Size 16 //there're two kinds of bits,8 bits and 16 bits
#define Sample_Rate 8000 //sampling rate
typedef int SR_DWORD;
typedef short int SR_WORD ;

wave_pcm_hdr default_wav_hdr = 
{
	{ 'R', 'I', 'F', 'F' },
	0,
	{'W', 'A', 'V', 'E'},
	{'f', 'm', 't', ' '},
	16,
	1,
	1,
	16000,
	32000,
	2,
	16,
	{'d', 'a', 't', 'a'},
	0  
};

/* 文本合成 */
int text_to_speech(const char* src_text, const char* des_path, const char* params)
{
	int          ret          = -1;
	FILE*        fp           = NULL;
	const char*  sessionID    = NULL;
	unsigned int audio_len    = 0;
	wave_pcm_hdr wav_hdr      = default_wav_hdr;
	int          synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;

	if (NULL == src_text || NULL == des_path)
	{
		printf("params is error!\n");
		return ret;
	}
	fp = fopen(des_path, "wb");
	if (NULL == fp)
	{
		printf("open %s error.\n", des_path);
		return ret;
	}
	/* 开始合成 */
	sessionID = QTTSSessionBegin(params, &ret);
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSSessionBegin failed, error code: %d.\n", ret);
		fclose(fp);
		return ret;
	}
	ret = QTTSTextPut(sessionID, src_text, (unsigned int)strlen(src_text), NULL);
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSTextPut failed, error code: %d.\n",ret);
		QTTSSessionEnd(sessionID, "TextPutError");
		fclose(fp);
		return ret;
	}
	printf("正在合成 ...\n");
	fwrite(&wav_hdr, sizeof(wav_hdr) ,1, fp); //添加wav音频头，使用采样率为16000
	while (1) 
	{
		/* 获取合成音频 */
		const void* data = QTTSAudioGet(sessionID, &audio_len, &synth_status, &ret);
		if (MSP_SUCCESS != ret)
			break;
		if (NULL != data)
		{
			fwrite(data, audio_len, 1, fp);
		    wav_hdr.data_size += audio_len; //计算data_size大小
		}
		if (MSP_TTS_FLAG_DATA_END == synth_status)
			break;
	}
	printf("\n");
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSAudioGet failed, error code: %d.\n",ret);
		QTTSSessionEnd(sessionID, "AudioGetError");
		fclose(fp);
		return ret;
	}
	/* 修正wav文件头数据的大小 */
	wav_hdr.size_8 += wav_hdr.data_size + (sizeof(wav_hdr) - 8);
	
	/* 将修正过的数据写回文件头部,音频文件为wav格式 */
	fseek(fp, 4, 0);
	fwrite(&wav_hdr.size_8,sizeof(wav_hdr.size_8), 1, fp); //写入size_8的值
	fseek(fp, 40, 0); //将文件指针偏移到存储data_size值的位置
	fwrite(&wav_hdr.data_size,sizeof(wav_hdr.data_size), 1, fp); //写入data_size的值
	fclose(fp);
	fp = NULL;
	/* 合成完毕 */
	ret = QTTSSessionEnd(sessionID, "Normal");
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSSessionEnd failed, error code: %d.\n",ret);
	}

	return ret;
}

/*：
* argv[1]->年龄:一个浮点数的字符串形式 
* argv[2]->性别:"male"or"female"
* argv[3]->眼镜:"True"or"False"
*/

// int main(int argc, char* argv[])
int gen_voice(int age_int, char* gender, char* glass)
{
	int         ret                  = MSP_SUCCESS;
	const char* login_params         = "appid = 5f27877b, work_dir = .";//登录参数,appid与msc库绑定,请勿随意改动
	/*
	* rdn:           合成音频数字发音方式
	* volume:        合成音频的音量
	* pitch:         合成音频的音调
	* speed:         合成音频对应的语速
	* voice_name:    合成发音人
	* sample_rate:   合成音频采样率
	* text_encoding: 合成文本编码格式
	*
	*/
    // float age = atof(argv[1]);//年龄字符串转为浮点数
	// // float age = argv[1];
	// char* gender = argv[2];
	// char* glass = argv[3];
	// int age_int = (int)age;//取整
	
	const char* session_begin_params = "engine_type = purextts,voice_name=xiaoyan, text_encoding = UTF8, tts_res_path = fo|res/xtts/xiaoyan.jet;fo|res/xtts/common.jet, sample_rate = 16000, speed = 50, volume = 100, pitch = 50, rdn = 0";
	const char* filename             = "reminder.wav"; //合成的语音文件名称
	//const char* text                 = "亲爱的用户，您好，这是一个语音合成示例，感谢您对科大讯飞语音技术的支持！科大讯飞是亚太地区最大的语音上市公司，股票代码：002230"; //合成文本
	
	char text1[] = "防疫千万条，口罩第一条。请这位";
	if (age_int <= 10)
	{
		//char text1[] = "请这位";
		char text2[3];
		sprintf(text2,"%d", age_int);
        strcat(text1, text2);//拼接text1,2

		char text3[] = "岁左右";
	    strcat(text1, text3);//拼接text1,3

		char text4[] = "戴眼镜";
	    if (strcmp("True", glass) == 0)
	    {
		    strcat(text1, text4);//拼接text1,4 
	    }

		char text5[] = "的小";
	    strcat(text1, text5);//拼接text1,5

        char text6[8];
		if (strcmp("male", gender) == 0)
		{
			strcpy(text6, "帅哥");
			//char text6[] = "男";
		}
		else
		{
			strcpy(text6, "美女");
			//char text6[] = "女";
		}
		strcat(text1, text6);//拼接text1,6
		
		char text7[] = "戴好口罩，做自己的健康小卫士";
		strcat(text1, text7);//拼接text1,7
	}
	else
	{
		char text2[3];//年龄 
		//itoa(age_int, text2, 10);//年龄整数转为字符串存入text2
		sprintf(text2,"%d", age_int);
		strcat(text1, text2);//拼接text1,2

		char text3[] = "岁左右";
		strcat(text1, text3);//拼接text1,3

		char text4[] = "戴眼镜";
		if (strcmp("True", glass) == 0)
		{
			strcat(text1, text4);//拼接text1,4
	
		}

		char text5[] = "的";
		strcat(text1, text5);//拼接text1,5

		char text6[5];
		if (strcmp("male", gender) == 0)
		{
			strcpy(text6, "男");
			//char text6[] = "男";
		}
		else
		{
			strcpy(text6, "女");
			//char text6[] = "女";
		}
		strcat(text1, text6);//拼接text1,6
		
		char text7[] = "士戴上口罩";
		strcat(text1, text7);//拼接text1,7
	}

	/* 用户登录 */
	ret = MSPLogin(NULL, NULL, login_params); //第一个参数是用户名，第二个参数是密码，第三个参数是登录参数，用户名和密码可在http://www.xfyun.cn注册获取
	if (MSP_SUCCESS != ret)
	{
		printf("MSPLogin failed, error code: %d.\n", ret);
		goto exit ;//登录失败，退出登录
	}
/*
	printf("\n###########################################################################\n");
	printf("## 语音合成（Text To Speech，TTS）技术能够自动将任意文字实时转换为连续的 ##\n");
	printf("## 自然语音，是一种能够在任何时间、任何地点，向任何人提供语音信息服务的  ##\n");
	printf("## 高效便捷手段，非常符合信息时代海量数据、动态更新和个性化查询的需求。  ##\n");
	printf("###########################################################################\n\n");
*/
	/* 文本合成 */
	printf("开始合成 ...\n");
	ret = text_to_speech(text1, filename, session_begin_params);
	if (MSP_SUCCESS != ret)
	{
		printf("text_to_speech failed, error code: %d.\n", ret);
	}
	printf("合成完毕\n");
	system("play reminder.wav");

exit:
	//printf("按任意键退出 ...\n");
	//getchar();
	MSPLogout(); //退出登录

	return 0;
}

