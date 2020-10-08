import socket
import time

class P2P_Station:
    def __init__(self,
                AppID = 2020,
                IPv4_server_TCP = "server.singularity-blog.top",
                IPv4_server_UDP = "flow.singularity-blog.top",
                port_server_TCP = 8000,
                port_server_UDP = 8001,
                local_TCP_port = 8000,
                local_UDP_port = 8001):
        #存储基本属性
        self.AppID = AppID
        self.IPv4_server_TCP = IPv4_server_TCP
        self.IPv4_server_UDP = IPv4_server_UDP
        self.port_server_TCP = port_server_TCP
        self.port_server_UDP = port_server_UDP
        self.local_TCP_port = local_TCP_port
        self.local_UDP_port = local_UDP_port

        print("AppID is:%d"%(self.AppID))
        print("TCP_server IP:%s,port:%d"%(self.IPv4_server_TCP,self.port_server_TCP))
        print("UDP_server_IP:%s,port:%d"%(self.IPv4_server_UDP,self.port_server_UDP))

        #创建套接字
        self.socket_UDP = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        self.socket_TCP = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        #设置本地套接字端口
        self.socket_UDP.bind((str(socket.INADDR_ANY),local_UDP_port))
        self.socket_TCP.bind((str(socket.INADDR_ANY),local_TCP_port))
        print("本地UDP服务启动，端口号：%d"%local_UDP_port)
        print("本地TCP服务启动，端口号：%d"%local_TCP_port)
        #连接服务器TCP
        print("开始和服务器握手")
        self.socket_TCP.connect((IPv4_server_TCP,port_server_TCP))
        print("服务器握手成功（TCP）")
        #连接UDP服务器
        print("开始向UDP服务器做内网穿透")
        i = 0
        while i<5:
            i += 1
            print("开始第"+str(i)+"次穿透")
            self.socket_UDP.sendto((AppID +i).to_bytes(length = 4,
                                                byteorder = 'little',
                                                signed = True),
                                    (self.IPv4_server_UDP,self.port_server_UDP))
            print("等待服务器响应穿透")
            recvData,recvAddr = self.socket_UDP.recvfrom(4)
            recvAppID = int.from_bytes(recvData,byteorder='little',signed=True)
            print("收到穿透响应，ID：%d",recvAppID)
            time.sleep(1)
        #穿透结束
        return

    def P2P_sendData(self,data):
        self.socket_UDP.sendto( data,(self.IPv4_server_UDP,self.port_server_UDP))
        return
    
    def P2P_sendCMD(self,cmd):
        self.socket_TCP.send(cmd)
        return

    def P2P_recvData(self,prefSize):
        self.P2P_sendData(bytes(1))
        return self.socket_UDP.recvfrom(prefSize)

    def P2P_recvCMD(self,prefSize):
        return self.socket_TCP.recv(prefSize)