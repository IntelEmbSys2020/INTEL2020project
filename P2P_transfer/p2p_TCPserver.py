import socket

class p2p_TCPserver:
    def __init__(self,
                    AppID = 2020,
                    local_port = 8000)
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
    
