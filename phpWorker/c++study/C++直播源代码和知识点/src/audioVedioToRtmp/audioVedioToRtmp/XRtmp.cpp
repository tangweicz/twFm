//
//  XRtmp.cpp
//  audioVedioToRtmp
//
//  Created by tangwei on 18/1/9.
//  Copyright © 2018年 tangwei. All rights reserved.
//

#include "XRtmp.hpp"


XRtmp *XRtmp::Get(){

    av_register_all();//初始化所有的封装器
    
    avformat_network_init();;//注册所有的网络协议
    
    return new XRtmp();
    
}

bool XRtmp::initAvFormat(){

    int res = avformat_alloc_output_context2(&this->avFormatCtx, NULL, "flv", this->url);
    
    if(res < 0){
    
        std::cout << "avformat_alloc_output_context2 failed" << std::endl;
        
        return false;
        
    }
    
    if(this->avFormatCtx == NULL){
    
        std::cout << "avformat_alloc_output_context2 failed" << std::endl;
        
        return false;
        
    }
    
    return true;
    
}

int XRtmp::addStream(AVCodecContext *codecCtx){

    if(codecCtx->codec_type == AVMEDIA_TYPE_AUDIO){//如果是音频的编码器的话，证明这儿是加入的音频的流，对应做相应的处理即可
    
        this->audioStream = avformat_new_stream(this->avFormatCtx, codecCtx->codec);
        
        if(this->audioStream == NULL){
        
            std::cout << "audio avformat_new_stream failed" << std::endl;
            
            return -1;
            
        }
        
        this->audioStream->codecpar->codec_tag = 0;
        
        int res = avcodec_parameters_from_context(this->audioStream->codecpar, codecCtx);
        
        if(res < 0){
        
            std::cout << "avcodec_parameters_from_context failed" << std::endl;
            
            return -1;
            
        }
        
        this->audioCtx = codecCtx;
        
        av_dump_format(this->avFormatCtx, 0, this->url, 1);
        
        return this->audioStream->index;
        
    }
    
    if(codecCtx->codec_type == AVMEDIA_TYPE_VIDEO){//如果是视频的编码器的话，证明这儿是加入的视频的流，对应做相应的处理即可
    
        this->vedioStream = avformat_new_stream(this->avFormatCtx, codecCtx->codec);
        
        if(this->vedioStream == NULL){
            
            std::cout << "vedio avformat_new_stream failed" << std::endl;
            
            return -1;
            
        }
        
        this->vedioStream->codecpar->codec_tag = 0;
        
        int res = avcodec_parameters_from_context(this->vedioStream->codecpar, codecCtx);
        
        if(res < 0){
            
            std::cout << "vedio avcodec_parameters_from_context failed" << std::endl;
            
            return -1;
            
        }
        
        this->vedioCtx = codecCtx;
        
        av_dump_format(this->avFormatCtx, 0, this->url, 1);
        
        return this->vedioStream->index;
        
    }
    
    return 0;
}

bool XRtmp::openIOWriteHeader(){
    
    int res;

    res = avio_open(&this->avFormatCtx->pb, this->url, AVIO_FLAG_WRITE);
    
    if(res < 0){
    
        std::cout << "avio_open failed" << std::endl;
        
        return false;
        
    }
    
    res = avformat_write_header(this->avFormatCtx, NULL);
    
    if(res < 0){
    
        std::cout << "avformat_write_header failed" << std::endl;
        
        return false;
        
    }

    return true;
}


int XRtmp::pushFrame(AVPacket pkt, int streamIndex){

    if(pkt.size <= 0 || streamIndex < 0)return -1;
    
    pkt.stream_index = streamIndex;
    
    if(this->audioStream != NULL && this->audioCtx != NULL && this->audioStream->index == pkt.stream_index){//如果是音频
    
        pkt.pts = av_rescale_q(pkt.pts, this->audioCtx->time_base, this->audioStream->time_base);
        
        pkt.dts = av_rescale_q(pkt.dts, this->audioCtx->time_base, this->audioStream->time_base);
        
        pkt.duration = av_rescale_q(pkt.duration, this->audioCtx->time_base, this->audioStream->time_base);
        
    }
    
    if(this->vedioStream != NULL && this->vedioCtx != NULL && this->vedioStream->index == pkt.stream_index){//如果是视频
    
        pkt.pts = av_rescale_q(pkt.pts, this->vedioCtx->time_base, this->vedioStream->time_base);
        
        pkt.dts = av_rescale_q(pkt.dts, this->vedioCtx->time_base, this->vedioStream->time_base);
        
        pkt.duration = av_rescale_q(pkt.duration, this->vedioCtx->time_base, this->vedioStream->time_base);
        
    }
    
    int res = av_interleaved_write_frame(this->avFormatCtx, &pkt);
    
    if(res < 0){
    
        return -1;
        
    }
    
    return 0;
    
}














