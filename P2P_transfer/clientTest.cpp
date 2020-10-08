#include "p2p.hpp"

int main(void)
{
    struct p2p communicator;
    communicator.APP_ID = 2020;
    communicator.localType = TERMINAL;
    strcpy(communicator.IPv4_server_TCP,"81.68.85.122");
    strcpy(communicator.IPv4_server_UDP,"119.28.6.20");
    communicator.port_server_TCP = 8000;
    communicator.port_server_UDP = 8001;
    
    P2P_Init(&communicator);

    char msg[] = "INTEL2020";
    std::cout<<"按一次按键对地面站发送一次数据"<<std::endl;
    while (true)
    {
        getchar();
        P2P_sendData(&communicator,msg,sizeof(msg));
        std::cout<<"发送一次消息"<<std::endl;
    }
    

}