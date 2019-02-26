

//
//  main.cpp
//  testAudio
//
//  Created by tangwei on 17/11/3.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include <iostream>

#include <fstream>

#include "XMedioEncode.hpp"

#include "XRtmp.hpp"

#include "XAudioRecord.hpp"

#include "XVideoRecord.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    
    long long begintime = av_gettime();
    
    XMedioEncode*medio =  XMedioEncode::Get();
    
    XVideoRecord *vedio = XVideoRecord::Get();
    
    vedio->init(0);
    
    medio->PIXSize = 3;
    
    medio->inHeight = vedio->frameHeight;
    
    medio->inWidth = vedio->frameWidth;
    
    medio->outHeight = vedio->frameHeight;
    
    medio->outWidth = vedio->frameWidth;
    
    bool initres = medio->initScale();
    
    if(!initres)medio->close();
    
    bool res = medio->initVideoCodec();
    
    if(!res) medio->close();
    
    vedio->start();
    
    XAudioRecord *audio = XAudioRecord::Get(X_AUDIO_QT5);
     
    audio->start();
    
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
    
    int a = rtmp->addStream(medio->avctx);
    
    cout << "xxx" << a << endl;
    
    int b = rtmp->addStream(medio->auctx);
    
    cout << "xxx" << b << endl;
    
    rtmp->sendHeader();
    
    while(true){
        
        XDATA audiodata =  audio->Pop();
         
        XDATA vediodata =  vedio->Pop();
        
        if(audiodata.dataSize <= 0 && vediodata.dataSize <= 0)continue;
        
        if(vediodata.dataSize > 0){
        
            medio->RGBToYUV(vediodata.data);
            
            vediodata.drop();
            
            if(medio->yuv == NULL) medio->close();
            
            medio->yuv->pts = vediodata.pts - begintime;
            
            medio->toH264();
            
            std::cout << medio->pkt.size << "vedio" << std::endl;
            
            rtmp->writeFrame(&medio->pkt, a);
            
        }
        
        if(audiodata.dataSize > 0){
            
            medio->toFltp(audiodata.data);
            
            audiodata.drop();
            
            medio->fltp->pts = audiodata.pts - begintime;
            
            medio->toAac();
            
            std::cout << medio->aupkt.size << "---------audio" << std::endl;
            
            //rtmp->writeFrame(&medio->aupkt, b);
            
        }

        
        
    }
    
    
}

