//
//  xSocket.hpp
//  xSocket
//
//  Created by tangwei on 18/3/22.
//  Copyright © 2018年 tangwei. All rights reserved.
//

#ifndef xSocket_h
#define xSocket_h

#include <iostream>

#include <unistd.h>

#include <fcntl.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <sys/select.h>

#include<errno.h>

#include<string.h>

class xSocket{

public:
    
    int sock;
    
    sockaddr_in addrInfo;
    
    sockaddr_in clientAddrInfo;
    
    unsigned short port;
    
    xSocket();
    
    xSocket(const xSocket &another);
    
    bool xCreateSocket();//创建socket
    
    bool xConnect(char *ip, unsigned short port, int maxTime);//连接客户端socket
    
    bool xBind(const char *ip, unsigned short port);//绑定socket服务
    
    bool xListen(int maxNumber);//开始监听socket服务，判断有没有连接进来
    
    void xSetBlock(bool setOption);//设置socket为阻塞还是非阻塞
    
    long xRecv(char *tmpdata, int leng);//接收数据
    
    long xSend(char *tmpdata, int leng);//发送数据
    
    void xclose();//关闭socket连接
    
    xSocket xAccept();//获取客户端的连接
    
};

#endif /* xSocket_h */
