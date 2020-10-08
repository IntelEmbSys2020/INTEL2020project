#include "myThread.hpp"
#include "Demo/samples/xtts_offline_sample/xtts_offline_sample.h"
#include "../P2P/p2p.hpp" 
using namespace std;
/*	@brief:	        capture and send the img to the client. sleep 1s and do it once.
 *	@notice:        thread1
 *	@author:        cyl
 *	@param:         cameraObeject：相机对象
 　　　　　　　　　　　pixBuf：图像源数据缓冲池对象
 *	@retv:	        null
**/

void* captureImg(void *threadarg)
{
    //transform the arg form
    struct thread_data *my_data; 
    my_data = (struct thread_data *) threadarg;
    
    //enter an forever loop
    while(1){
        //wait a second
        sleep(1);

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
        unsigned long dataLength = (unsigned long)my_data->dataLength;
        if(P2P_sendData(my_data->p2p_target,my_data->data, dataLength) == false)
        {
            cout << "send one frame failed." << endl;
        }
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
        if(P2P_recvData(target, &age, sizeof(int))==sizeof(int))
        {
            printf("the age data hasn't been received.\n");
        }
        //receive gender
        if(P2P_recvData(target, &gender, sizeof(char*))==sizeof(char*))
        {
            printf("the gender data hasn't been received.\n");
        }
        //receive glass
        if(P2P_recvData(target, &glass, sizeof(char*))==sizeof(char*))
        {
            printf("the glasses data hasn't been received.\n");
        }

        //give out the voice
        gen_voice(age, gender, glass);
    }
    
    pthread_exit(NULL);
}
