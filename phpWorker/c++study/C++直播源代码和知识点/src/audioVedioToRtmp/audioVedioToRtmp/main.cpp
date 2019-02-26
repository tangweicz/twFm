//
//  main.cpp
//  audioVedioToRtmp
//
//  Created by tangwei on 17/12/28.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include <iostream>

#include <fstream>

#include "XAudioRecord.hpp"

#include "XVedioRecord.hpp"

#include "XMedioEncode.hpp"

#include "XRtmp.hpp"

int main(int argc, const char * argv[]) {
    
    long long beginTime = av_gettime();
    
    XAudioRecord *xaudiorecord = XAudioRecord::Get();//拿到音频的录制对象
    
    XMedioEncode *medioencode = XMedioEncode::Get();//拿到音频、视频的编码、转码类
    
    XVedioRecord *xvediorecord = XVedioRecord::Get();//拿到视频录制对象
    
    XRtmp *rtmp = XRtmp::Get();//拿到推流类对象
    
    xaudiorecord->start();
    
    bool openCameraRes = xvediorecord->start();//开始采集视频数据
    
    if(!openCameraRes){
    
        std::cout << "XVedioRecord start failed" << std::endl;
        
        return -1;
        
    }
    
    bool initavformat = rtmp->initAvFormat();//初始化推流上下文
    
    if(!initavformat){
    
        std::cout << "initavformat failed" << std::endl;
        
        return -1;
        
    }
    
    bool initReSampleRes = medioencode->initReSample();//初始化音频重采样上下文
    
    if(!initReSampleRes){
    
        std::cout << "initReSample failed" << std::endl;
        
        return -1;
        
    }
    
    bool initCodecRes = medioencode->initAudioCodec();//初始化编码器
    
    if(!initCodecRes){
    
        std::cout << "initCodec failed" << std::endl;
        
        return -1;
        
    }
    
    medioencode->vedioHeight = xvediorecord->frameHeight;
    
    medioencode->vedioWidth = xvediorecord->frameWidth;
    
    bool initSwsScaleRes = medioencode->initSwsScale();//初始化视频转码上下文
    
    if(!initSwsScaleRes){
    
        std::cout << "init sws_scale failed" << std::endl;
        
        return -1;
        
    }
    
    bool initVedioCodecRes = medioencode->initVedioCodec();//初始化视频编码器
    
    if(!initVedioCodecRes){
        
        std::cout << "initVedioCodec failed" << std::endl;
    
        return -1;
        
    }
    
    std::fstream fs;
    
    fs.open("/Users/tangwei/Desktop/test.txt", std::ios::in|std::ios::out|std::ios::trunc|std::ios::binary);
    
    if(!fs){
    
        std::cout << "open file /Users/tangwei/Desktop/test.txt failed" << std::endl;
        
        return -1;
        
    }
    
    int audioIndex = rtmp->addStream(medioencode->audioCodecCtx);
    
    int vedioIndex = rtmp->addStream(medioencode->vedioCodecCtx);
    
    bool openRes = rtmp->openIOWriteHeader();
    
    if(!openRes){
    
        std::cout << "openIOWriteHeader failed" << std::endl;
        
        return -1;
        
    }

    while(true){
    
        XData audioData = xaudiorecord->Pop();//取到录音的数据，下面就可以对数据进行处理
        
        XData vedioData = xvediorecord->Pop();//取到视频的数据，下面就可以对数据进行处理
        
        if(audioData.data == NULL && vedioData.data == NULL)continue;
        
        if(vedioData.leng >0){
            
            medioencode->colsNum = xvediorecord->colsNum;
            
            medioencode->elemSize = xvediorecord->elemSize;
        
            bool toYuvRes = medioencode->toYuv(vedioData.data);
            
            if(!toYuvRes){
            
                std::cout << "vedio toYuv failed" << std::endl;
                
                return -1;
                
            }
            
            medioencode->vedioFrame->pts = vedioData.timestamp - beginTime;
            
            medioencode->toH264();
            
            rtmp->pushFrame(medioencode->vedioPkt, vedioIndex);
            
            std::cout << medioencode->vedioPkt.size << std::endl;
            
            
            
        }
        
        if(audioData.leng > 0){
            
            bool toFltpRes = medioencode->toFltp(audioData.data);//对音频进行重采样
            
            if(!toFltpRes){
                
                std::cout << "audio toFltp failed" << std::endl;
                
                return -1;
                
            }
            
            medioencode->audioFrame->pts = audioData.timestamp - beginTime;
            
            medioencode->toAAC();
            
            rtmp->pushFrame(medioencode->audioPkt, audioIndex);
            

        }
        
    }
 
 
 
    
}



