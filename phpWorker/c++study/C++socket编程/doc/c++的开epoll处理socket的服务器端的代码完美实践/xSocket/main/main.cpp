//
//  main.cpp
//  xSocket
//
//  Created by tangwei on 18/3/22.
//  Copyright © 2018年 tangwei. All rights reserved.
//

#include <iostream>

#include <sys/epoll.h>

#include "xSocket.hpp"

#include <sys/signal.h>

#include <string>

#include <regex>

#include <fstream>

int main(int argc, const char * argv[]) {
    
    signal(SIGPIPE, SIG_IGN);
    
    xSocket server;
    
    server.xCreateSocket();
    
//    char *ip = "192.168.6.104";
//    
//    server.xSetBlock(false);
//    
//    bool connRes = server.xConnect(ip, 8089, 5);
//    
//    if(!connRes){
//    
//        std::cout << "fail" << std::endl;
//        
//    }else{
//    
//        std::cout << "success" << std::endl;
//        
//    }
//    
//    server.xSetBlock(true);
//    
//    std::cout << "ok" << std::endl;
//    
//    char * str = "GET / HTTP/1.1";
//    
//    server.xSend(str, strlen(str)+1);
//    
//    while(true){
//        
//        char recvdata[1024] = {0};
//        
//        long recvlen = server.xRecv(recvdata, sizeof(recvdata));
//        
//        if(recvlen <= 0)continue;
//        
//        std::cout << recvlen << std::endl;
//        
//        std::cout << recvdata << std::endl;
//        
//    }
    
   

    
    
    const char *ip = "0";

    unsigned short port = 8089;

    bool bindRes = server.xBind(ip, port);
    
    if(!bindRes){
    
        std::cout << "绑定socket到本地失败！！！" << std::endl;
        
        exit(0);
    }
    
    bool listenRes = server.xListen(10);
    
    if(!listenRes){
    
        std::cout << "监听socket到本地失败！！！" << std::endl;
        
        exit(0);
        
    }
    
    int epfd = epoll_create(256);//创建一个epoll，里面能放256个套接字
    
    epoll_event ev;
    
    ev.data.fd = server.sock;
    
    ev.events = EPOLLIN|EPOLLET;//EPOLLET模式下，同时有两个连接进来只会触发一次事件响应。EPOLLET模式类似于原来的select,只要还有没有处理的事件就会一直通知.
    
    epoll_ctl(epfd, EPOLL_CTL_ADD, server.sock, &ev);//将server.sock这个fd添加到epfd这个epoll中去，触发条件是：当前sock可读（就是有返回值 比如 有新的连接需要accept、 有数据需要recv(...) ）、对端关闭socket。
    
    epoll_event events[20];//表示等待20个事件
    
    server.xSetBlock(false);//将server端改成非阻塞的状态
    
    while (true) {
        
        int num = epoll_wait(epfd, events, sizeof(events), 500);
        
        if(num <= 0)continue;
        
        for (int i =0; i < num; i++) {
            
            if(events[i].data.fd == server.sock){//如果是连接操作
                
                while(true){
                
                    std::cout << "尝试连接" << std::endl;
                    
                    xSocket client = server.xAccept();
                    
                    if(client.sock < 0)break;
                    
                    ev.data.fd = client.sock;
                    
                    ev.events = EPOLLIN|EPOLLET;
                    
                    epoll_ctl(epfd, EPOLL_CTL_ADD, client.sock, &ev);

                    
                }
                
                
            }else{//如果不是连接操作，那么就是接收和发送数据
                
                xSocket clientTmp;
            
                clientTmp.sock = events[i].data.fd;
                
                clientTmp.xSetBlock(false);//下面要开始接收数据，所以要关闭阻塞
                
                char strStr[10240] = {0};
                
                bool canSendData = true;//用来标注，是否需要将数据发送出去，如果为false，那么不发送数据，而是直接关闭连接
                
                while (true) {
                    
                    char strTmp[1024] = {0};
                    
                    long recvSize = clientTmp.xRecv(strTmp, sizeof(strTmp));
                    
                    if(recvSize == -1){//如果接收出错。(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)的情况下认为连接是正常的，继续接收。但是我们这儿是循环接收数据，一般出现EINTR、EWOULDBLOCK、EAGAIN是已经没有数据了，就可以跳出循环了。
                        
                        if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN){
                        
                            canSendData = true;
                            
                            break;
                            
                        }else{
                            
                            std::cout << "获取到-1关闭连接" << std::endl;
                        
                            epoll_ctl(epfd, EPOLL_CTL_DEL, clientTmp.sock, &ev);//将句柄从epoll中删除
                            
                            clientTmp.xclose();
                            
                            canSendData = false;
                            
                            break;
                            
                        }
                        
                        
                        
                    }else if(recvSize == 0){//客户端主动关闭连接，且没有收到数据，我们会接收到0,我们要主动关闭这个连接
                        
                        std::cout << "获取到0关闭连接" << std::endl;
                        
                        epoll_ctl(epfd, EPOLL_CTL_DEL, clientTmp.sock, &ev);//将句柄从epoll中删除
                            
                        clientTmp.xclose();
                            
                        canSendData = false;
                            
                        break;
                        
                    }else{//正常连接
                        
                        strcat(strStr, strTmp);
                    
                        canSendData = true;
                    
                    }
                
                
                }
                
                if(canSendData){
                    
                    std::cout << "recvData :" << strStr << std::endl;//GET /index.php HTTP/1.1
                    
                    std::string strrecv = strStr;
                    
                    std::string guize = "^([A-Z]+) /(.*) HTTP/1";
                    
                    std::regex re(guize);
                    
                    std::smatch mas;
                    
                    std::regex_search(strrecv, mas, re);
                    
                    std::string type = mas[1];
                    
                    std::string path = mas[2];
                    
                    std::cout << "请求类型：" << type << std::endl;//匹配到请求的类型 是 GET 还是 POST
                    
                    std::cout << "请求路径：" << path << std::endl;//匹配到请求的路径 /index.php?page=1&name=tangwei
                    
                    size_t fileSize = 0;
                    
                    std::regex reg("([a-z0-9A-Z]+\.[a-z0-9A-z]+)\?(.*)");
                    
                    std::smatch mam;
                    
                    std::regex_search(path, mam, reg);
                    
                    std::string filenames = mam[1];//匹配到请求的文件名 如： index.php index.html index.js
                    
                    std::string querypath = mam[2];//匹配到请求后续参数 ?page=1&name=.....
                    
                    std::cout << filenames << std::endl;
                    
                    std::cout << querypath << std::endl;
                    
                    std::fstream tmpfsr(filenames, std::ios::in);//读取请求的文件，需要判断文件存在不存在
                    
                    std::string filename = "";
                    
                    if(!tmpfsr){//如果文件不存在
                    
                        filename = "404.html";
                        
                    }else{
                    
                        std::regex reextension("([0-9a-zA-Z]+)\.([0-9a-zA-Z]+)");
                        
                        std::smatch magg;
                        
                        std::regex_search(filenames, magg, reextension);
                        
                        std::string file = magg[1];
                        
                        std::string extension = magg[2];
                        
                        if(extension == "php"){//如果是php后缀的
                            
                            if(type == "GET"){
                                
                                
                            }else if( type == "POST"){
                                
                                
                                
                            }
                            
                            filename = filenames+".html";
                            
                            std::string cmd = "php-cgi "+filenames+" > "+filename;
                            
                            system(cmd.c_str());
                            
                            
                            
                        }
                        
                        
                    }
                    
                    std::fstream fsr(filename, std::ios::in | std::ios::binary);
                    
                    fsr.seekg(0, std::ios::end);
                    
                    fileSize = fsr.tellg();
                    
                    std::cout << "文件大小：" << fileSize << std::endl;
                    
                    clientTmp.xSetBlock(true);
                    
                    std::string msg = "HTTP/1.1 200 OK\r\n";//设置需要返回给用户的数据
                    
                    msg += "Server: XHttp\r\n";
                    
                    msg += "Content-Type: text/html";
                    
                    msg += "\r\nContent-Length:";
                    
                    msg += fileSize;
                    
                    msg += "\r\n\r\n";
                    
                    char *mag = const_cast<char *>(msg.c_str());
                    
                    clientTmp.xSend(mag, strlen(mag));//发送消息头
                    
                    fsr.seekg(0, std::ios::beg);
                    
                    while(true){//最好是这样去读取文件内容
                        
                        if(fsr.eof())break;
                        
                        char tmpstr[1024] = {0};
                        
                        fsr.read(tmpstr, sizeof(tmpstr));
                        
                        int sendSize = clientTmp.xSend(tmpstr, strlen(tmpstr));//发送消息体
                        
                        std::cout << "发送数据大小：" << sendSize << std::endl;
                        
                    }
        
                    std::cout << "正常关闭连接" << std::endl;
                    
                    epoll_ctl(epfd, EPOLL_CTL_DEL, clientTmp.sock, &ev);//将句柄从epoll中删除
                    
                    clientTmp.xclose();//当建立一个socket连接后，它是以文件描述符的形式在程序发挥作用

                    
                }
                
                
            }
            
        }
        
    }
    
}
