//
//  XMedioEncode.hpp
//  rtsp_to_rtmp
//
//  Created by tangwei on 17/11/21.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#ifndef XMedioEncode_hpp
#define XMedioEncode_hpp

#include <stdio.h>

extern "C"{
    
#include <libswscale/swscale.h>
    
#include "libavutil/frame.h"
    
#include "libavcodec/avcodec.h"
    
#include "libavformat/avformat.h"
    
#include "libswresample/swresample.h"
    
#include "libavcodec/avcodec.h"
    
#include "libavutil/frame.h"
    
}

class XMedioEncode{
    
public:
    
    int inWidth = 1280;
    
    int inHeight = 720;
    
    int outWidth = 1280;
    
    int outHeight = 720;
    
    int PIXSize = 3;
    
    int fps = 25;
    
    int channels = 2;
    
    int sampleRate = 44100;
    
    AVSampleFormat inSampleFmt = AV_SAMPLE_FMT_U8;
    
    AVSampleFormat OutSampleFmt = AV_SAMPLE_FMT_FLTP;
    
    int nb_samples = 1024;
    
    AVPacket pkt = {0};
    
    AVPacket aupkt = {0};
    
    AVCodecContext *avctx = NULL;
    
    AVCodecContext *auctx = NULL;
    
    AVFrame *yuv = NULL;
    
    AVFrame *fltp = NULL;
    
    static XMedioEncode *Get();

    virtual bool initScale() = 0;
    
    virtual bool initReSample() = 0;
    
    virtual AVFrame * RGBToYUV(char *rgb) = 0;
    
    virtual AVFrame *toFltp(char *data) = 0;
    
    virtual void close() = 0;
    
    virtual bool initVideoCodec() = 0;
    
    virtual bool initAudioCodec() = 0;
    
    virtual void toH264() = 0;//这其中的packet不需要手动清理，每次调用自动清理
    
    virtual void toAac() = 0;//这其中的packet不需要手动清理，每次调用自动清理
    
};

class AVMedioEncode :public XMedioEncode{
    
public:
    
    bool initScale();
    
    AVFrame *RGBToYUV(char *rgb);
    
    AVFrame *toFltp(char *data);
    
    void close();
    
    bool initVideoCodec();
    
    void toH264();
    
    bool initReSample();
    
    bool initAudioCodec();
    
    void toAac();
    
private:
    
    SwsContext *sws = NULL;
    
    SwrContext *swr = NULL;
    
};


#endif /* XMedioEncode_hpp */
