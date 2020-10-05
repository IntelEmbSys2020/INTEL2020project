// #include "worker/cameraGetImage/cameraGetImage.h"
// #include "P2P/p2p.hpp" 
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include "myThread.hpp"
using namespace std;

#define NUM_THREADS 2

int main()
{
    bool initOK = true;
    bool cameraOK = true;
    p2p p2pObj;
    cameraObeject camObj;
    imgData img;

    //initialize a p2p Object    
    if(P2P_Init(&p2pObj) == false)
    {
        return 1;//初始化错误，直接返回
    }

    //initialize the camera
    if(camera_deviceInit(&camObj) == false)
    {
        return 1;//错误并返回
    }
    if(camera_beginCapture(&camObj) == false)
    {
        return 1;//错误并返回
    }
    if(camera_pixBufferInit(&camObj, &img) == false)
    {
        return 1;//错误并返回
    }
    

    /*enter the threads:
    * 1. capture and send the img to the client. sleep 1s and do it once.
    * 2. receive the data sending from the client, call down at the crowds. 
    **/
    pthread_t threads[NUM_THREADS];
    struct thread_data td1;//for thread1
    struct thread_data td2;//for thread2
    int rc1,rc2;

    
    /*enter the 1st thread
    *capture and send the img to the client.*/\

    //为传入线程参数做准备工作
    td1.buffer = &img;
    td1.cam_target = &camObj;
    td1.p2p_target = &p2pObj;
    td1.data = nullptr;
    td1.dataLength = 0;
    //参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
    rc1 = pthread_create(&threads[0], NULL,
                        captureImg, (void *)&td1);
    //判断线程创建是否成功
    if (rc1){
        cout << "Error:unable to create thread," << rc1 << endl;
        exit(-1);
    }
    

    /*enter the 2nd thread
    *capture and send the img to the client.*/
    td2.buffer = nullptr;
    td2.cam_target = nullptr;
    td2.p2p_target = &p2pObj;
    td2.data = nullptr;
    td2.dataLength = 0;

    //参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
    rc2 = pthread_create(&threads[1], NULL,
                    tts, (void *)&td2);
    if (rc2){
        cout << "Error:unable to create thread," << rc2 << endl;
        exit(-1);
    }

    pthread_exit(NULL);
    
}
