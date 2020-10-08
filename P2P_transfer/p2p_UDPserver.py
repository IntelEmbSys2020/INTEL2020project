import socket

class p2p_UDPserver:
    __MAX_UDP_PACKEGE_LENGTH = 60000
    def __init__(self,
                    AppID = 2020,
                    local_port = 8001):
        '''
        p2p对象构造函数
        '''
        self.local_port = local_port
        self.AppID = AppID
        print("在端口%d上建立UDP服务"%local_port)
        self.socket_UDP = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        self.socket_UDP.bind((str(socket.INADDR_ANY),local_port))
        #先等待地面连接
        print("等待地面站连接")
        i = 0
        
        while i<5:
            i += 1
            print("等待地面站第"+str(i)+"次连接")
            recvData,recvAddr = self.socket_UDP.recvfrom(4)
            print("地面站IP：%s，端口：%d"%recvAddr)
            self.socket_UDP.sendto(self.AppID.to_bytes(length = 4,
                                                        byteorder = 'little',
                                                        signed = True)
                                    ,recvAddr)
            recvAppID = int.from_bytes(recvData,byteorder='little',signed=True)
            print("接收到地面站ID：%d"%recvAppID)
        
        self.addrGroundStation = recvAddr
        print("地面站IP：%s，端口：%d"%recvAddr)
        #先等待作业端连接
        print("等待作业端连接")
        i = 0
        
        while i<5:
            i += 1
            print("等待作业端第"+str(i)+"次连接")
            recvData,recvAddr = self.socket_UDP.recvfrom(4)
            print("作业端连接IP：%s，port：%d"%recvAddr)
            self.socket_UDP.sendto(self.AppID.to_bytes(length = 4,
                                                        byteorder = 'little',
                                                        signed = True)
                                    ,recvAddr)
            recvAppID = int.from_bytes(recvData,byteorder='little',signed=True)
            print("接收到作业端ID：%d"%recvAppID)
       
        self.addrWorker = recvAddr
        print("作业端IP：%s，端口：%d"%recvAddr)
        #穿透完成
        return
    
    def __del__(self):
        '''
        析构函数
        '''
        self.socket_UDP.close()
    
    def p2p_Transfer(self):
        '''
        无脑死循环转播（有空再写可控请求传输吧）
        方向：作业端->该服务器->地面站
        '''
        while True:
            data,recvAddr = self.socket_UDP.recvfrom(self.__MAX_UDP_PACKEGE_LENGTH) #监听数据
            print("接受到数据")
            if(recvAddr != self.addrWorker):    #如果收到的数据不是作业端的
                print("不是作业端数据")
                continue        #忽略该信息包
            print("发送数据给地面站")
            self.socket_UDP.sendto(data,self.addrGroundStation) #发给地面站
