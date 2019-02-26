//
//  xSocket.cpp
//  xSocket
//
//  Created by tangwei on 18/3/23.
//  Copyright © 2018年 tangwei. All rights reserved.
//

#include <stdio.h>

#include "xSocket.hpp"

bool xSocket::xCreateSocket(){

    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if(this->sock == -1){
    
        std::cout << "创建socket失败" << std::endl;
        
        return false;
        
    }
    
    return true;
}

bool xSocket::xBind(const char *ip, unsigned short port){
    
    addrInfo.sin_family = AF_INET;
    
    addrInfo.sin_port   = htons(port);
    
    addrInfo.sin_addr.s_addr = htonl(0);
    
    int bindRes = bind(this->sock, (sockaddr *)&addrInfo, sizeof(addrInfo));
    
    if(bindRes != 0)return false;
    
    return true;
}

bool xSocket::xListen(int maxNumber = 10){
    
    int listenRes = listen(this->sock, maxNumber);
    
    if(listenRes < 0)return false;

    return true;
}

void xSocket::xclose(){

    close(this->sock);
    
}

void xSocket::xSetBlock(bool setOption){
    
    int flags = fcntl(this->sock, F_GETFL, 0);
    
    if(flags < 0){
    
        std::cout << "fcntl()读取当前socket状态失败" << std::endl;
        
        exit(0);
        
    }

    if(setOption){//设置为阻塞
        
        flags = flags&~O_NONBLOCK;
    
    }else{//设置为非阻塞
    
        flags = flags|O_NONBLOCK;
        
    }
    
    int setRes = fcntl(this->sock, F_SETFL, flags);
    
    if(setRes < 0){
    
        std::cout << "fcntl()设置当前socket状态失败" << std::endl;
        
        exit(0);
        
    }
    
}

xSocket xSocket::xAccept(){//获取客户端的连接
    
    
    
    
    
    xSocket client;
    
    sockaddr_in tmp;
    
    socklen_t len = sizeof(tmp);
    
    client.sock = accept(this->sock, (sockaddr *)&tmp, &len);
    
    if(client.sock <= 0)return client;
    
    client.port = ntohs(tmp.sin_port);
    
    std::cout << "接收到" << inet_ntoa(tmp.sin_addr) << "port" << client.port << std::endl;

    return client;
    
}

xSocket::xSocket(const xSocket &another){

    this->sock = another.sock;
    
    this->addrInfo = another.addrInfo;
    
}

long xSocket::xRecv(char *tmpdata, int leng){

    long len = recv(this->sock, tmpdata, leng, 0);
    
    return len;
}


long xSocket::xSend(char *tmpdata, int leng){
    
    long size = 0;
    
    while(true){
        
        if(size >= leng)break;
    
        long len = send(this->sock, tmpdata+size, leng-size, 0);
        
        size = size + len;
        
    }
    
    return size;
}

xSocket::xSocket(){}

bool xSocket::xConnect(char *ip, unsigned short port, int maxTime){
    
    bool tmpConnectRes = true;

    sockaddr_in tmp;
    
    tmp.sin_family = AF_INET;
    
    tmp.sin_port = htons(port);
    
    tmp.sin_addr.s_addr = inet_addr(ip);
    
    fd_set writeFd;
    
    int connectRes = connect(this->sock, (sockaddr *)&tmp, sizeof(tmp));
    
    if(connectRes < 0){
        
        if (errno != EINPROGRESS)return false;//因为连接用的非阻塞，如果返回EINPROGRESS，表明连接正在进行，所以，这儿不能退出
        
        FD_ZERO(&writeFd);
    
        FD_SET(this->sock, &writeFd);
        
        timeval timeSet;
        
        timeSet.tv_sec = 0;
        
        timeSet.tv_usec = maxTime * 1000;
        
        for (int i = 0; i <= 2; i++) {
            
            int selectRes = select(this->sock+1, 0, &writeFd, 0, &timeSet);
            
            if(selectRes <= 0){
                
                if(i == 2){
                    
                    std::cout << "连接失败，停止尝试连接" << std::endl;
                    
                    tmpConnectRes = false;
                
                }else{
                
                    std::cout << "第" << i+1 << "次连接失败，继续尝试下一次连接" << std::endl;
                    
                }
            
                
                
            }else{
            
                
                
                if(FD_ISSET(this->sock, &writeFd)){
                    
                    tmpConnectRes = true;
                        
                    break;
                
                }else{
                    
                    if(i == 2){
                        
                        std::cout << "连接句柄不可写，停止尝试连接" << std::endl;
                        
                        tmpConnectRes = false;
                        
                    }else{
                    
                        std::cout << "第" << i+1 << "次连接句柄不可写入，继续尝试下一次连接" << std::endl;
                        
                    }
                
                    
                }
                
            }
            
            
        }
        
        return tmpConnectRes;
        
    }else{
    
        return tmpConnectRes;
        
    }
      

}








