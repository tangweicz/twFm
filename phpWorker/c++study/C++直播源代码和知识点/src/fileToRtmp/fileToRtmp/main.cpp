//
//  main.cpp
//  fileToRtmp
//
//  Created by tangwei on 17/10/13.
//  Copyright © 2017年 tangwei. All rights reserved.
//
//  参考资料：http://www.cnblogs.com/findman/p/6050580.html

//  Stream: I P B B P B B // 存储顺序
//  PTS: 0312645
//  DTS: 0123456

//  解码器输出
//  Stream: I B B P B B P
//  PTS: 0123456
//  通常只有当显示B帧时，PTS和DTS才会不一致。
//
//
//
//  视频码率就是数据传输时单位时间传送的数据位数，一般我们用的单位是kbps即千位每秒。通俗一点的理解就是取样率，单位时间内取样率越大，精度就越高，处理出来的文件就越接近原始文件。
//
//  I帧间隔，I帧是视频的关键帧，I帧间隔就是I帧与I帧的时间间隔，时间间隔越大，那么视频直播的延时就约严重。
//
//  rtsp协议访问格式：rtsp://用户名:密码@IP地址  如果没有用户名和密码，就只要rtsp://IP地址即可
//
//
//  -----------------------------------------------------------------------------******************这个流程做成了图片，放在项目下的image文件夹下******************-------------------------------------------------------------

//

extern "C"{

#include "libavformat/avformat.h"//所有的ffmpeg都是C语言的库
    
#include "libavcodec/avcodec.h"
    
#include "libavutil/mathematics.h"
    
#include "libavutil/time.h"
    
}

#include <iostream>

int main(int argc, const char * argv[]) {
    
    av_register_all();//初始化所有的封装和解封装库，封装是指：FLV、mp4、mov、mp3等视频格式
    
    avformat_network_init();//初始化网络库
    
    //输入流代码区块开始
    
    //如果是mp4等格式的视频文件，需要打开文件，解封装文件协议头，开始打开文件
    
    AVFormatContext *ictx = NULL;// AVFormatContext 这其中包含 1、AVIOContext *pb 2、文件流个数 nb_streams 3、AVFormat **streams的流数组
    
    const char *filename = "/Users/tangwei/Downloads/123.flv";
    
    int openRes = avformat_open_input(&ictx, filename, NULL, NULL);
    
    if(openRes != 0){//如果出错了，这儿就把错误反馈出来
        char buff[1024] = {0};
        
        av_strerror(openRes, buff, sizeof(buff));
        
        std::cout << buff << std::endl;
        
        return -1;
    }
    
    std::cout << "open file " << filename << " success! " << std::endl;
    
    av_dump_format(ictx, 0, filename, 0);//打印获取到的信息
    
    std::cout << "--------------" << std::endl;
    //打开文件完毕
    
    //打开文件头完毕之后，文件的帧率、宽高信息都在头之后，这儿就是开始获取其他信息。如果没有文件头，那么直接获取其他信息，比如H264流。
    
    int findRes = avformat_find_stream_info(ictx, 0);//是上面打开文件时设置的
    
    if(findRes != 0){//如果出错了，这儿就把错误反馈出来
        char buff[1024] = {0};
        
        av_strerror(findRes, buff, sizeof(buff));//需要用到libutil.so这个动态库
        
        std::cout << buff << std::endl;
        
        return -2;
    }
    
    av_dump_format(ictx, 0, filename, 0);//打印获取到的信息
    
    //获取其他信息完毕
    
    //输入流代码区块完毕
    
    
    
    //输出流代码区块开始
    
    //创建输出流上下文开始
    AVFormatContext *octx = NULL;//输出流上下文
    
    const char *outUrl = "rtmp://120.55.186.199/live";//输出RTMP地址
    
    std::cout << "*******" << std::endl;
    
    int outre = avformat_alloc_output_context2(&octx, NULL, "flv", outUrl);//创建一个输出上下文的空间
    
    av_dump_format(octx, 0, outUrl, 1);
    
    std::cout << "*******" << std::endl;
    
    if(!octx){//创建输出空间失败
    
        char buff[1024] = {0};
        
        av_strerror(outre, buff, sizeof(buff));
        
        return -3;
        
    }
    
    //创建输出流上下文完毕
    
    //配置输出流开始
    
    for(int i = 0; i < ictx->nb_streams; i++){//遍历输入的avstream
    
        AVStream *out = avformat_new_stream(octx, ictx->streams[i]->codec->codec);//创建输出流 AVStream中有的数据：1、AVCodecContext codec 存放流的内部参数信息 有许多 比如 bit_rate、codec_type 2、AVCodecParameters codecpar 存放流的内部参数信息，用于替代codec的，但是目前还没有完全替代 3、time_base时间基
        
        if(out == NULL){
        
            std::cout << "输出流配置失败" << std::endl;
            
            return -4;
            
        }
        
        //int re = avcodec_copy_context(out->codec, ictx->streams[i]->codec);//复制配置信息,MP4才用这个
        
        avcodec_parameters_copy(out->codecpar, ictx->streams[i]->codecpar);//AVStream包含很多参数，用于记录通道信息，其中最重要的是  :AVCodecParameters * codecpar  ：用于记录编码后的流属性，即通道中存储的流的编码信息。这儿的时间基是不会被拷贝到的。
        
    }
    
    av_dump_format(octx, 0, outUrl, 1);
    //配置输出流完毕
    
    //输出流代码区块完毕
    
    
    //RTMP推流开始
    
    //打开网络通信连接
    
    int re = avio_open(&octx->pb, outUrl, AVIO_FLAG_WRITE);
    
    if(re < 0){
    
        char buff[1024] = {0};
        
        av_strerror(re, buff, sizeof(buff));
        
        
        return -5;
    }
    
    //打开网络通信连接完毕
    
    //写入头信息开始
    
    int writeres = avformat_write_header(octx, 0);
    
    if(writeres < 0){
    
        char buff[1024] = {0};
        
        av_strerror(writeres, buff, sizeof(buff));
        
        return -6;
        
    }
    
    //写入头信息完毕
    
    //推流每一帧数据开始
    
    AVPacket pkt;//其中的收据有：1、int stream_index 流索引，可以根据流索引来确定是音频还是视频 2、int size 数据大小 3、uint8_t *data 是数据 4、int64_t pts 显示时间戳，这个时间戳用来告诉播放器该在什么时候显示这一帧的数据 5、int64_t dts 解码时间戳，这个时间戳的意义在于告诉播放器该在什么时候解码这一帧的数据。视频中，有I、P、B帧的分别，I帧为关键数据，只解码I帧也能看到视频。P帧是帧间编码的方式，是向前编码，即本帧跟上一帧的变化。B帧也是帧间编码，是存放上一帧和下一帧之间的变化，需先解码。音频没有类似视频中 B 帧，不需要双向预测，所以音频帧的 DTS、PTS 顺序是一致的
    
    long long startTime = av_gettime();//获取开始推流的时间，后期需要做声话同步的时候用
    
    std::cout << startTime << "------" << std::endl;
    
    while(true){
    
        int readres = av_read_frame(ictx, &pkt);
        
        if(readres != 0){
        
            break;
            
        }
        
        std::cout << pkt.pts << std::endl;
        
        //计算转换pts、dts
        
        AVRational itime = ictx->streams[pkt.stream_index]->time_base;
        
        AVRational otime = octx->streams[pkt.stream_index]->time_base;
        
        pkt.pts = av_rescale_q_rnd(pkt.pts, itime, otime, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));//这儿要计算下，是因为，如果原来的视频格式是MP4，而我新建的资源上下文是FLV格式的，两者的time_base是不一样的，那么如果继续沿用之前的pts、dts、duration 就会带来问题，所以，需要按照之前的时间基、现在的时间基，算出新的pts、dts、duration的值。
        
        pkt.dts = av_rescale_q_rnd(pkt.dts, itime, otime, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        
        pkt.duration = av_rescale_q_rnd(pkt.duration, itime, otime, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));//duration 到这一帧视频为止已经经过了多少时间
        
        pkt.pos = -1;
        
        if(ictx->streams[pkt.stream_index]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){//如果是视频流，需要做声话不同步的问题
        
            AVRational tb = ictx->streams[pkt.stream_index]->time_base;
            
            long long nowTime = av_gettime() - startTime;//获取已经推了多久了
            
            long long dts = 0;//存放微秒数
            
            dts = pkt.dts*1000*1000*(double)tb.num/(double)tb.den;// tb.num/tb.den是计算文件dts的最小单位（秒），而文件dts是指走多了多少个最小单位。
            
            std::cout << dts << "xxxxxxxxxxxxxxx" << std::endl;
            
            if(dts > nowTime){
            
                av_usleep(dts - nowTime);
                
            }
        
        }
        
        int sendres = av_interleaved_write_frame(octx, &pkt);//这个函数在内部会根据pts排序发送，还会帮我们释放pkt的空间。所以就不要用av_packet_unref()了
        
        if(sendres < 0){
        
            char buff[1024] = {0};
            
            av_strerror(sendres, buff, sizeof(buff));
            
            return -7;
            
        }
        //av_packet_unref(&pkt);
        
    }
    
    //推流每一帧数据完毕
    
    
    
    //RTMP推流完毕
    
    
}











