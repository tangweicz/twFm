//
//  XRtmp.cpp
//  rtsp_to_rtmp
//
//  Created by tangwei on 17/11/23.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include "XRtmp.hpp"

XRtmp * XRtmp::Get(){
    
    avcodec_register_all();
    
    avformat_network_init();

    return new CXRtmp;
    
}

void CXRtmp::close(){

    if(this->avfctx){
    
        avformat_free_context(this->avfctx);
        
        this->avfctx = NULL;
        
    }
    
}


bool CXRtmp::initForamt(){
    
    int res = avformat_alloc_output_context2(&this->avfctx, NULL, "flv", this->url.c_str());
    
    if(res < 0)return false;

    return true;
}

int CXRtmp::addStream(AVCodecContext *avctx){
    
    this->newStream = avformat_new_stream(this->avfctx, avctx->codec);
    
    if(this->newStream == NULL)return -1;
    
    this->newStream->codecpar->codec_tag = 0;
    
    int res = avcodec_parameters_from_context(this->newStream->codecpar, avctx);
    
    if(res < 0)return -1;
    
    av_dump_format(this->avfctx, 0, this->url.c_str(), 1);
    
    if(avctx->codec_type == AVMEDIA_TYPE_VIDEO){
    
        this->vctx = avctx;
        
        this->vs = this->newStream;
        
    }
    
    if(avctx->codec_type == AVMEDIA_TYPE_AUDIO){
    
        
        this->actx = avctx;
        
        this->as = this->newStream;
    }
    
    this->rtmpcodec = avctx;
    
    return newStream->index;
}

bool CXRtmp::sendHeader(){

    int res = avio_open(&this->avfctx->pb, this->url.c_str(), AVIO_FLAG_WRITE);
    
    if(res < 0)return false;
    
    int re = avformat_write_header(this->avfctx, NULL);
    
    if(re < 0) return false;
    
    return true;
    
}

int CXRtmp::writeFrame(AVPacket *pkt){
    
    AVRational stime;
    
    AVRational dtime;
    
    if(this->vctx != NULL && this->vs != NULL && pkt->stream_index == this->vs->index){
    
        stime = this->vctx->time_base;
        
        dtime = this->vs->time_base;
        
    }
    
    if(this->actx != NULL && this->as != NULL && pkt->stream_index == this->as->index){
    
        stime = this->actx->time_base;
        
        dtime = this->as->time_base;
        
    }
    
    pkt->pts = av_rescale_q(pkt->pts, stime, dtime);
    
    pkt->dts = av_rescale_q(pkt->dts, stime, dtime);
    
    pkt->duration = av_rescale_q(pkt->duration, stime, dtime);

    av_interleaved_write_frame(this->avfctx, pkt);

    return 0;
}
