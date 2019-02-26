//
//  main.cpp
//  udpSyslogSocket
//
//  Created by tangwei on 17/10/10.
//  Copyright © 2017年 tangwei. All rights reserved.
//

/*
 *  这儿是做，将一台服务器的syslog的信息，通过UDP发送到另一台服务器上。
 *  
 *  修改/etc/rsyslog.conf文件，加上一句：
 *                                  *.* @121.41.9.17 //将服务器的所有级别的syslog通过UDP发送到121.41.9.17这台服务器
 *                                  *.* @@121.41.9.17 //将服务器的所有级别的syslog通过TCP发送到121.41.9.17这台服务器
 *
 *  重启rsyslog服务
 *
 *  发送邮件到163邮箱：
 *                  1、设置163邮箱的授权码
 *                  2、打开163邮箱的POP3/SMTP服务
 *                  3、在centos服务器上安装mailx    yum install mailx
 *                  4、配置mailx服务，编辑/etc/mail.rc 添加：
 *                                                          set from=18168820608@163.com
 *                                                          set smtp=smtp.163.com
 *                                                          set smtp-auth-user=18168820608@163.com
 *                                                          set smtp-auth-password=授权码
 *                                                          set smtp-auth=login
 *                  5、运行命令发送邮件：
 *                                  echo '邮件内容' | mailx -s "标题" 18168820608@163.com
 */

#include <iostream>

#include "XUdp.hpp"

#include <regex>

#include <stdlib.h>

int main(int argc, const char * argv[]) {
    XUdp syslog;
    
    syslog.createSocket();
    
    bool bindres = syslog.Bind(3309);
    
    if(!bindres){
    
        std::cout << "绑定到网卡失败" << std::endl;
        
        return -1;
        
    }
    
    char buff[1024] = {0};
    
    int leng = sizeof(buff);
    
    while(true){
    
        long len = syslog.Recv(buff, leng);
        
        if(len <= 0)continue;
        
        buff[len] = '\0';
        
        //如果登录失败，会报：<86>Oct 10 17:15:23 iZ23t1smx5nZ sshd[28756]: Failed password for root from 153.101.30.12 port 30276 ssh2 这个错误，那么我们现在可以对其进行分析，然后做出预警
        
        std::string str = buff;
        
        std::regex re("Failed password for ([a-zA-Z0-9]+) from ([0-9.]+)");
        
        std::smatch mas;
        
        std::regex_search(str, mas, re);
        
        if(mas.size() > 0){
        
            std::cout << "用户：" << mas[1] << "，在IP:"<< mas[2] << "，上登录失败，请留意" << std::endl;
            
            std::string cmd = "";
            
            cmd += "echo '";
            
            cmd += "用户：";
            
            cmd +=  mas[1];
            
            cmd += "，在IP:";
            
            cmd += mas[2];
            
            cmd += "，上登录失败，请留意";
            
            cmd += "' | ";
            
            cmd += "mailx -s '服务器警报' 18168820608@163.com";
            
            std::cout << cmd << std::endl;
            
            system(cmd.c_str());
            
            //利用linux的mail命令，发送邮件处理
            
        }

        std::cout << buff << std::endl;
        
    }
    
    
    
    
}
