import sys
sys.path.append('../')
import p2p_Station

communicator = p2p_Station.P2P_Station()

while True:
    recvData = communicator.P2P_recvCMD(10)
    if(len(data) != 0):
        print(  "recvData is :%s"%recvData.decode('utf-8') ) 

