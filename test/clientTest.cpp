#include "p2p.hpp"

int main(int argc, char const *argv[])
{
    p2p communicater;
    communicater.APP_ID = 2020;
    communicater.localType = TERMINAL;
    communicater.port_terminal_TCP = 8000;
    communicater.port_terminal_UDP = 8001;
    communicater.port_server_UDP = 8001;
    P2P_Init(&communicater);

    return 0;
}