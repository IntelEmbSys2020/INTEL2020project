#include "myThread.hpp"
#include "Demo/samples/xtts_offline_sample/xtts_offline_sample.h"
#include "../P2P_transfer/p2p.hpp" 
#include<ctime>
using namespace std;
#define MAX_LEN 60000
#define IMG_LEN 3932214

/*	@brief:	        capture and send the img to the client. sleep 1s and do it once.
 *	@notice:        thread1
 *	@author:        cyl
 *	@param:         cameraObeject：相机对象
 　　　　　　　　　　　pixBuf：图像源数据缓冲池对象
 *	@retv:	        null
**/

//thread的参数很特殊
void* captureImg(void *threadarg)
{
    clock_t start = 0;
    clock_t end = 0;

    //transform the arg form
    struct thread_data *my_data; 
    my_data = (struct thread_data *) threadarg;
    
    //enter an forever loop
    while(1){
        //wait a second
        sleep(1);
        start=clock();
        //get one frame per second
        //cameraObeject * target,imgData * buffer
        if(camera_pixBufferInit(my_data->cam_target,my_data->buffer) == false)
        {
            cout << "get one frame failed." << endl;
        }
        
        my_data->data = my_data->buffer->imageData;
        my_data->dataLength = my_data->buffer->imageLength;

        //send the frame to the client
        //p2p *target,void * data,size_t dataLength
        sendImg(my_data);
        
        //compute the time
        end=clock();
        double endtime=(double)(end-start)/CLOCKS_PER_SEC;
	    cout<<"program time:"<<endtime<<endl;
    } 
    
    pthread_exit(NULL);
}


/*	@brief:	        receive the data sending from the client, call down at the crowds.
 *	@notice:        thread2
 *	@author:        cyl
 *	@param:         null
 *	@retv:	        null
**/
void *tts(void *threadarg)
{
    struct thread_data *my_data; 
    my_data = (struct thread_data *) threadarg;
    //把传入的参数赋值
    p2p *target = my_data->p2p_target;
    void *data = my_data->data;
    size_t dataLength = my_data->dataLength;

    int age;
    char* gender;
    char* glass;
    //enter the forever loop
    while(1){
        //receive the data
        //receive age
        if(P2P_recvCMD(target, &age, sizeof(int))==sizeof(int))
        {
            printf("the age data hasn't been received.\n");
        }
        //receive gender
        if(P2P_recvCMD(target, &gender, sizeof(char*))==sizeof(char*))
        {
            printf("the gender data hasn't been received.\n");
        }
        //receive glass
        if(P2P_recvCMD(target, &glass, sizeof(char*))==sizeof(char*))
        {
            printf("the glasses data hasn't been received.\n");
        }

        //give out the voice
        gen_voice(age, gender, glass);
    }
    
    pthread_exit(NULL);
}

void sendImg(struct thread_data *my_data)
{
    char* currPtr = (char*)my_data->data;
    int count = 0;
    int total = IMG_LEN/MAX_LEN;
    int remain = IMG_LEN%MAX_LEN;
    cout<<"total: "<<total<<endl;
    cout<<"remain: "<<remain<<endl;

    for(count = 0; count < total; count++)
    {
        //1.p2p*  2.void*  3.unsigned long
        if(P2P_sendData(my_data->p2p_target,currPtr, MAX_LEN) == false)
        {
            cout << "send part of one frame failed." << endl;
        }
        currPtr += MAX_LEN;
    }

    if(P2P_sendData(my_data->p2p_target,currPtr, remain) == false)
    {
        cout << "send part of one frame failed." << endl;
    }
    
}