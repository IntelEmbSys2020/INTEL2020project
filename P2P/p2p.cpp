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
    int sendRet;
    int recvRet;
    TCPcommand ctrlMsg;
    int recvAppID;
    auto addrLength = sizeof(target->addr_recv);

    memset(&(target->addr_send),0,sizeof(struct sockaddr_in));      //默认数据填充0
    memset(&(target->addr_recv),0,sizeof(struct sockaddr_in));      //默认数据填充0
    
    target->addr_send.sin_family = AF_INET;                       //IPV4地址类型
    target->addr_recv.sin_family = AF_INET;                       //IPV4地址类型
    target->addr_send.sin_addr.s_addr = htonl(INADDR_ANY);        //自动获取IP地址
    target->addr_send.sin_addr.s_addr = htonl(INADDR_ANY);

    switch (target->localType)
    {
    case TERMINAL:      //作业端初始化
        //首先创建UDP和TCP套接字
        target->socket_UDP  = socket(AF_INET,SOCK_DGRAM,0);
        target->socket_TCP_local = socket(PF_INET,SOCK_STREAM,0);
        if(target->socket_UDP == -1 || target->socket_TCP_local == -1)
        {
            return false;
        }

        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"terminal create socket success!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //初始化UDP套接字，绑定端口
        target->addr_send.sin_family = AF_INET;     //IPv4地址
        target->addr_send.sin_addr.s_addr = htonl(INADDR_ANY);                  //任意对象
        target->addr_send.sin_port = htons(target->port_terminal_UDP);  //本地开放端口
        if( bind(   target->socket_UDP,
                            (struct sockaddr *)&(target->addr_send),
                            sizeof(target->addr_send)   ) 
            < 0)
        {
            return false;
        }
        
        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"terminal UDP socket bind success!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //TCP套接字绑定端口
        target->addr_send.sin_port = htons(target->port_terminal_TCP);
        if(bind(  target->socket_TCP_local,
                            (struct sockaddr *)&(target->addr_send),
                            sizeof(target->addr_send)   )
                < 0)
        {
            return false;
        }

        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"terminal TCP socket bind success!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/


        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"terminal TCP waiting connect with server!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //TCP套接字与服务器握手
        target->addr_send.sin_family = AF_INET;     //IPv4地址
        target->addr_send.sin_addr.s_addr = inet_addr(target->IPv4_server); //服务器地址
        target->addr_send.sin_port = htons(target->port_server_TCP);  //服务器端口
        if( connect(    target->socket_TCP_local,           //与服务器TCP握手
                                    (struct sockaddr *)&(target->addr_send),
                                    sizeof(target->addr_send)   ) 
            == -1)
        {
            return false;
        }

        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"terminal TCP connect server success!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/



        /*********开始UDP穿透*********/

        do  //等待服务器指示开始穿透
        {
            recvRet = recv(target->socket_TCP_local,&ctrlMsg,sizeof(ctrlMsg),0);
            if(recvRet == -1 )
                return false;
            if(ctrlMsg == P2P_begin)    //校验开始指令
                continue;
        } while (0);
        
        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"terminal get server CMD : begin make hole!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //注册设备口令
        target->addr_send.sin_addr.s_addr = inet_addr(target->IPv4_server);    //目标服务器IP
        target->addr_send.sin_port    = htons(target->port_server_UDP);           //端口设置导入
        sendRet = sendto(target->socket_UDP,        //UDP套接字
                                                &target->APP_ID,             //口令（app号）
                                                sizeof(target->APP_ID),//口令大小
                                                0,  //flag
                                                (struct sockaddr *)&(target->addr_send),    //发送服务器
                                                sizeof(target->addr_send));

        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"terminal UDP send AppID:"<< target->APP_ID << " success!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        do  //等待服务器指示测试穿透目标IP字符串
        {
            recvRet = recv(target->socket_TCP_local,
                                            &(target->IPv4_station),
                                            sizeof(target->IPv4_station),0);
            if(recvRet == -1 )
                return false;
        } while (0);
        target->addr_send.sin_addr.s_addr = inet_addr(target->IPv4_station);
        
        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"terminal get target IP success! IP:"
                <<target->IPv4_station<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        do  //等待服务器指示测试穿透目标端口
        {
            recvRet = recv(target->socket_TCP_local,
                                            &(target->port_station_UDP),
                                            sizeof(target->port_station_UDP),0);
            if(recvRet == -1 )
                return false;
        } while (0);
        target->addr_send.sin_port = target->port_station_UDP;

        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"terminal get target UDP port success! port:"
                <<target->port_station_UDP<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //本地对目标穿透一下
        sendRet = sendto(target->socket_UDP,
                                                &(target->APP_ID),sizeof(target->APP_ID),
                                                0,
                                                (sockaddr *)&(target->addr_send),sizeof(target->addr_send));

        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"terminal has made a local NAT hole!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //接收目标的穿透尝试消息
        do
        {
            recvRet = recvfrom(target->socket_UDP,&recvAppID,sizeof(recvAppID),
                0,(sockaddr *)&(target->addr_recv),(socklen_t *)sizeof(target->addr_recv));
            if (recvRet == -1)
            {
                return false;
            }
            if(target->addr_recv.sin_addr.s_addr != target->addr_send.sin_addr.s_addr)
                continue;
        } while (0);

        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"terminal has get massage from station!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //反馈穿透成功命令给服务器
        ctrlMsg = P2P_OK;
        sendRet = send(target->socket_TCP_local,&(ctrlMsg),sizeof(ctrlMsg),0);

        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"terminal has feed back success to server!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        /***********穿透结束************/
        break;
    






    case SERVER:
        //先创建socket
        target->socket_TCP_local = socket(PF_INET,SOCK_STREAM,0);
        target->socket_UDP = socket(AF_INET,SOCK_DGRAM,0);
        if(target->socket_UDP == -1 || target->socket_TCP_local == -1)
        {
            return false;
        }

        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"server has create socket success!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //绑定服务器TCP端口号
        target->addr_send.sin_family = AF_INET;
        target->addr_send.sin_addr.s_addr = htonl(INADDR_ANY);
        target->addr_send.sin_port = htons(target->port_server_TCP);
        if( bind(
                target->socket_TCP_local,
                (struct sockaddr *)&(target->addr_send),
                sizeof(target->addr_send)) 
            == -1)
        {
            return false;
        }
                
        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"server TCP socket bind success!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //绑定UDP端口号
        target->addr_send.sin_port = htons(target->port_server_UDP);
        if( bind(
                target->socket_UDP,
                (struct sockaddr *)&(target->addr_send),
                sizeof(target->addr_send)) 
            == -1)
        {
            return false;
        }
        
        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"server UDP socket bind success!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //启动监听，并设置最高连接数量为4
        if(listen(target->socket_TCP_local,4) == -1)
        {
            return false;
        }
        
        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"server TCP listen begin success!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //握手
            //先地面站
            target->socket_TCP_ConnectStation = accept(target->socket_TCP_local,
                                                        (struct sockaddr *) &(target->addr_recv),
                                                        (socklen_t *)&addrLength   );
            if(target->socket_TCP_ConnectStation == -1)
                return false;
            memcpy(&(target->addrCache),&(target->addr_recv.sin_addr.s_addr),4);
            strcpy(target->IPv4_station,inet_ntoa(target->addrCache));  //存储地面站IP
            /*****************调试辅助打印(START)********************/
            #ifdef __USER_DEBUG_P2P_CPP__
            std::cout<<"server connect with station success!"
                    <<"station IP is: "<<target->IPv4_station<<std::endl;
            #endif
            /*****************调试辅助打印(END)********************/

            //后作业端
            target->socket_TCP_ConnectTerminal = accept(target->socket_TCP_local,
                                                        (struct sockaddr *) &(target->addr_recv),
                                                        (socklen_t *)&addrLength   );
            if(target->socket_TCP_ConnectTerminal == -1)
                return false;
            memcpy(&(target->addrCache),&(target->addr_recv.sin_addr.s_addr),4);
            strcpy(target->IPv4_terminal,inet_ntoa(target->addrCache)); //存储作业端IP
            
            /*****************调试辅助打印(START)********************/
            #ifdef __USER_DEBUG_P2P_CPP__
            std::cout<<"server connect with Terminal success!"
                    <<"Terminal IP is: "<<target->IPv4_terminal<<std::endl;
            #endif
            /*****************调试辅助打印(END)********************/

        
        //对地面站发送穿透开始指令
        ctrlMsg = P2P_begin;
        sendRet = send(target->socket_TCP_ConnectStation,&ctrlMsg,sizeof(ctrlMsg),0);
        if(sendRet == -1)
        {
            return false;
        }
        
        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"server send begin massage success!"<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //接收地面站发来的口令
        recvRet = recvfrom(target->socket_UDP,
                                                    &recvAppID,
                                                    sizeof(recvAppID),
                                                    0,
                                                    (sockaddr *)&(target->addr_recv),
                                                    (socklen_t *)&addrLength);
        if(recvRet == -1)
        {
            return false;
        }
        target->port_station_UDP = target->addr_recv.sin_port;
                
        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"server recv AppID from station success!"<<std::endl;
        std::cout<<"station UDP port is :"<< target->port_station_UDP<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/

        //校验口令
        if(recvAppID != target->APP_ID)
            return false;

        //对作业端发送穿透开始指令
        ctrlMsg = P2P_begin;
        sendRet = send(target->socket_TCP_ConnectTerminal,&ctrlMsg,sizeof(ctrlMsg),0);
        if(sendRet == -1)
        {
            return false;
        }

        //接收作业端发来的口令
        recvRet = recvfrom(target->socket_UDP,
                                                    &recvAppID,
                                                    sizeof(recvAppID),
                                                    0,
                                                    (sockaddr *)&(target->addr_recv),
                                                    (socklen_t *)&addrLength);
        if(recvRet == -1)
        {
            return false;
        }
        target->port_terminal_UDP = target->addr_recv.sin_port;
                
        /*****************调试辅助打印(START)********************/
        #ifdef __USER_DEBUG_P2P_CPP__
        std::cout<<"server recv AppID from terminal success!"<<std::endl;
        std::cout<<"terminal UDP port is :"<< target->port_terminal_UDP<<std::endl;
        #endif
        /*****************调试辅助打印(END)********************/


        //校验口令
        if(recvAppID != target->APP_ID)
            return false;

        //-----------发送地面站的IP给作业端(TCP)--------
        
        sendRet = send(target->socket_TCP_ConnectTerminal,
                                                &(target->IPv4_station),strlen(target->IPv4_station),
                                                0);
        //发送地面站UDP端口号给作业端
        sendRet = send(target->socket_TCP_ConnectTerminal,
                                                &(target->port_station_UDP),sizeof(target->port_station_UDP),
                                                0);

        //----------发送作业端的IP给地面站(TCP)--------
        
        sendRet = send(target->socket_TCP_ConnectStation,
                                                &(target->IPv4_terminal),strlen(target->IPv4_terminal),
                                                0);
        //发送作业端UDP端口号给地面站
        sendRet = send(target->socket_TCP_ConnectStation,
                                                &(target->port_terminal_UDP),sizeof(target->port_terminal_UDP),
                                                0);

        //接收成功打洞信号
        recvRet = recv(target->socket_TCP_ConnectTerminal,
                                    &(ctrlMsg),sizeof(ctrlMsg),0);

        //把好消息告诉地面站
        sendRet = send(target->socket_TCP_ConnectStation,
                                    &(ctrlMsg),sizeof(ctrlMsg),0);

        break;

    case WORKSTATION:
        /* code */
        break;
    
    default:
        return false;
        break;
    }
    return true;
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
        target->addr_send.sin_addr.s_addr = inet_addr(target->IPv4_station);    //终端发向地面站
        target->addr_send.sin_port = htons(target->port_station_UDP);  //地面站端口
        
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
        sendRet = send(target->socket_TCP_local,
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
            if(target->addr_recv.sin_addr.s_addr != inet_addr(target->IPv4_station))
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
        recvRet = recv(target->socket_TCP_local,
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

/*	@brief:	        P2P中转服务
 *	@notice:        建议开一个线程不断调用该函数，执行一次相当于中转一次
 *	@author:    江榕煜（2020.10.3）
 *	@param:     
                target（p2p对象）
 *	@retv:	        工作成功or错误
**/
bool P2P_serverTransfer(p2p *target)
{
    if(target->localType != SERVER)
        return false;
    
    static char buffer[20]; 

    // do{
        //接收地面站
        int recvRet = recv(target->socket_TCP_ConnectStation,
                                                &buffer,20,MSG_DONTWAIT);   //非阻塞
        if(recvRet == -1)
        {
            return false;
        }
        if(recvRet > 0 )
        {   //转作业端
            if(send(target->socket_TCP_ConnectTerminal,
                        &buffer,recvRet,0)
                    == -1)
                return false;
            return true;
        }

        //接收作业端
        recvRet = recv(target->socket_TCP_ConnectTerminal,
                                        &buffer,20,MSG_DONTWAIT);
        if(recvRet == -1)
        {
            return false;
        }
        if(recvRet > 0 )
        {   //转地面站
            if(send(target->socket_TCP_ConnectStation,
                        &buffer,recvRet,0)
                    == -1)
                return false;
            return true;
        }

        // continue;
    // }while(0);

    return true;
}

