//
//  main.cpp
//  udpxsocket
//
//  Created by tangwei on 17/10/8.
//  Copyright © 2017年 tangwei. All rights reserved.
//

/*
 * udp协议的的连接很简单，
 
 协议结构：
 16字节源端口    16字节目的端口
 16字节数据长度    16字节校验和
 
 数据值
 
 udp协议的socket是不需要listen的，如果你要listen只会无限失败
 
 recvfrom是会进程阻塞等待的
 
 客户端的连接不需要三次握手 创建完毕 可以直接sendto、recvfrom，在recvfrom中获取连接信息并记录，然后再通过获取到的信息将数据sendto
 
 *
 */

#include <stdlib.h>

#include <iostream>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <sys/wait.h>

#include <unistd.h>

#include <arpa/inet.h>

int main(int argc, const char * argv[]) {
    
    unsigned short port = 3309;
    
    if(argc > 1){
        
        port = atoi(argv[1]);
        
    }
    
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(sock < 0)
    {
        std::cout << "create socket faild" << std::endl;
        
        return -1;
    }
    
    sockaddr_in tmpaddr;
    
    tmpaddr.sin_family = AF_INET;
    
    tmpaddr.sin_port = htons(port);
    
    tmpaddr.sin_addr.s_addr = htonl(0);
    
    int bindres = bind(sock, (sockaddr *)&tmpaddr, sizeof(tmpaddr));
    
    if(bindres == -1){
        
        std::cout << "bind faild" << std::endl;
        
        return -2;
        
    }
    
    while(true){
        char buffres[10240] = {0};
        
        sockaddr_in sockinput;
        
        socklen_t leng = sizeof(sockinput);
        
        long len = recvfrom(sock, buffres, sizeof(buffres), 0, (sockaddr *)&sockinput, &leng);
        
        buffres[len] = '\0';
        
        std::cout << buffres << std::endl;
        
        std::cout << ntohs(sockinput.sin_port) << std::endl;
        
        std::cout << inet_ntoa(sockinput.sin_addr) << std::endl;
        
        sendto(sock, "recvtangwei", 9, 0, (sockaddr *)&sockinput, sizeof(sockinput));
    }
}
