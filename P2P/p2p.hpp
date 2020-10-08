#ifndef _P2P_H_
#define _P2P_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>

/***********NOTATION************
该方案仅适用于IPv4
双信道方案：TCP+UDP
命令和数据分离传输
一个服务器，两个NAT后设备P2P
*************END***************/

//设备类型枚举变量
enum deviceType{
    TERMINAL,           //作业终端
    SERVER,                //服务器
    WORKSTATION  //工作总站
};

//TCP命令集
enum TCPcommand{
    P2P_begin = 1,
    P2P_OK = 2,
    P2P_ERROR = 3
};

struct p2p
{
    //应用口令，安全用
    int APP_ID;
    //本地设备需要的服务类型
    deviceType localType;
    //作业端IP和端口
    char IPv4_terminal[16];
    int port_terminal_TCP;
    int port_terminal_UDP;
    //服务器IP和端口
    char IPv4_server[100];
    int port_server_TCP;
    int port_server_UDP;
    //总站IP和端口
    char IPv4_station[16];
    int port_station_TCP;
    int port_station_UDP;

    //命令传输TCP套接字
    int socket_TCP_local;
    //TCP握手套接字(服务器用)
    int socket_TCP_ConnectTerminal;
    int socket_TCP_ConnectStation;

    //主数据传输UDP套接字
    int socket_UDP;

    //数据发送地址设置器
    struct sockaddr_in addr_send;
    //数据接收地址设置器
    struct sockaddr_in addr_recv;

    //IP地址字符串转换中转变量
    struct in_addr addrCache;
};

/*	@brief:	    通信初始化（进行TCP握手，UDP穿透）
 *	@notice:    注意在使用该函数之前需要对服务器地址进行初始化
                            先开机地面站，再开无人机！
 *	@author:    江榕煜（2020.10.3）
 *	@param:     
                p2p对象
 *	@retv:	        创建成功or失败
**/
bool P2P_Init(p2p * target);

/*	@brief:	        发送数据给对方
 *	@notice:        用的是UDP
 *	@author:    江榕煜（2020.10.3）
 *	@param:     
                target（p2p对象）
                data（指针）发送数据的头指针
                dataLength（size_t）数据长度
 *	@retv:	        发送成功or失败
**/
bool P2P_sendData(p2p *target,void * data,size_t dataLength);

/*	@brief:	        发送命令or重要数据给对方
 *	@notice:        用的是TCP
 *	@author:    江榕煜（2020.10.3）
 *	@param:     
                target（p2p对象）
                data（指针）发送数据的头指针
                dataLength（size_t）数据长度
 *	@retv:	        发送成功or失败
**/
bool P2P_sendCMD(p2p *target,void * data,size_t dataLength);

/*	@brief:	        接收数据
 *	@notice:        用的是UDP
 *	@author:    江榕煜（2020.10.3）
 *	@param:     
                target（p2p对象）
                data（指针）数据接收池的头指针
                dataLength（size_t）接收池长度
 *	@retv:	        发送成功or失败
**/
long P2P_recvData(p2p *target,void * data,size_t dataLength);

/*	@brief:	        接收命令or重要数据
 *	@notice:        用的是TCP，建议挂线程
 *	@author:    江榕煜（2020.10.3）
 *	@param:     
                target（p2p对象）
                data（指针）数据接收池的头指针
                dataLength（size_t）接收池长度
 *	@retv:	        发送成功or失败
**/
long P2P_recvCMD(p2p *target,void * data,size_t dataLength);

/*	@brief:	        P2P中转服务
 *	@notice:        建议开一个线程不断调用该函数，执行一次相当于中转一次
 *	@author:    江榕煜（2020.10.3）
 *	@param:     
                target（p2p对象）
 *	@retv:	        工作成功or错误
**/
bool P2P_serverTransfer(p2p *target);

/*	@brief:	        清理P2P对象使用的网络内核栈
 *	@notice:        此函数谨慎使用！BE CAREFUL！
 *	@author:      江榕煜（2020.10.6）
 *	@param:
                target（p2p对象）
 *	@retv:	            工作成功or错误
**/
void P2P_stackClean(p2p*target);

#endif