#ifndef _myThread_
#define _myThread_
#include "worker/cameraGetImage/cameraGetImage.h"
#include "P2P/p2p.hpp" 

struct thread_data{
    //first two are used by func: camera_pixBufferInit
    imgData * buffer;
    cameraObeject * cam_target;
    //last three are used by func: P2P_sendData
    p2p *p2p_target;
    void * data;
    size_t dataLength;
};

/*	@brief:	        capture and send the img to the client. sleep 1s and do it once.
 *	@notice:        thread1
 *	@author:        cyl
 *	@param:         cameraObeject：相机对象
 　　　　　　　　　　　pixBuf：图像源数据缓冲池对象
 *	@retv:	        null
**/
void* captureImg(void *threadarg);

/*	@brief:	        receive the data sending from the client, call down at the crowds.
 *	@notice:        thread2
 *	@author:        cyl
 *	@param:         null
 *	@retv:	        null
**/
void *tts(void *threadarg);

#endif // !myThread