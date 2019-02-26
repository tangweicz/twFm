//
//  XRtmp.hpp
//  audioVedioToRtmp
//
//  Created by tangwei on 18/1/9.
//  Copyright © 2018年 tangwei. All rights reserved.
//

#ifndef XRtmp_hpp
#define XRtmp_hpp

#include <stdio.h>

#include <iostream>

extern "C"{

    #include "libavformat/avformat.h"
    
}



class XRtmp{

public:
    static XRtmp *Get();
    
    bool initAvFormat();
    
    int addStream(AVCodecContext *codecCtx);
    
    bool openIOWriteHeader();
    
    int pushFrame(AVPacket pkt, int streamIndex);
    
private:
    
    const char *url = "rtmp://120.55.186.199/live";
    
    AVFormatContext *avFormatCtx = NULL;
    
    AVStream *audioStream = NULL;
    
    AVStream *vedioStream = NULL;
    
    AVCodecContext *audioCtx = NULL;
    
    AVCodecContext *vedioCtx = NULL;
    
};

#endif /* XRtmp_hpp */
