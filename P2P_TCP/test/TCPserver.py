import sys
sys.path.append('../')
import p2p_TCPserver

communicator = p2p_TCPserver.p2p_TCPserver()
print("完成穿透")
print("开始数据服务")
communicator.p2p_Transfer()