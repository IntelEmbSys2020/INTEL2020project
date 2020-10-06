#include "p2p.hpp"

int main(int argc, char const *argv[])
{
    std::cout<<"服务器启动！"<<std::endl;
    p2p communicater;
    communicater.APP_ID = 2020;
    communicater.localType = SERVER;
    communicater.port_server_TCP = 8000;
    communicater.port_server_UDP = 8001;
    P2P_Init(&communicater);

    std::cout<<"服务器掉线！"<<std::endl;
    return 0;
}
