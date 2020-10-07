'''
func:
循环：
    1. 接收传来的图片
    ２．处理传来的图片
    ３．发送回去处理得到的信息
Author:cyl
'''
import socket
from ../test/p2p import *
import ../faceDetect/tensorflow_infer
import sys
import cv2
import dataTransform

print("地面站开始工作")
'''create p2p'''
communicater  = P2P()
'''enter loop'''
while(1)
{
    '''receive img'''
    communicater.P2P_recvData(dataSize)

    '''process img'''
    age,gender,glasses = tensorflow_infer.run_on_img(img, conf_thresh=0.7)
    #the return are int, str, bool
    #turn the info towards bytes
    bGender = str2bytes(gender)
    bAge = int2bytes(age)
    cGlasses = bool2charStar(glasses)
    bGlasses = str2bytes(cGlasses)

    '''send info
       worker recv it in order age, gender, glasses
    '''
    communicater.P2P_sendData(bAge)
    communicater.P2P_sendData(bGender)
    communicater.P2P_sendData(bGlasses)    
}

print("地面站结束")