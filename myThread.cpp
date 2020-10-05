#include "myThread.hpp"
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
        if(P2P_sendData(my_data->p2p_target,my_data->data,my_data->dataLength) == false)
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
    // struct thread_data *my_data;
    
    // my_data = (struct thread_data *) threadarg;
    

    //enter the forever loop
    while(1){
        P2P_recvData(p2p *target,void * data,size_t dataLength);
    }
    
    pthread_exit(NULL);
}
