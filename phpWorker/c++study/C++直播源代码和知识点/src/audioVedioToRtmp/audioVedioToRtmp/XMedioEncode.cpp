
//
//  XMedioEncode.cpp
//  audioVedioToRtmp
//
//  Created by tangwei on 18/1/3.
//  Copyright © 2018年 tangwei. All rights reserved.
//


#include "XMedioEncode.hpp"

XMedioEncode *XMedioEncode::Get(){
    
    avcodec_register_all();

    return new XMedioEncode();
    
}

bool XMedioEncode::initReSample(){

    this->swrCtx = swr_alloc_set_opts(this->swrCtx, av_get_default_channel_layout(2), AV_SAMPLE_FMT_FLTP, 44100, av_get_default_channel_layout(2), AV_SAMPLE_FMT_U8, 44100, 0, 0);
    
    if(this->swrCtx == NULL){
    
        std::cout << "audio alloc swrContext failed" << std::endl;
        
        return false;
        
    }
    
    swr_init(this->swrCtx);
    
    this->audioFrame = av_frame_alloc();
    
    if(this->audioFrame == NULL){
    
        std::cout << "audio av_frame_alloc failed" << std::endl;
        
        return false;
        
    }
    
    this->audioFrame->format = AV_SAMPLE_FMT_FLTP;
    
    this->audioFrame->channels = 2;
    
    this->audioFrame->channel_layout = av_get_default_channel_layout(2);
    
    this->audioFrame->nb_samples = 1024;
    
    this->audioFrame->pts = 0;
    
    int res = av_frame_get_buffer(this->audioFrame, 0);
    
    if(res < 0){
    
        std::cout << "audio frame_get_buffer failed" << std::endl;
        
        return false;
        
    }
    
    return true;
     
     
}

bool XMedioEncode::toFltp(char *audioData){
    
    const uint8_t *indata[8] = {0};
    
    indata[0] = (uint8_t *)audioData;
    
    int res = swr_convert(this->swrCtx, this->audioFrame->data, this->audioFrame->nb_samples, indata, this->audioFrame->nb_samples);
    
    if(res < 0){
    
        std::cout << "audio swr convert failed" << std::endl;
        
        return false;
    }
    
    return true;
    
}



bool XMedioEncode::initAudioCodec(){

    AVCodec *audioCodec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    
    if(audioCodec == NULL){
    
        std::cout << "avcodec_find_encoder failed" << std::endl;
        
        return false;
    }
    
    this->audioCodecCtx = avcodec_alloc_context3(audioCodec);
    
    if(this->audioCodecCtx == NULL){
    
        std::cout << "avcodec_alloc_context3 failed" << std::endl;
        
        return false;
        
    }
    
    this->audioCodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    
    this->audioCodecCtx->thread_count = 8;
    
    this->audioCodecCtx->bit_rate = 409600;
    
    this->audioCodecCtx->sample_rate = 44100;
    
    this->audioCodecCtx->sample_fmt = AV_SAMPLE_FMT_FLTP;
    
    this->audioCodecCtx->channels = 2;
    
    this->audioCodecCtx->channel_layout = av_get_default_channel_layout(2);
    
    this->audioCodecCtx->time_base = {1, 1000000};
    
    int res = avcodec_open2(this->audioCodecCtx, audioCodec, 0);
    
    if(res < 0){
    
        std::cout << "avcodec_open2 failed" << std::endl;
        
        return false;
    }
    
    
    return true;
    
}

void XMedioEncode::toAAC(){
    
    int res;
    
    res = avcodec_send_frame(this->audioCodecCtx, this->audioFrame);
    
    if(res != 0){
    
        std::cout << "avcodec_send_frame failed" << std::endl;
        
    }
    
    memset(&this->audioPkt, 0, sizeof(this->audioPkt));
    
    av_packet_unref(&this->audioPkt);//每次循环回来都需要对pakcet进行内存释放，否则会导致内存泄漏
    
    avcodec_receive_packet(this->audioCodecCtx, &this->audioPkt);
    
}

bool XMedioEncode::initSwsScale(){
    
    if(this->vedioHeight == 0 || this->vedioWidth == 0){
    
        std::cout << "you need set the frame width and height first" << std::endl;
        
        return false;
        
    }

    this->swsCtx = sws_getCachedContext(this->swsCtx, this->vedioWidth, this->vedioHeight, AV_PIX_FMT_BGR24, this->vedioWidth, this->vedioHeight, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    
    if(this->swsCtx == NULL){
    
        std::cout << "sws_getCachedContext failed" << std::endl;
        
        return false;
    }
    
    this->vedioFrame = av_frame_alloc();
    
    if(this->vedioFrame == NULL){
    
        std::cout << "av_frame_alloc failed" <<std::endl;
        
        return false;
        
    }
    
    this->vedioFrame->format = AV_PIX_FMT_YUV420P;
    
    this->vedioFrame->width = this->vedioWidth;
    
    this->vedioFrame->height = this->vedioHeight;
    
    this->vedioFrame->pts = 0;
    
    int res = av_frame_get_buffer(this->vedioFrame, 32);
    
    if(res < 0){
    
        std::cout << "av_frame_get_buffer failed" << std::endl;
        
        return false;
        
    }
    
    return true;
}


bool XMedioEncode::toYuv(char *vedioData){
    
    if(this->colsNum ==0 || this->elemSize == 0){
    
        std::cout << "you need set the img cols and elemSize first" << std::endl;
        
        return false;
        
    }

    uint8_t *indata[8] = {0};
    
    indata[0] = (uint8_t *)vedioData;
    
    int insize[8] = {0};
    
    insize[0] = this->colsNum * this->elemSize;
    
    sws_scale(this->swsCtx, indata, insize, 0, this->vedioHeight, vedioFrame->data, vedioFrame->linesize);
    
    return true;
    
}

bool XMedioEncode::initVedioCodec(){
    
    AVCodec *vedioCodec = avcodec_find_encoder(AV_CODEC_ID_H264);

    this->vedioCodecCtx = avcodec_alloc_context3(vedioCodec);
    
    if(this->vedioCodecCtx == NULL){
    
        std::cout << "avcodec_alloc_context3 failed" << std::endl;
        
        return false;
        
    }
    
    this->vedioCodecCtx->flags = AV_CODEC_FLAG_GLOBAL_HEADER;//全局参数
    
    this->vedioCodecCtx->codec_id = vedioCodec->id;//上面查找到的编码器的ID号
    
    this->vedioCodecCtx->thread_count = 8;//编码开启的线程数量
    
    this->vedioCodecCtx->bit_rate = 50*1024*8;//比特率，是压缩后每秒视频的位大小 我们在这儿设置为50KB
    
    this->vedioCodecCtx->width = this->vedioWidth;//视频的宽度，opencv打开的cam可以得到宽度
    
    this->vedioCodecCtx->height = this->vedioHeight;//视频的高度，opencv打开的cam可以得到高度
    
    this->vedioCodecCtx->time_base = {1, 1000000};//设置时间基
    
    this->vedioCodecCtx->framerate = {25, 1};//设置帧率
    
    this->vedioCodecCtx->gop_size = 50;//画面组的大小，多少帧为一个关键帧，这值设置的越大，压缩率就越高，但是如果其中丢掉一帧，那么这个50帧的画面就GG了，这50个帧中，第一个一定是I帧，后面才是B帧和P帧。
    
    this->vedioCodecCtx->max_b_frames = 0;//B帧设置为0 这样上面设置的50帧中，就不会有B帧，都是I帧、P帧
    
    this->vedioCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;//设置像素格式
    
    int res = avcodec_open2(this->vedioCodecCtx, NULL, 0);
    
    if(res < 0){
    
        std::cout << "video avcodec_open2 failed, error code:" << res << std::endl;
        
        return false;
        
    }
    
    return true;
    
}

void XMedioEncode::toH264(){

    int res = avcodec_send_frame(this->vedioCodecCtx, this->vedioFrame);
    
    if(res < 0){
        
        std::cout << "avcodec_send_frame failed" << std::endl;
        
    }
    
    memset(&this->vedioPkt, 0, sizeof(this->vedioPkt));
    
    av_packet_unref(&this->vedioPkt);//每次循环回来都需要对pakcet进行内存释放，否则会导致内存泄漏
    
    avcodec_receive_packet(this->vedioCodecCtx, &this->vedioPkt);
    
}






