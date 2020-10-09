import socket               
# import p2p
from dataTransform import bytes2int
import sys
sys.path.append('../')
from P2P_transfer.p2p_station import *

#关于图像的数据传输用udp
#传入的参数包括序号num可以作为存储图像的名称
def recvImg(num, communicater):
    #注意！要在服务器的安全组中开放相应的进出口！
    MAX_LEN = 60000
    imgLength = 3932214

    #创建本地图像文件
    path='../Img/img'+num+'.bmp'
    imgLocalFile = open(path,mode = 'wb')

    #求解需要接收的图像数据包的个数
    packNum = imgLength / MAX_LEN
    #求解尾包数据个数
    remain = imgLength % MAX_LEN

    packNow = 0
    while packNow < packNum:
        #接收数据
        # communicater.P2P_recvData(packNow.to_bytes(length = 4,byteorder = 'little',signed = True),client_addr)
        data = communicater.P2P_recvCMD(MAX_LEN)
        #写图文件
        imgLocalFile.write(data)
        #包数目加１
        packNow += 1

    #最后一次接收数据
    # data,client_addr = communicater.P2P_recvData(remain)
    data = communicater.P2P_recvCMD(remain)
    #最后一次写图文件
    imgLocalFile.write(data)

    #关闭文件
    imgLocalFile.close()
    return path
