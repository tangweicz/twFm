//
//  main.cpp
//  tcpclient
//
//  Created by tangwei on 17/9/21.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include <iostream>

#include "XTcp.hpp"

int main(int argc, const char * argv[]) {
    XTcp client;
    
    client.CreateSocket();
    
    if(!client.Connect("120.55.186.199", 3309, 5000)){
    
        std::cout << "连接到远程socket失败" << std::endl;
        
        return -1;
        
    }
    
    client.SetBlock(true);
    
    std::cout << "ok" << std::endl;
    
    char * str = "GET / HTTP/1.1";
    
    client.Send(str, strlen(str)+1);
    
    while(true){
    
        char recvdata[1024];
        
        long recvlen = client.Recv(recvdata, sizeof(recvdata));
        
        if(recvlen <= 0)break;
        
        std::cout << recvlen << std::endl;
        
        std::cout << recvdata << std::endl;
        
    }
    
    
}
