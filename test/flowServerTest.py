import socket
s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
s.bind(('0',8002))
print("打洞")
s.sendto(bytes(1),('flow.singularity-blog.top',8000))
data,recvAddr = s.recvfrom(60000)
if(len(data)):
    print("收到数据，长度：%d",len(data))
else:
    print("接收失败")