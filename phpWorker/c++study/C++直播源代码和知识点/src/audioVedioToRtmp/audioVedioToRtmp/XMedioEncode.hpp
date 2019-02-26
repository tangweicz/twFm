

//
//  XMedioEncode.hpp
//  audioVedioToRtmp
//
//  Created by tangwei on 18/1/3.
//  Copyright © 2018年 tangwei. All rights reserved.
//



#ifndef XMedioEncode_hpp
#define XMedioEncode_hpp

#include <stdio.h>

#include <iostream>

extern "C"{

#include "libswresample/swresample.h"
    
#include "libavcodec/avcodec.h"
    
#include "libswscale/swscale.h"
    
}



class XMedioEncode{
    
public:
    
    static XMedioEncode *Get();//拿到类本身
    
    bool initReSample();//初始化音频重采样
    
    bool toFltp(char *audioData);//将数据转换成fltp的格式
    
    bool initAudioCodec();//初始化音频的编码器
    
    void toAAC();//将音频编码成aac的格式，用于后期H264的推流
    
    bool initSwsScale();//初始化视频转码器
    
    bool toYuv(char *vedioData);//将视频的RGB转换码YUV的格式
    
    bool initVedioCodec();//初始化视频的编码器
    
    void toH264();//将视频编码成H264的格式，用于推流
    
    SwrContext *swrCtx = NULL;
    
    AVFrame *audioFrame = NULL;
    
    AVCodecContext *audioCodecCtx = NULL;
    
    AVCodecContext *vedioCodecCtx = NULL;
    
    AVPacket audioPkt = {0};
    
    AVPacket vedioPkt = {0};
    
    SwsContext *swsCtx = NULL;
    
    AVFrame *vedioFrame = NULL;
    
    int vedioWidth = 0;
    
    int vedioHeight = 0;
    
    int colsNum = 0;
    
    int elemSize = 0;
    
};

#endif
