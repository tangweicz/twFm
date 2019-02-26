//
//  main.cpp
//  testAudio
//
//  Created by tangwei on 17/11/3.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include <iostream>

#include "QtCore/QCoreApplication"

#include "QtMultimedia/QAudioFormat"

#include "QtMultimedia/QAudioInput"

#include "QtMultimedia/QAudioOutput"

extern "C"{
    
#include "libswresample/swresample.h"
    
#include "libavcodec/avcodec.h"
    
#include "libavutil/frame.h"
    
#include "libavformat/avformat.h"
    
}

#include <fstream>

#include "XMedioEncode.hpp"

#include "XRtmp.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    
    XMedioEncode*medio =  AVMedioEncode::Get();
    
    QAudioFormat fmt;
    
    fmt.setSampleRate(44100);
    
    fmt.setChannelCount(2);
    
    fmt.setSampleSize(8);
    
    fmt.setSampleType(QAudioFormat::UnSignedInt);
    
    fmt.setCodec("audio/pcm");
    
    fmt.setByteOrder(QAudioFormat::LittleEndian);
    
    QAudioDeviceInfo device = QAudioDeviceInfo::defaultInputDevice();
    
    if(!device.isFormatSupported(fmt)){
        
        std::cout << "设备不支持该类型的音频格式" << std:: endl;
        
        return -1;
        
    }
    
    QAudioInput *input = new QAudioInput(fmt);
    
    QIODevice *io = input->start();
    
    medio->initReSample();
    
    medio->channels = 2;
    
    medio->sampleRate = 44100;
    
    medio->inSampleFmt = AV_SAMPLE_FMT_U8;
    
    medio->OutSampleFmt = AV_SAMPLE_FMT_FLTP;
    
    medio->nb_samples = 1024;
    
    medio->initAudioCodec();
    
    XRtmp *rtmp = CXRtmp::Get();
    
    rtmp->url = "rtmp://120.55.186.199/live";
    
    rtmp->initForamt();
    
    rtmp->addStream(medio->auctx);
    
    rtmp->sendHeader();
    
    char *buf = new char[4096];
    
    int dpts = 0;
    
    while(true){
        
        if(input->bytesReady() < 4096) continue;
        
        int size = 0;
        
        while(size != 4096){
            
            int len = io->read(buf+size, 4096-size);
            
            if(len <0)break;
            
            size += len;
            
        }
        
        if(size != 4096) continue;
        
        medio->toFltp((char *)buf);
        
        medio->fltp->pts += dpts;
        
        dpts = av_rescale_q(medio->fltp->nb_samples, {1, 44100}, medio->auctx->time_base);
        
        medio->toAac();
        
        std::cout << medio->aupkt.size << std::endl;
        
        if(medio->aupkt.size <= 6) continue;
        
        rtmp->writeFrame(&medio->aupkt);
        
    }
    
    
}
