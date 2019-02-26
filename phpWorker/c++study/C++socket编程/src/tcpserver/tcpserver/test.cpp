//
//  test.cpp
//  tcpserver
//
//  Created by tangwei on 17/9/26.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include <stdio.h>

#include <fstream>

#include <iostream>

#include <vector>

#include <string>

int main()
{
//    std::fstream fs("/Users/tangwei/Desktop/sock/src/tcpserver/tcpserver/www/index.html", std::ios::in|std::ios::out|std::ios::binary);
//    
//    if(!fs){
//        
//        std::cout << "文件打开失败" << std::endl;
//        
//        exit(0);
//        
//    }
//    
//    char tmpbuf[1024] = {0};
//    
//    while(fs.getline(tmpbuf, sizeof(tmpbuf), '\n')){
//        
//        std::cout << tmpbuf << std::endl;
//        
//        //sleep(1);
//        
//        //tmpbuf[1024] = {0};
//        
//        //if(fs.eof())break;
//        
//        
//    }
//    
//    fs.close();
    
//        char s[] = "Golden Global   View,disk * desk";
//        const char *d = " ,*";
//        char *p;
//        p = strtok(s,d);
//    
//        while(p)
//        {
//            printf("%s\n",p);
//            //p=strtok(NULL,d);
//        }
//        
//        return 0;
    
    
    //std::vector<float> arr(11.61);
    
    char str[1024];
    
    strcpy(str, "tagnwei");
    
    char *mm = str;
    
    std::cout << mm+2 << std::endl;
    
    return 0;
    
    
    
}
