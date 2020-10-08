import socket

class p2p_UDPserver:
    def __init__(self,
                    AppID = 2020,
                    local_port = 8001):
        self.local_port = local_port
        self.AppID = AppID
        print("在端口%d上建立UDP服务"%local_port)
        self.socket_UDP = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        self.socket_UDP.bind((str(socket.INADDR_ANY),local_port))
        #先等待地面连接
        print("等待地面站连接")
        recvData,recvAddr = self.socket_UDP.recvfrom(4)
        self.socket_UDP.sendto(self.AppID.to_bytes(length = 4,
                                                    byteorder = 'little',
                                                    signed = True)
                                ,recvAddr)
        recvAppID = int.from_bytes(recvData,byteorder='little',signed=True)
        print("接收到地面站ID：%d"%recvAppID)
        #先等待作业端连接
        i = 0
        while i<5:
            i += 1
            print("等待作业端第"+str(i)+"次连接")
            recvData,recvAddr = self.socket_UDP.recvfrom(4)
            self.socket_UDP.sendto(self.AppID.to_bytes(length = 4,
                                                        byteorder = 'little',
                                                        signed = True)
                                    ,recvAddr)
            recvAppID = int.from_bytes(recvData,byteorder='little',signed=True)
            print("接收到地面站ID：%d"%recvAppID)
        #穿透完成
        return
    
