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
    
#include "libavutil/channel_layout.h"//libavutil/channel_layout.h
    
#include "libswscale/swscale.h"
    
#include "libavcodec/avcodec.h"
    
#include "libavutil/frame.h"
    
#include "libavformat/avformat.h"
    
}

#include <fstream>

using namespace std;

int main(int argc, const char * argv[]) {
    
    avcodec_register_all();
    
    av_register_all();
    
    avformat_network_init();
    
    
    
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
    
    SwrContext *swr = NULL;
    
    swr = swr_alloc_set_opts(swr, av_get_default_channel_layout(2), AV_SAMPLE_FMT_FLTP, 44100, av_get_default_channel_layout(2), AV_SAMPLE_FMT_U8, 44100, 0, 0);
    
    swr_init(swr);
    
    AVFrame *pcm = av_frame_alloc();
    
    pcm->format = AV_SAMPLE_FMT_FLTP;
    
    pcm->channels = 2;
    
    pcm->channel_layout = av_get_default_channel_layout(2);
    
    pcm->nb_samples = 1024;//一通道一帧的样本量
    
    pcm->pts = 0;
    
    av_frame_get_buffer(pcm, 0);
    
    AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    
    AVCodecContext *ctx = avcodec_alloc_context3(codec);
    
    if (!ctx)
    {
        cout << "avcodec_alloc_context3 AV_CODEC_ID_AAC failed!" << endl;
        getchar();
        return -1;
    }
    
    ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    
    ctx->thread_count = 8;
    
    ctx->bit_rate = 40000;//一秒钟的比特数
    
    ctx->sample_rate = 44100;//编码成什么样的样本率
    
    ctx->sample_fmt = AV_SAMPLE_FMT_FLTP;
    
    ctx->channels = 2;
    
    ctx->channel_layout = av_get_default_channel_layout(2);
    
    int ret = avcodec_open2(ctx, 0, 0);
    
    if (ret != 0)
    {
        char err[1024] = { 0 };
        av_strerror(ret, err, sizeof(err) - 1);
        cout << err << endl;
        getchar();
        return -1;
    }
    
    
    
    AVFormatContext *otx = NULL;
    
    avformat_alloc_output_context2(&otx, 0, "flv", "rtmp://120.55.186.199/live");
    
    AVStream *ostream = avformat_new_stream(otx, 0);
    
    ostream->codecpar->codec_tag = 0;
    
    avcodec_parameters_from_context(ostream->codecpar, ctx);
    
    av_dump_format(otx, 0, "rtmp://120.55.186.199/live", 1);
    
    avio_open(&otx->pb, "rtmp://120.55.186.199/live", AVIO_FLAG_WRITE);
    
    int rets = avformat_write_header(otx, 0);
    
    if(rets < 0){
        
        char buff[1024] = {0};
        
        av_strerror(rets, buff, sizeof(buff)-1);
        
        return -8;
        
    }

    
    char *buf = new char[4096];
    
    int dpts = 0;
    
    AVPacket pkt = { 0 };
    
    const uint8_t *indata[8] = {0};
    
    indata[0] = (uint8_t *)buf;
    
    //buf就是获取到的音频数据
    
    while(true){
        
        if(input->bytesReady() < 4096) continue;
        
        int size = 0;
        
        while(size != 4096){
        
            int len = io->read(buf+size, 4096-size);
            
            if(len <0)break;
            
            size += len;
            
        }
        
        if(size != 4096) continue;
        
        
        /*
         std::fstream fsw("/Users/tangwei/Desktop/test.txt", std::ios::in | std::ios::out | std::ios::ate);把原始数据写入到文件，判断原数据是否有问题
        
        if(!fsw)
            std::cout << "文件打开失败" << std::endl;
        
    
        fsw << buf << std::endl;
            
        
        fsw.close();
         */
        swr_convert(swr, pcm->data, pcm->nb_samples, indata, pcm->nb_samples);
            
        pcm->pts += dpts;
            
        dpts = av_rescale_q(pcm->nb_samples, {1, 44100}, ctx->time_base);
            
        avcodec_send_frame(ctx, pcm);
            
        int r = avcodec_receive_packet(ctx, &pkt);
            
        if(r != 0)continue;
                
        std::cout << pkt.size << std::endl;
        
        if(pkt.size <= 6) continue;
            
        pkt.dts = av_rescale_q(pkt.dts, ctx->time_base, ostream->time_base);
            
        pkt.pts = av_rescale_q(pkt.pts, ctx->time_base, ostream->time_base);
            
        pkt.duration = av_rescale_q(pkt.duration, ctx->time_base, ostream->time_base);
                
        av_interleaved_write_frame(otx, &pkt);
        
    }
    
    
}
