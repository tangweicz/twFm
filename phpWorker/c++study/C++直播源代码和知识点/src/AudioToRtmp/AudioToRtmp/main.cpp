//
//  main.cpp
//  AudioToRtmp
//
//  Created by tangwei on 17/11/23.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include <iostream>

#include <string>

#include <QtMultimedia/QAudioFormat>

#include <QtMultimedia/QAudioDeviceInfo>

#include <QtCore/QIODevice>

#include <QtMultimedia/QAudioInput>

extern "C"{

#include <libswresample/swresample.h>
    
#include "libavutil/frame.h"
    
#include <libavcodec/avcodec.h>
    
#include <libavformat/avformat.h>
    
}

using namespace std;

int main(int argc, const char * argv[]) {
    
    avcodec_register_all();
    
    av_register_all();
    
    avformat_network_init();
    
    QAudioFormat fmt;
    
    fmt.setChannelCount(2);
    
    fmt.setSampleRate(44100);
    
    fmt.setSampleSize(8);
    
    fmt.setSampleType(QAudioFormat::UnSignedInt);
    
    fmt.setCodec("audio/pcm");
    
    fmt.setByteOrder(QAudioFormat::LittleEndian);
    
    QAudioDeviceInfo device = QAudioDeviceInfo::defaultInputDevice();
    
    if(!device.isFormatSupported(fmt)){
    
        cout << "Device Not Support" << endl;
        
        return -1;
        
    }
    
    QAudioInput *input = new QAudioInput(fmt);
    
    QIODevice *io = input->start();
    
    SwrContext *swrctx = NULL;
    
    swrctx = swr_alloc_set_opts(swrctx, av_get_default_channel_layout(2), AV_SAMPLE_FMT_FLTP, 44100, av_get_default_channel_layout(2), AV_SAMPLE_FMT_U8, 44100, 0, 0);
    
    swr_init(swrctx);
    
    AVFrame *pcm = av_frame_alloc();
    
    pcm->channels = 2;
    
    pcm->channel_layout = av_get_default_channel_layout(2);
    
    pcm->nb_samples = 1024;//一通道一帧的样本量
    
    pcm->pts = 0;
    
    pcm->format = AV_SAMPLE_FMT_FLTP;
    
    av_frame_get_buffer(pcm, 0);
    
    AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    
    AVCodecContext * aucodecctx = avcodec_alloc_context3(codec);
    
    aucodecctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    
    aucodecctx->thread_count = 8;
    
    aucodecctx->bit_rate = 40000;//一秒钟的比特数
    
    aucodecctx->sample_rate = 44100;//编码成什么样的样本率
    
    aucodecctx->sample_fmt = AV_SAMPLE_FMT_FLTP;
    
    aucodecctx->channels = 2;
    
    aucodecctx->channel_layout = av_get_default_channel_layout(2);

    avcodec_open2(aucodecctx, 0, 0);
    
    char *buff = new char[4096];
    
    const uint8_t *indata[8] = {0};
    
    indata[0] = (uint8_t *)buff;
    
    int dpts = 0;
    
    AVPacket pkt = {0};
    
    AVFormatContext *avfmctx = NULL;
    
    avformat_alloc_output_context2(&avfmctx, NULL, "flv", "rtmp://120.55.186.199/live");

    AVStream *stream = NULL;
    
    stream = avformat_new_stream(avfmctx, codec);
    
    stream->codecpar->codec_tag = 0;

    avcodec_parameters_from_context(stream->codecpar, aucodecctx);

    avio_open(&avfmctx->pb, "rtmp://120.55.186.199/live", AVIO_FLAG_WRITE);
    
    avformat_write_header(avfmctx, NULL);
    
    while(true){
    
        if(input->bytesReady() < 4096) continue;
        
        int size = 0;
        
        while(size != 4096){
        
            int len = io->read(buff+size, 4096-size);
            
            if(len < 0) break;
            
            size = size + len;
            
        }
        
        if(size != 4096) continue;
        
        swr_convert(swrctx, pcm->data, pcm->nb_samples, indata, pcm->nb_samples);
        
        pcm->pts += dpts;
        
        dpts = av_rescale_q(pcm->nb_samples, {1, 44100}, aucodecctx->time_base);
        
        avcodec_send_frame(aucodecctx, pcm);
        
        avcodec_receive_packet(aucodecctx, &pkt);
        
        cout << pkt.size << endl;
        
        if(pkt.size <= 6)continue;
        
        pkt.pts = av_rescale_q(pkt.pts, aucodecctx->time_base, stream->time_base);
        
        pkt.dts = av_rescale_q(pkt.dts, aucodecctx->time_base, stream->time_base);
        
        pkt.duration = av_rescale_q(pkt.duration, aucodecctx->time_base, stream->time_base);
        
        int r = av_interleaved_write_frame(avfmctx, &pkt);
        
    }
    
    
    
}
