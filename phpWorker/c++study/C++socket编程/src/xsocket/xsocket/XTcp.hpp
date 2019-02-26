//
//  XTcp.hpp
//  sockettw
//
//  Created by tangwei on 17/9/18.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#ifndef XTcp_hpp
#define XTcp_hpp

#include <stdio.h>

#include <stdlib.h>

#include <sys/types.h>

#include <sys/socket.h>//如何去查找socket依赖的库呢？就只要到命令行 man socket 即能看到！

#include <unistd.h>

#include <iostream>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <string.h>

#include <thread>

#include <sys/ioctl.h>

#include <fcntl.h>

#include <sys/select.h>

#include <regex>


class XTcp{

public:
    int CreateSocket();
    
    bool Bind(unsigned short port);
    
    bool Listen(int maxConnect = 10);
    
    long Recv(char *buf, int bufsize);
    
    long Send(const char * buf, long sendsize);
    
    bool Connect(const char * ip, unsigned short port, int timeouttms=1000);
    
    bool SetBlock(bool isblock);
    
    XTcp Accept();
    
    void Close();
    
    XTcp();
    
    ~XTcp();
    
    int sock = 0;
    
    unsigned short port = 3309;
    
    char ip[16];
};

#endif /* XTcp_hpp */
