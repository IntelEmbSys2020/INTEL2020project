#include "p2p.hpp"

#define __USER_DEBUG_P2P_CPP__

/*	@brief:	    通信初始化（进行TCP握手，UDP穿透）
 *	@notice:    注意在使用该函数之前需要对服务器地址进行初始化
 *	@author:    江榕煜（2020.10.3）
 *	@param:     
                p2p对象
 *	@retv:	        创建成功or失败
**/
bool P2P_Init(p2p * target)
{
    int sendRet,recvRet;
    int recvAppID;
    int addrLength = sizeof(target->addr_recv);
    switch (target->localType)
    {
    case TERMINAL:
        target->socket_UDP = socket(AF_INET,SOCK_DGRAM,0);
        target->socket_TCP = socket(AF_INET,SOCK_STREAM,0);
        if(target->socket_UDP == -1 || target->socket_TCP == -1)
        {
            return false;
        }

        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"套接字创建成功!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //初始化UDP套接字，绑定端口
        target->addr_send.sin_family = AF_INET;     //IPv4地址
        target->addr_send.sin_addr.s_addr = htonl(INADDR_ANY);                  //任意对象
        target->addr_send.sin_port = htons(target->port_server_UDP);  //本地开放端口
        if( bind(   target->socket_UDP,
                            (struct sockaddr *)&(target->addr_send),
                            sizeof(target->addr_send)   ) 
            < 0)
        {
            return false;
        }
        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"UDP套接字绑定端口!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //TCP套接字绑定端口
        target->addr_send.sin_addr.s_addr = htonl(INADDR_ANY);
        target->addr_send.sin_port = htons(target->port_server_TCP);
        if(bind(  target->socket_TCP,
                            (struct sockaddr *)&(target->addr_send),
                            sizeof(target->addr_send)   )
                < 0)
        {
            return false;
        }

        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"TCP套接字端口绑定!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/


        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"等待和TCP服务器握手"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //TCP套接字与服务器握手
        target->addr_send.sin_family = AF_INET;     //IPv4地址
        target->addr_send.sin_addr.s_addr = inet_addr(target->IPv4_server_TCP); //服务器地址
        target->addr_send.sin_port = htons(target->port_server_TCP);  //服务器端口
        if( connect(    target->socket_TCP,           //与服务器TCP握手
                        (struct sockaddr *)&(target->addr_send),
                        sizeof(target->addr_send)   ) 
            == -1)
        {
                /*****************调试辅助打印(START)********************/
                #ifdef __USER_DEBUG_P2P_CPP__
                std::cout<<"TCP服务器握手 ERROR!"<<std::endl;
                #endif
                /*****************调试辅助打印(END)********************/

            return false;
        }

        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"TCP服务器握手 成功!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //注册设备口令(UDP)
        target->addr_send.sin_addr.s_addr = inet_addr(target->IPv4_server_UDP);    //目标服务器IP
        target->addr_send.sin_port    = htons(target->port_server_UDP);           //端口设置导入
        sendRet = sendto(target->socket_UDP,        //UDP套接字
                        &target->APP_ID,             //口令（app号）
                        sizeof(target->APP_ID),     //口令大小
                        0,  //flag
                        (struct sockaddr *)&(target->addr_send),    //发送服务器
                        sizeof(target->addr_send));
        
        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"terminal UDP send AppID:"<< target->APP_ID << " success!"<<std::endl;
        std::cout<<"等待服务器UDP响应"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        recvRet = recvfrom(target->socket_UDP,&recvAppID,sizeof(recvAppID),0,
                            (sockaddr *)&(target->addr_recv),(socklen_t *)&(addrLength));
        memcpy(&(target->addrCache),&(target->addr_recv.sin_addr.s_addr),4);
        std::cout<<"接受到响应，IP："<<inet_ntoa(target->addrCache)
            <<".端口："<<ntohs(target->addr_recv.sin_port)<<std::endl;

        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"P2P穿透FINISH"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        break;
    
    default:
        break;
    }
}

/*	@brief:	        发送数据给对方
 *	@notice:        用的是UDP
 *	@author:    江榕煜（2020.10.3）
 *	@param:     
                target（p2p对象）
                data（指针）发送数据的头指针
                dataLength（size_t）数据长度
 *	@retv:	        发送成功or失败
**/
bool P2P_sendData(p2p *target,void * data,size_t dataLength)
{
    int sendRet;
    switch (target->localType)
    {
    case TERMINAL:
        target->addr_send.sin_family = AF_INET;     //IPv4地址
        target->addr_send.sin_addr.s_addr = inet_addr(target->IPv4_server_UDP);    //终端发向地面站
        target->addr_send.sin_port = htons(target->port_server_UDP);  //地面站端口
        
        sendRet = sendto(target->socket_UDP,
                            data,dataLength,
                            0,
                            (sockaddr *)&(target->addr_send),
                            sizeof(target->addr_send));
        if(sendRet == -1)
            return false;
        break;

    default:
        return false;
        break;
    }

    return true;
}

/*	@brief:	        发送命令or重要数据给对方
 *	@notice:        用的是TCP
 *	@author:    江榕煜（2020.10.3）
 *	@param:     
                target（p2p对象）
                data（指针）发送数据的头指针
                dataLength（size_t）数据长度
 *	@retv:	        发送成功or失败
**/
bool P2P_sendCMD(p2p *target,void * data,size_t dataLength)
{
    int sendRet;
    int socketNow;
    switch (target->localType)
    {
    case TERMINAL:
        sendRet = send(target->socket_TCP,
                        data,dataLength,0);
        if(sendRet == -1)
            return false;
        break;

    default:
        return false;
        break;
    }

    return true;

}

/*	@brief:	        接收数据
 *	@notice:        用的是UDP
 *	@author:    江榕煜（2020.10.3）
 *	@param:     
                target（p2p对象）
                data（指针）数据接收池的头指针
                dataLength（size_t）接收池长度
 *	@retv:	        发送成功or失败
**/
long P2P_recvData(p2p *target,void * data,size_t dataLength)
{
    int addrLength = sizeof(target->addr_recv);
    int recvRet;
    switch (target->localType)
    {
    case TERMINAL:
        do
        {
            recvRet = recvfrom(target->socket_UDP,
                                data,dataLength,
                                0,
                                (sockaddr *)&(target->addr_recv),
                                (socklen_t *)&addrLength);
            if(recvRet == -1)
                return -1;
            if(target->addr_recv.sin_addr.s_addr != inet_addr(target->IPv4_server_UDP))
                continue;
        } while (0);
        
        
        break;

    default:
        return -1;
        break;
    }

    return recvRet;

}

/*	@brief:	        接收命令or重要数据
 *	@notice:        用的是TCP
 *	@author:    江榕煜（2020.10.3）
 *	@param:     
                target（p2p对象）
                data（指针）数据接收池的头指针
                dataLength（size_t）接收池长度
 *	@retv:	        发送成功or失败
**/
long P2P_recvCMD(p2p *target,void * data,size_t dataLength)
{
    int addrLength = sizeof(target->addr_recv);
    int recvRet;
    switch (target->localType)
    {
    case TERMINAL:
        recvRet = recv(target->socket_TCP,
                        data,dataLength,0);
        if(recvRet == -1)
            return -1;
        
        break;

    default:
        return -1;
        break;
    }

    return recvRet;
    
}

/*	@brief:	        清理P2P对象使用的网络内核栈
 *	@notice:        此函数谨慎使用！BE CAREFUL！
 *	@author:      江榕煜（2020.10.6）
 *	@param:
                target（p2p对象）
 *	@retv:	            无
**/
void P2P_stackClean(p2p*target)
{
    static char history[1024];
    static int addrLength = sizeof(target->addr_recv);
    while (recv(target->socket_TCP,history,1024,MSG_DONTWAIT) == 1024){}
    while (recvfrom(target->socket_UDP,
                        history,1024,
                        0,
                        (sockaddr *)&(target->addr_recv),
                        (socklen_t *)&addrLength)==1024){}
    return;
}
