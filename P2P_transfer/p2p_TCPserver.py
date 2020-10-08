import socket

class p2p_TCPserver:
    __TCP_MAX_PACKAGE_LENGTH = 1000
    def __init__(self,
                    AppID = 2020,
                    local_port = 8000):
        '''
        p2p对象构造函数
        '''
        self.local_port = local_port
        self.AppID = AppID
        print("在端口%d上建立TCP服务"%local_port)
        self.socket_TCP = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.socket_TCP.bind((str(socket.INADDR_ANY),local_port))
        #建立TCP监听
        self.socket_TCP.listen(4)
        #先等待地面连接
        print("等待地面站连接")
        self.socket_STATION,self.stationAddr = self.socket_TCP.accept()
        #后等待作业端连接
        print("等待作业端连接")
        self.socket_WORKER,self.stationAddr = self.socket_TCP.accept()
        #穿透完成
        return
    
    def __del__(self):
        '''
        对象析构函数
        '''
        self.socket_STATION.close()
        self.socket_WORKER.close()
        self.socket_TCP.close()

    def p2p_Transfer(self):
        '''
        无脑转播
        方向：地面站->该服务器->作业端
        '''
        data = self.socket_STATION.recv(__TCP_MAX_PACKAGE_LENGTH)   #收到地面消息
        self.socket_WORKER.send(data)   #转发给作业端
    
