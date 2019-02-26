//
//  XUdp.hpp
//  xsocket
//
//  Created by tangwei on 17/10/10.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#ifndef XUdp_hpp
#define XUdp_hpp

#include <stdio.h>

#include <sys/socket.h>

#include <arpa/inet.h>

#include <unistd.h>

#include <stdlib.h>

#include <string.h>

#include <iostream>

class XUdp{

public:
    
    int createSocket();
    
    bool Bind(unsigned short port = 3309);
    
    void Close();
    
    long Recv(char *buff, int buffsize);
    
    char *ip;
    
    unsigned short port;
    
protected:
    int sock;
    
private:
    sockaddr_in * addr = NULL;
};

#endif /* XUdp_hpp */
