//
//  XRtmp.hpp
//  rtsp_to_rtmp
//
//  Created by tangwei on 17/11/23.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#ifndef XRtmp_hpp
#define XRtmp_hpp

#include <stdio.h>

#include <string>

#include <iostream>


extern "C"{
    
#include "libavformat/avformat.h"
    
}

class XRtmp{

public:
    
    AVFormatContext *avfctx = NULL;
    
    std::string url = "rtmp://120.55.186.199/live";
    
    static XRtmp* Get();
    
    virtual bool initForamt() = 0;
    
    virtual int addStream(AVCodecContext *avctx) = 0;
    
    virtual bool sendHeader() = 0;
    
    virtual int writeFrame(AVPacket *pkt, int index) = 0;
    
    virtual void close() = 0;
    
};

class CXRtmp:public XRtmp{

public:
    bool initForamt();
    
    int addStream(AVCodecContext *avctx);
    
    int writeFrame(AVPacket *pkt, int index);
    
    void close();
    
    bool sendHeader();
    
private:
    
    AVStream *newStream = NULL;
    
    AVCodecContext *rtmpcodec = NULL;
    
    AVStream *vs = NULL;
    
    AVStream *as = NULL;
    
    AVCodecContext *actx = NULL;
    
    AVCodecContext *vctx = NULL;
    
    
};

#endif /* XRtmp_hpp */
