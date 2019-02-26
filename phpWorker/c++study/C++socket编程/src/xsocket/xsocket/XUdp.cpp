//
//  XUdp.cpp
//  xsocket
//
//  Created by tangwei on 17/10/10.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include "XUdp.hpp"

int XUdp::createSocket(){

    this->sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(this->sock < 0){
        
        std::cout << "创建socket失败" << std::endl;
        
        exit(0);
    }

    return this->sock;
    
}

void XUdp::Close(){

    close(this->sock);
    
}

bool XUdp::Bind(unsigned short port){
    
    sockaddr_in tmp;
    
    tmp.sin_family = AF_INET;
    
    tmp.sin_port = htons(port);
    
    tmp.sin_addr.s_addr = htonl(0);

    int bindres = bind(this->sock, (sockaddr *)&tmp, sizeof(tmp));
    
    if(bindres == -1)return false;
    
    return true;
    
}

long XUdp::Recv(char *buff, int buffsize){
    
    if(this->addr == NULL){
    
        this->addr = new sockaddr_in;
        
    }
    
    socklen_t leng = sizeof(sockaddr_in);
    
    long recvres = recvfrom(this->sock, buff, buffsize, 0, (sockaddr *)this->addr, &leng);
    
    this->port = ntohs(this->addr->sin_port);
    
    this->ip = inet_ntoa(this->addr->sin_addr);
    
    std::cout << ntohs(this->addr->sin_port) << std::endl;
    
    std::cout << inet_ntoa(this->addr->sin_addr) << std::endl;
    
    return recvres;
    
}
