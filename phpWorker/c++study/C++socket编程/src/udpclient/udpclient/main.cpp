//
//  main.cpp
//  udpclient
//
//  Created by tangwei on 17/10/9.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include <stdlib.h>

#include <iostream>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <sys/wait.h>

#include <unistd.h>

#include <arpa/inet.h>

int main(int argc, const char * argv[]) {
    
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    int tmp = 1;
    
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&tmp, sizeof(tmp));//修改sock的属性为广播模式
    
    unsigned short port = 3309;
    
    sockaddr_in tmpsock;
    
    tmpsock.sin_family = AF_INET;
    
    tmpsock.sin_port = htons(port);
    
    tmpsock.sin_addr.s_addr = INADDR_BROADCAST; //inet_addr("127.0.0.1");//因为是广播，这儿就不要设置对方的IP地址了
    
    char str[1024] = "tangwei";
    
    long sendres = sendto(sock, str, sizeof(str), 0, (sockaddr *)&tmpsock, sizeof(tmpsock));
    
    char buffres[10240] = {0};
    
    sockaddr_in sockinput;
    
    socklen_t leng = sizeof(tmpsock);
    
    long len = recvfrom(sock, buffres, sizeof(buffres), 0, NULL, 0);
    
    buffres[len] = '\0';
    
    std::cout << buffres << std::endl;
    
    char strs[1024] = "tangwei666";
    
    long sendress = sendto(sock, strs, sizeof(strs), 0, (sockaddr *)&tmpsock, sizeof(tmpsock));
    
    
    
    
    //这儿是正常发送数据，并不能用在广播中
//    unsigned short port = 3309;
//    
//    sockaddr_in tmpsock;
//    
//    tmpsock.sin_family = AF_INET;
//    
//    tmpsock.sin_port = htons(port);
//    
//    tmpsock.sin_addr.s_addr = inet_addr("127.0.0.1");
//    
//    char str[1024] = "tangwei";
//    
//    long sendres = sendto(sock, str, sizeof(str), 0, (sockaddr *)&tmpsock, sizeof(tmpsock));
//    
//    char buffres[10240] = {0};
//    
//    sockaddr_in sockinput;
//    
//    socklen_t leng = sizeof(tmpsock);
//    
//    long len = recvfrom(sock, buffres, sizeof(buffres), 0, NULL, 0);
//    
//    buffres[len] = '\0';
//    
//    std::cout << buffres << std::endl;
//    
//    char strs[1024] = "tangwei666";
//    
//    long sendress = sendto(sock, strs, sizeof(strs), 0, (sockaddr *)&tmpsock, sizeof(tmpsock));
    //这儿是正常发送数据，并不能用在广播中
    
}
