import p2p_UDPserver

communicator = p2p_UDPserver.p2p_UDPserver()
print("完成穿透")
print("开始数据服务")
communicator.p2p_Transfer()