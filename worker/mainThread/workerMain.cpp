#include <iostream>


#include "cameraGetImage/cameraGetImage.h"

using namespace std;

int main()
{
    imgData imgBuffer;
    cameraObeject camera;

    /********相机初始化**********/
    if( !camera_deviceInit(&camera) )
    {
        std::cout<<"Camera Init Fail !"<<std::endl;
        return -1;
    }

    /*********启动相机流*********/
    if(!camera_beginCapture(&camera))
    {
        std::cout<<"Camera begin Fail !"<<std::endl;
        return -1;
    }

    /*********初始化图像缓冲区*********/
    if(!camera_pixBufferInit(&camera,&imgBuffer))
    {
        std::cout<<"buffer Init Fail !"<<std::endl;
        return -1;
    }

    /*********获取一帧*********/
    if(!camera_getImage(&camera,&imgBuffer))
    {
        std::cout<<"get Image Fail !"<<std::endl;
        return -1;
    }

    /*********存储数据*********/
    FILE * fp = fopen("getImageSouce.bmp","wb");
    if (NULL == fp)
    {
        std::cout<<"fopen failed!"<<std::endl;
        return -1;
    }
    cout<<" the length are: "<<imgBuffer.imageLength<<endl;
    fwrite(imgBuffer.imageData, 1,imgBuffer.imageLength, fp);
    fclose(fp);

    /**********停止相机取流************/
    if(!camera_endCapture(&camera))
    {
        std::cout<<"endCapture Error !"<<std::endl;
        return -1;
    }

    /**********关闭相机************/
    if(!camera_deviceDestroy(&camera))
    {
        std::cout<<"endCapture Error !"<<std::endl;
        return -1;
    }

    /**********关闭相机************/
    if(!camera_pixBufferFree(&imgBuffer))
    {
        std::cout<<"endCapture Error !"<<std::endl;
        return -1;
    }

    std::cout<<"work finish!"<<std::endl;

    return 0;
}
