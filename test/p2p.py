'''
由于时间限制，该文件目前仅支持地面站服务（锥形NAT）
Author：江榕煜
e-mail：2373180028@qq.com
'''

import socket


class P2P:
    """
    内部对象
    """
    __TCPcommand = {
        "P2P_begin" : 1,
        "P2P_OK" : 2,
        "P2P_ERROR" : 3
    }

    def __init__(self,AppID = 2020,server_IPv4 = "server.singularity-blog.top",server_TCPport = 8000,server_UDPport = 8001,TCP_port = 8000,UDP_port = 8001):
        #存储基本属性
        self.AppID = AppID
        self.server_IPv4 = server_IPv4
        self.server_UDPport = server_UDPport
        self.server_TCPport = server_TCPport
        self.local_UDPport = UDP_port
        self.local_TCPport = TCP_port

        # print("AppID is:"+str(self.AppID)+",IP of server is :"+self.server_IPv4)
        print("AppID is:%d,IP of server is :%s"%(self.AppID,self.server_IPv4))

        #创建套接字
        self.socket_UDP = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        self.socket_TCP = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        #设置本地套接字端口
        self.socket_UDP.bind((str(socket.INADDR_ANY),UDP_port))
        self.socket_TCP.bind((str(socket.INADDR_ANY),TCP_port))
        print("本地UDP服务启动，端口号：%d"%UDP_port)
        print("本地TCP服务启动，端口号：%d"%TCP_port)
        #连接服务器TCP
        print("开始和服务器握手")
        self.socket_TCP.connect((server_IPv4,server_TCPport))
        print("服务器握手成功（TCP）")
        #等待穿透指示（TCP）
        print("等待服务器发送穿透开始指令")
        recvData = self.socket_TCP.recv(4)
        if(recvData != self.__TCPcommand["P2P_begin"]):
            print("SERVER CMD ERROR! when 穿透指示")
            return False
        #-----------开始穿透-------------
        print("开始内网穿透...")
        #发送（注册）口令（UDP）
        self.socket_UDP.sendto(     AppID.to_bytes(length = 4,
                                                                            byteorder = 'little',
                                                                            signed = True),
                                                                            (server_IPv4,server_UDPport))
        print("口令注册")
        #等待接收作业端地址（TCP）
        recvData = self.socket_TCP.recv(16)
        self.terminal_IPv4 = recvData.decode('utf-8')
        print("从服务器接收到作业端公网IP：%s"%self.terminal_IPv4)
        #接收作业端UDP端口（TCP）
        recvData = self.socket_TCP.recv(4)
        self.terminal_UDPport = int.from_bytes(recvData,byteorder='little',signed=True)
        print("从服务器接收到作业端UDP通信端口：%d"%self.terminal_UDPport)
        #本地穿透
        self.socket_UDP.sendto(      AppID.to_bytes(length = 4,
                                                                            byteorder = 'little',
                                                                            signed = True),
                                                                            (self.terminal_IPv4,self.terminal_UDPport))
        #接收作业端的握手消息
        recvData,recvAddr = self.socket_UDP.recvfrom(4)
        recvData = int.from_bytes(recvData,byteorder='little',signed=True)
        print("接收作业端UDP握手消息：%d"%recvData)
        if(recvData != self.AppID):
            print("作业端传来的ID ERROR!")
            return False
        print("穿透成功")
        #反馈成功信息给服务器
        self.socket_TCP.send(self.__TCPcommand["P2P_OK"].to_bytes(
                                                                                                                                length = 4,
                                                                                                                                byteorder = 'little',
                                                                                                                                signed = True))
        return True
    
    def P2P_sendData(self,data):
        """
        调用该函数发送数据，注意：底层使用UDP
        """
        self.socket_UDP.sendto( data,(self.terminal_IPv4,self.terminal_UDPport))
        return

    def P2P_sendCMD(self,data):
        """
        调用该函数发送重要数据or命令，注意：底层使用TCP
        """
        self.socket_TCP.send(data)
        return

    def P2P_recvData(self,dataSize):
        """
        调用该函数接收数据，注意：底层使用UDP
        """
        return self.socket_UDP.recvfrom(dataSize)

    def P2P_recvCMD(self,dataSize):
        """
        调用该函数接收重要数据or命令
        可以选择等待数据（阻塞）或试探性读取（非阻塞）
        """
        return self.socket_TCP.recv(dataSize)
        
