//
//  XTcp.cpp
//  sockettw
//
//  Created by tangwei on 17/9/18.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include "XTcp.hpp"

XTcp::XTcp(){

    this->sock = this->CreateSocket();
    
}

XTcp::~XTcp(){

    
    
}

int XTcp::CreateSocket(){
    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if(this->sock == -1){
        
        std::cout << "创建socket失败~" << std::endl;
        
        return 0;
        
    }
    
    return this->sock;
}

bool XTcp::Bind(unsigned short tmpport){
    if(tmpport > 128)
        this->port = tmpport;
    
    sockaddr_in tmp;
    
    tmp.sin_family = AF_INET;
    
    tmp.sin_port = htons(this->port);
    
    tmp.sin_addr.s_addr = htonl(0);
    
    int BindRes = bind(this->sock, (sockaddr *)&tmp, sizeof(tmp));
    
    if(BindRes == 0){
    
        return true;
        
    }else{
    
        return false;
        
    }
    
}

bool XTcp::Listen(int maxConnect){

    int listenRes = listen(this->sock, maxConnect);
    
    if(listenRes == 0){
    
        return true;
        
    }else{
    
        return false;
        
    }
    
}

XTcp XTcp::Accept(){
    
    XTcp tcp;

    sockaddr_in tmp;
    
    socklen_t len = sizeof(tmp);
    
//    fd_set set;
//    
//    FD_ZERO(&set);
//    
//    FD_SET(this->sock, &set);
//    
//    timeval tm;
//    
//    tm.tv_sec = 0;
//    
//    tm.tv_usec = 9000 *1000;
//    
//    if(select(this->sock + 1, &set, 0, 0, &tm)){
    
    tcp.sock = accept(this->sock, (sockaddr *)&tmp, &len);
        
    if(tcp.sock <= 0)return tcp;
    
    strcpy(tcp.ip, inet_ntoa(tmp.sin_addr));
        
    tcp.port = ntohs(tmp.sin_port);
        
    std::cout << "接收到" << tcp.ip << "port" << tcp.port << std::endl;
        
    //}
    
    return tcp;
    
}

void XTcp::Close(){

    close(this->sock);

}

long XTcp::Recv(char *buf, int bufzise){

    long recvlen = recv(this->sock, buf, bufzise - 1, 0);
    
    if(recvlen == -1 || recvlen == 0){//这儿一定要判断，否则，客户端连接失败会循环读取数据
    
        return -1;
        
    }
    
    return recvlen;
    
}


long XTcp::Send(const char * buf, long sendsize){
    
    long s = 0;
    
    while(s != sendsize){
    
        long len = send(this->sock, buf+s, sendsize-s, 0);//如果len大于剩余空间大小send就一直等待协议把sock的发送缓冲中的数据发送完，如果len小于剩余 空间大小send就仅仅把buf中的数据copy到剩余空间里（注意并不是send把sock的发送缓冲中的数据传到连接的另一端的，而是协议传的，send仅仅是把buf中的数据copy到sock的发送缓冲区的剩余空间里）。如果send函数copy数据成功，就返回实际copy的字节数
        
        if(len <= 0)break;
        
        s += len;
        
    }

    return s;
    
}

bool XTcp::Connect(const char * ip, unsigned short port, int timeouttms){
    
    if(this->sock <= 0)this->CreateSocket();
    
    sockaddr_in tmp;
    
    tmp.sin_family = AF_INET;
    
    tmp.sin_port = htons(port);
    
    tmp.sin_addr.s_addr = inet_addr(ip);
    
    fd_set set;
    
    int connres = connect(this->sock, (sockaddr *)&tmp, sizeof(tmp));

    if( connres != 0){//目前connect是阻塞的，即如果连接不上，进程会在这儿等待，但是，我们肯定不能让他无限期等待下去，所以就要设置一个阻塞超时！但是阻塞又会有问题，因为整个进程卡死在那边什么都不做，这样肯定不行。那么我们现在的解决方案是：设置connect为非阻塞，用连接的结果来确定是否连接上，如果没有连接上那么就重新连接。
        
        FD_ZERO(&set);//将set文件数组置空
        
        FD_SET(this->sock, &set);//将sock加入到set的文件数组中
        
        timeval tm;
        
        tm.tv_sec = 0;
        
        tm.tv_usec = timeouttms * 1000;
        
        printf("connect %s:%d failed!:%s\n",ip,port,strerror(errno));
        
        int selres = select(sock+1, 0, &set, 0, &tm);
        
        if( selres <= 0){//select本身也是阻塞函数，如果把timeva 设置为0 它就不再阻塞.  select返回0 就是超时 返回-1就是失败  所有文件描述符的范围，即所有文件描述符的最大值加1，不能错！readnfds: 指向检查读性的套接字集合的可选的指针。writefds: 指向检查可写性的套接字集合的可选的指针。exceptfds: 指向检查错误的套接字集合的可选的指针
        
            std::cout << "连接到ip：" << ip << "，端口：" << port << "失败" << std::endl;
        
            printf("connect %s:%d failed!:%s\n",ip,port,strerror(errno));
        
            return false;
        
        }else{
        
            if(FD_ISSET(this->sock, &set)){//判断句柄可写
            
                if(errno == 0){//这个errno是connect返回的，等于0为建立连接成功



                }
                
            }else{
            
                //连接超时
                
            }
            
            
            
        }
        
    }
    
    return true;
}

bool XTcp::SetBlock(bool isblock){

    if(this->sock <= 0)return false;
    
    int flags = fcntl(this->sock, F_GETFL, 0);//查看当前sock的阻塞状态
    
    if(flags < 0)return false;
    
    if(isblock){//如果设置为阻塞，即传值为true
        
        flags = flags&~O_NONBLOCK;
        
    }else{//如果设置为非阻塞，即传值为false
    
        flags = flags|O_NONBLOCK;
        
    }
    
    if(fcntl(this->sock, F_SETFL, flags) != 0)return false;
    
    return true;
    
}


















