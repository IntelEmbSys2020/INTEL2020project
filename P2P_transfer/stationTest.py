import p2p_station

communicator = p2p_station.P2P_Station()

while True:
    recvData = communicator.P2P_recvData(10)
    if(len(data) != 0):
        print(  "recvData is :%s"%recvData.decode('utf-8') ) 

