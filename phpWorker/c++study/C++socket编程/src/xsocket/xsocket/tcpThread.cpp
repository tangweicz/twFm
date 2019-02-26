//
//  tcpThread.cpp
//  sockettw
//
//  Created by tangwei on 17/9/18.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include "tcpThread.hpp"

void tcpThread::run(){
    
    while(true){

        char str[1024];
    
        long l = this->sock.Recv(str, sizeof(str));
        
        if(l == -1){
            
            std::cout << "该线程读取数据失败，或该线程的连接端已经丢失" << std::endl;
            
            break;
            
        }
            
        str[l] = '\0';
    
        std::cout << str << std::endl;
        
        char *sendata = "recv success";
        
        long sendlen = this->sock.Send(sendata, strlen(sendata)+1);
        
        if(sendlen == -1){
        
            std::cout << "发送数据失败，线程退出" << std::endl;
            
            break;
            
        }
    }
    
}
