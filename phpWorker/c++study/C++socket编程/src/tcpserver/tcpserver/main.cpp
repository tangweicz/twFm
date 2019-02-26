//
//  main.cpp
//  twsocket
//
//  Created by tangwei on 17/9/12.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#if 0

tcp/ip协议分层

在数据包，经过网络传递之前，从用户发出数据到另一个用户收到数据，经过什么？

应用层    QQ/ftp/ssh等(具体待传输数据 data)                   QQ/ftp/ssh等(解包等到数据 data)
|
|
|
|
传输层       在数据前面打上端口信息                                 去掉数据前面端口信息
|
|
|
|
网络层       在数据前面打上IP信息                                   去掉数据前面IP信息------------------>分为：tcp/udp的数据部门和头部。              头部信息：                   源端口（16位）                    目的端口（16位）

序号（32位）
|
|                                                                                                                                                                                   保留号（32位）
|
|                                                                                                                                                                  数据偏移（4位）保留（4位）数据包的内容（6位）窗口（16位）
|
|                                                                                                                                                                  校验和（16位）                        紧急指针（16位）
|
|                                                                                                                                                                  选项（长度可变）                        填充
|
|
|
|
数据链路层      在数据前面打上mac地址信息                               去掉数据前面mac地址信息



tcp三次握手

客户端                                 服务器端

socket connect-------SYN J ---------->socket bind listen

<-------SYN K , ack J+1--

--------ack K+1-------->Accept返回

整个三次握手发生在客户端的connect阶段



http协议分析：
    HTTP/1.0是短连接
    HTTP/1.1支持持续连接(现在普遍支持，因为如果是用短连接，网页上的图片、js、css等都需要从服务器上拉取，建立一次连接拉一个css，建立一次连接拉一个js，这样的开销太大了。所以用1.1，只需要建立一次连接，拉取所有网页上的数据。)

    HTTP协议请求包括：（使用telnet能够看到信息的数据信息）
        Method Request-URI HTTP-Version CRLF

        Method分为（所有方法全为大写）：

            GET

            POST

            HEAD

            PUT

            DELETE

            TRACE

            CONNECT

            OPTIONS

    HTTP协议的反馈：
        HTTP/1.1 200 OK \r\n Date:时间\r\nContent-type:text/html\r\nContent-Length:xxx\r\nLast-Modified:时间\r\nConnection: Keep-Alive\r\n等一系列内容\r\n\r\n网页数据


正则表达式：
    引用 regex这个类库



#endif

#include "XTcp.hpp"

#include "tcpThread.hpp"

#include <thread>

#include <string>

#include <regex>

#include <fstream>

#include <signal.h>

#include <stdlib.h>

#include <sys/epoll.h>

#include <boost/regex.hpp>




//#include <stdlib.h>
//
//#include <sys/types.h>
//
//#include <sys/socket.h>//如何去查找socket依赖的库呢？就只要到命令行 man socket 即能看到！
//
//#include <unistd.h>
//
//#include <iostream>
//
//#include <netinet/in.h>
//
//#include <arpa/inet.h>
//
//#include <string.h>
//
//#include <thread>


//class TcpThread{
//public:
//    int tmpsocket;
//
//    void run();
//
//};
//
//void TcpThread::run(){
//
//    while(true){
//
//        char str[1024];
//
//        long recvlen = recv(tmpsocket, &str, sizeof(str) - 1, 0);
//
//        if(recvlen < 0){
//
//            std::cout << "recv data error" << std::endl;
//
//            break;
//
//        }
//
//        str[recvlen] = '\0';
//
//        std::cout << str << std::endl;
//
//        if(strstr(str, "quit") != NULL){
//
//            std::string outstr = "you quit success ! \n";
//
//            send(tmpsocket, &outstr, outstr.size() + 1, 0);//第二个参数指明一个存放应用程序要发送数据的缓冲区；第三个参数指明实际要发送的数据的字节数；第四个参数一般置0。
//
//            close(tmpsocket);
//
//        }
//
//
//        std::string outstr = "client send to server";
//
//        send(tmpsocket, &outstr, outstr.size() + 1, 0);
//
//    }
//
//}

int main(int argc, const char * argv[]) {//argc代表命令行输入参数的个数 argv存储了所有的命令行参数  注册存储进去的数据都是以字符串的形式
    
    
    //    int sock = socket(PF_INET, SOCK_STREAM, 0);//AF_INET为指定socket的协议是tcp/ip协议，SOCK_STREAM只接受tcp的数据，具体参数也可以通过 man socket 来查看，linux下对单个进程的文件句柄数是有限制的，用ulimit -n命令可以查看。所以socket的初始化，在一个进程里面不能启动超过ulimit -n个，否则会报错。
    //
    //    if(sock == -1)
    //        std::cout << "初始化socket失败" << std::endl;
    //
    //
    //    unsigned short port = 3309;//设置默认的端口号为3309
    //
    //    if(argc > 1){
    //
    //        port = atoi(argv[1]);//atoi输入的第二个参数变为数值形式
    //
    //    }
    //
    //    sockaddr_in tmp;
    //
    //    tmp.sin_family = AF_INET;
    //
    //    tmp.sin_port = htons(port);//这儿有个网络字节序的概念。不同的操作系统、平台，针对数据存储的字节序是有不同的，大致分为，小端法和大端法，比如 整型 2 二进制的顺序为00000000 00000000 00000000 00000010 在大端法上的字节序为0x187 0x186 0x185 0x184： 在小端法上的表示为：0x184 0x185 0x186 0x187,如果搞混的话 后果很严重。我们平常的电脑都是小端字节序，而网络字节序全部为大端序列。
    //
    //    tmp.sin_addr.s_addr = htonl(0);//内网 外网都可以访问
    //
    //    if(bind(sock, (sockaddr *)&tmp, sizeof(tmp)) != 0){//这儿bind内部使用的是，sockaddr这个结构体，但是它存在一定的弊端，所以我们这儿改用sockaddr_in，需要先引用
    //
    //        std::cout << "bind failed" << std::endl;
    //
    //        return -2;
    //
    //
    //    }
    //
    //    std::cout << "bind port success" << std::endl;
    //
    //    if(listen(sock, 10) == -1){//开始接受用户的连接,10是指最大连接数，具体含义为，如果有10个连接有数据传输，但是都没有用accept来接收数据，那么第11个进来，前面的第一个连接的数据就会被丢掉。如果被accept了，那么这里面的数字就会减一 那么就是9了。
    //
    //        std::cout << "listen failed" << std::endl;
    //
    //        return -3;
    //
    //    }
    //
    //    while(true){
    //
    //        sockaddr_in clientaddr;
    //
    //        socklen_t len = sizeof(clientaddr);
    //
    //        int client = accept(sock, (sockaddr *)&clientaddr, &len);//如果accept成功，那么会返回一个新的socket，这个socket就是用来跟连上来的客户端进行通信的。会阻塞等待。
    //
    //        //std::string clientip = inet_ntoa(clientaddr.sin_addr);//拿到客户端的IP信息
    //
    //        //unsigned short clientport = ntohs(clientaddr.sin_port);//拿到客户端的端口信息，需要将网络字节序改为本地字节序
    //
    //        TcpThread *tcp = new TcpThread;
    //
    //        tcp->tmpsocket = client;
    //
    //        std::thread sth(&TcpThread::run, tcp);//生成子线程，thread的标准用法，std::thread t1(&HelloWorld::myThread,this);//创建一个分支线程，回调到TcpThread函数里中的run函数
    //
    //        sth.detach();//detach调用之后，目标线程就成为了守护线程，驻留后台运行，与之关联的std::thread对象失去对目标线程的关联，无法再通过std::thread对象取得该线程的控制权。当线程主函数执行完之后，线程就结束了，运行时库负责清理与该线程相关的资源。
    //
    //    }
    //
    //
    //    close(sock);//停止整个服务端的socket服务
    
    signal(SIGPIPE, SIG_IGN);//因为我们用的是HTTP/1.1的协议，默认是长连接，客户端在连接上之后不断开，不发RST包，不四次握手。那么服务端如果close掉了这个socket，该socket会进入TIME_WAIT状态，然后会向客户端发送一个FIN段信息，客户端recv到FIN后，返回值为0，客户端知道服务端已经不能读写数据了，但是仍然可以往该socket中写入数据，而写入数据（send）只是将数据放入输入缓冲区就直接返回了，所以不会出错 ，TCP协议会将数据发送到服务端，但是，服务端已经不能够再次接受发送数据了，就给客户端恢复了一个RST信号（连接重置信号），而这个RST信号被记录在了客户端的TCP协议层中，此时，客户端再次调用send时，TCP此时已经是RST状态了，因此不会将数据发出去，而是给服务端发送一个SIGPIPE信号，导致整个进程退出。这儿就要忽略这个信号。
    
    
                            //TIME_WAIT状态，服务端主动关闭连接，会进入该状态，以等待最后一个ACK信号，等待时间为4分钟，如果超时没有ACK信号则正式关闭socket。
    
                            //CLOSE_WAIT状态，服务端被动关闭连接，会进入该状态，在收到客户端发送的FIN信号后，会关闭，如果这期间程序一直在I/O操作，那么该socket会一直存在。
    
    XTcp server;
    
    server.CreateSocket();
    
    unsigned short port = 0;
    
    if(argc > 1)
        port = atoi(argv[1]);
    
    if(!server.Bind(port))
        std::cout << "绑定到网卡失败！" << std::endl;
    
    if(!server.Listen(10))
        std::cout << "监听端口失败" << std::endl;
    
    server.SetBlock(false);
    
//    while(true){//这儿是每接收一个socket的连接，就开一个线程去处理连接，那么最好的方案是用epoll去处理，下面就是代码
//        
//        XTcp client = server.Accept();
//        
//        if(client.sock <= 0)continue;
//        
//        tcpThread *tcpthread = new tcpThread;
//        
//        tcpthread->sock = client;
//        
//        std::thread sth(&tcpThread::run, tcpthread);
//        
//        sth.detach();
//    }
    
    
    
    
    //拒绝使用线程，而是使用epoll去处理socket的连接，代码开始
    
    int epfd = epoll_create(256);//创建一个epoll，里面能放256个套接字
    
    epoll_event ev;
    
    ev.data.fd = server.sock;
    
    ev.events = EPOLLIN|EPOLLET;//EPOLLET模式下，同时有两个连接进来只会触发一次事件响应。EPOLLET模式类似于原来的select,只要还有没有处理的事件就会一直通知.
    
    epoll_ctl(epfd, EPOLL_CTL_ADD, server.sock, &ev);//将server.sock这个fd添加到epfd这个epoll中去，触发条件是：当前server.sock可读（就是有返回值 比如 有新的连接需要accept、 有数据需要recv(...) ）、对端关闭socket。
    
    epoll_event events[20];
    
    while(true){
    
        int num = epoll_wait(epfd, events, sizeof(events), 500);//events为上面定义的，表示等待20个事件或者500毫秒，超过20个事件或500毫秒就会往下走，而不会一直阻塞。返回产生socket的数量、具体产生的socket存放在events中
        
        if(num <= 0)continue;
        
        for(int i = 0;i < num; i++){
        
            if(events[i].data.fd == server.sock){
            
                while(true){
                
                    XTcp client = server.Accept();//在阻塞的情况下，2个同时进来系统默认只接收一个，所以，这儿只能用非阻塞的方式
                    
                    if(client.sock <= 0)break;
                
                    std::cout << "放入的" << client.sock << std::endl;
                
                    ev.data.fd = client.sock;
                
                    ev.events = EPOLLIN|EPOLLET;
                
                    epoll_ctl(epfd, EPOLL_CTL_ADD, client.sock, &ev);
                    
                }
                
            }else{
                
                
                
                char buf[1024] = {0};
                
                char bufres[10240] = {0};
            
                XTcp client;
                
                client.sock = events[i].data.fd;
                
                client.SetBlock(false);
                
                std::cout << "写入的" << client.sock << std::endl;
                
                while(true){//循环接收用户的数据
                    
                    long recvLen = client.Recv(buf, sizeof(buf));
                    
                    if(recvLen == -1){//如果没有接收到数据，或者是，接收数据失败，那么需要跳出循环
                        
                        break;
                        
                    }else{
                    
                        strcat(bufres, buf);
                        
                    }

                }
                
                client.SetBlock(true);
                
                std::string str = bufres;
                
                
                std::string patter = "^([A-Z]+) /(.*) HTTP/1";//匹配http的方式和路径参数,regex需要gcc版本大于4.9.3，具体升级可以百度下
                
                std::regex re(patter);

                std::smatch mas;
                
                std::regex_search(str, mas, re);
                
                if(mas.size() == 0){
                
                    std::cout << "failed!" << std::endl;
                    
                    epoll_ctl(epfd, EPOLL_CTL_DEL, client.sock, &ev);//将句柄从epoll中删除
                    
                    client.Close();//关闭客户端连接上来的服务器本地socket
                    
                    break;//如果没有匹配到数据，那么证明不是一个网页的http连接 那么直接退出 进入下一个连接处理
                    
                }
                
                std::string type = mas[1];
                
                std::string path = mas[2];
                
                std::string filename = "";
                
                if(path == ""){//有文件名才会去取数据
                    
                    filename = "/index.html";
                    
                }else{
                
                    filename = "/" + path;
                    
                }
                
                int filenamelen = filename.size() - 1;
                
                int pos = filename.find(".", 0);//对path进行甄别，查看是什么后缀，如果是index.php?id=1&name=123,那么下面的extension就是php?id=1&name=123
                
                std::string extension = filename.substr(pos+1, filenamelen);//如果存在query参数，那么到这儿extension为php?id=1&name=123&xxx=xxx,如果没有query参数，那么现在的extension为php/html等后缀名
                
                std::string quer = "";//初始化php-cig的参数字符串，如果没有后续参数处理那么就是空
                
                std::string query = "";
                
                int poslen = extension.find("?", 0);//如果extension里面有?被找到，证明有后续参数需要处理
                
                if(poslen > 1){//如果有后续参数进行处理，那么开始处理参数
            
                    std::string exten = extension.substr(0, poslen);//获取到从字符串开始到？之前的数据，即有后续参数的文件后缀名
                    
                    filename = filename.substr(0, pos);
                    
                    filename += ".";
                    
                    filename += exten;//拼接出文件名，并将filename重新赋值
                    
                    std::string que = extension.substr(poslen+1,extension.size()-1);//获取所有的后续参数，即？号之后的所有字符串数据
                    
                    std::string qu = "";
                    
                    int m = 0;
                    
                    while(true){
                        
                        int p = que.find("&", m);//查找到第一个&符的位置
                        
                        if(p <= 0){//如果没有，从起始位置截取到最后
                            
                            qu += que.substr(m, query.size()-1);
                            
                            break;
                            
                        }else{//如果有，那么从起始位置截取到&的位置
                            
                            qu += que.substr(m, p-m);
                            
                            qu += " ";
                            
                        }
                        
                        m = p + 1;
                        
                    }
                    
                    extension = exten;//赋值新的后缀名
                    
                    quer = qu;//初始化php-cig的参数字符串赋值
                    
                    query = que;
                }
                
                std::string contenttype = "";
                
                std::string filepath = "/root/sock/src/tcpserver/tcpserver/www";
                
                filepath += filename;
                
                if(extension == "html"){
                    
                    contenttype = "text/html";
                    
                }else if(extension == "php"){//这儿处理调用php-cgi解析php代码的过程
                    
                    contenttype = "text/html";
                    
                    if(type == "GET"){
                        
                        std::string cmd = "php-cgi ";//生成指令 php-cgi xxx.php id=1 name=tangwei > xxx.php.html中
                        
                        cmd += filepath;
                        
                        cmd += " ";
                        
                        cmd += quer;
                        
                        cmd += " > ";
                        
                        cmd += filepath;
                        
                        cmd += ".html";
                        
                        system(cmd.c_str());
                        
                        filepath += ".html";
                        
                    }else if(type == "POST"){//需要解除一个shell脚本来完成post的功能，因为测试下来system("export xxxxx")这个功能无效
                        
                        
                        
//                        shell脚本的内容为：
//                        export REDIRECT_STATUS=true
//                        export SCRIPT_FILENAME=$1
//                        export REQUEST_METHOD=POST
//                        export GATEWAY_INTERFACE=CGI/1.1
//                        export CONTENT_LENGTH=$2
//                        export CONTENT_TYPE=application/x-www-form-urlencoded
//                        export HTTP_COOKIE=PHPSESSID=vfg5csi76qpt3qlfml359ad210
//                        export QUERY_STRING=$3
//                        echo  $4 | php-cgi

//                        shell脚本使用：脚本名称./te 要执行的php文件：/root/sock/src/tcpserver/tcpserver/www/index.php post参数的长度：180 get参数：'id=123&name=kangkang' post参数：'test=hahahaha&ceshi=666'
                        
                        std::string postcmd = "";
                        
                        postcmd += "'";
                        
                        boost::regex reg("\"([a-zA-Z]*)?\"(\\r|\\n)*([a-zA-Z0-9]*)(\\r|\\n)*------");//"; name=((.|\\r|\\n)*?)------"
                        
                        boost::smatch ma;
                        
                        std::string::const_iterator start = str.begin();
                        
                        std::string::const_iterator end = str.end();
                        
                        while(boost::regex_search(start, end, ma, reg))
                        {
                            postcmd += ma[1];
                            
                            postcmd += "=";
                            
                            postcmd += ma[3];
                            
                            postcmd += "&";
                            
                            start = ma[0].second;
                        }
                        
                        postcmd = postcmd.substr(0, postcmd.size()-1);
                        
                        postcmd += "'";
                        
                        std::string cmd = "";
                        
                        cmd += "/root/sock/src/tcpserver/tcpserver/te";
                        
                        cmd += " ";
                        
                        cmd += filepath;
                        
                        cmd += " ";
                        
                        cmd += "180";
                        
                        cmd += " '";
                        
                        cmd += query;
                        
                        cmd += "' ";
                        
                        cmd += postcmd;//post拼凑的数据放在这儿
                        
                        cmd += " > ";
                        
                        cmd += filepath;
                        
                        cmd += ".html";
                        
                        system(cmd.c_str());
                        
                        filepath += ".html";
                        
                        
                    }

                }else if(extension == "jpeg"){
                    
                    contenttype = "image/jpeg";
                    
                }
                
                std::fstream fs(filepath, std::ios::in|std::ios::out|std::ios::binary);
                
                if(!fs){
                    
                    std::cout << "文件打开失败" << std::endl;
                    
                    epoll_ctl(epfd, EPOLL_CTL_DEL, client.sock, &ev);//将句柄从epoll中删除
                    
                    client.Close();//关闭客户端连接上来的服务器本地socket
                    
                    break;
                    
                }
                
                int filesize = 0;
                
                fs.seekg(0, std::ios::end);
                
                filesize = fs.tellg();
                
                fs.seekg(0, std::ios::beg);
                
                int headlen = 0;
                
                int xun = 0;
                
                if(extension == "php"){//这儿需要去掉php-cgi处理带来的头问题，去掉这个生成的头
                    
                    while(true){
                        char t[1] = {0};
                        
                        fs.read(t, sizeof(t));
                        
                        headlen ++;
                        
                        if(*t == '\r'){
                            
                            xun += 1;
                            
                            if(xun >= 2){
                                
                                headlen += 3;
                                
                                break;
                            }
                            
                            
                            
                        }
                        
                    }
                    
                    fs.seekg(0, std::ios::cur);
                    
                    filesize = filesize - headlen;
                    
                    
                    
                }
                
                std::string msg = "HTTP/1.1 200 OK\r\n";//设置需要返回给用户的数据
                
                msg += "Server: XHttp\r\n";
                
                msg += "Content-Type: ";
                
                msg += contenttype;
                
                msg += "\r\nContent-Length:";
                
                char slen[128] = {0};
                
                sprintf(slen,"%d",filesize);
                
                msg += slen;
                
                msg += "\r\n\r\n";
            
                client.Send(msg.c_str(), msg.size());//发送消息头
                
                while(true){
                    
                    if(fs.eof())break;
                    
                    char tmpbuf[1024] = {0};
                    
                    fs.read(tmpbuf, sizeof(tmpbuf));
                    
                    long l = client.Send(tmpbuf, sizeof(tmpbuf));//发送消息实体
                    
                    if(l <= 0)break;
                    
                }
                
                
                
                fs.close();
                
                epoll_ctl(epfd, EPOLL_CTL_DEL, client.sock, &ev);//将句柄从epoll中删除
                
                client.Close();//关闭客户端连接上来的服务器本地socket
                
            }
            
        }
        
    }
    
    //拒绝使用线程，而是使用epoll去处理socket的连接，代码完毕
    
    
    
    
    
}
