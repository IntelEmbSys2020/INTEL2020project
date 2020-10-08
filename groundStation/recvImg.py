import socket               
import p2p
from dataTransform import bytes2int
#关于图像的数据传输用udp
#传入的参数包括序号num可以作为存储图像的名称
def recvImg(num, communicater):
    #注意！要在服务器的安全组中开放相应的进出口！
    MAX_LEN = 60000

    data,client_addr = communicater.P2P_recvData(10)#接收图像数据长度
    imgLength = bytes2int(data)#图像数据长度从bytes->int
    # print("image Length value is %d"%imgLength)

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
        data,client_addr = communicater.P2P_recvData(MAX_LEN)
        #写图文件
        imgLocalFile.write(data)
        #接收反馈
        communicater.P2P_sendData(packNow.to_bytes(length = 4,byteorder = 'little',signed = True),client_addr)
        packNow += 1

    #最后一次读取数据
    data,client_addr = communicater.P2P_recvData(remain)
    #最后一次写图文件
    imgLocalFile.write(data)

    #关闭文件
    imgLocalFile.close()
    return path
