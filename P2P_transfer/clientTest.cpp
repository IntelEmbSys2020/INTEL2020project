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
}