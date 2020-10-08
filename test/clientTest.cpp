#include "p2p.hpp"

int main(int argc, char const *argv[])
{
    std::cout<<"作业端启动！"<<std::endl;
    p2p communicater;
    communicater.APP_ID = 2020;
    communicater.localType = TERMINAL;
    strcpy(communicater.IPv4_server, "81.68.85.122");
    communicater.port_terminal_TCP = 8000;
    std::cout<<"本地TCP端口："<<communicater.port_terminal_TCP<<std::endl;
    communicater.port_terminal_UDP = 8001;
    std::cout<<"本地UDP端口："<<communicater.port_terminal_UDP<<std::endl;
    communicater.port_server_TCP = 8000;
    std::cout<<"云服务器TCP端口："<<communicater.port_server_TCP<<std::endl;
    communicater.port_server_UDP = 8001;
    std::cout<<"云服务器UDP端口："<<communicater.port_server_UDP<<std::endl;
    P2P_Init(&communicater);

    std::cout<<"作业端结束！"<<std::endl;
    return 0;
}
