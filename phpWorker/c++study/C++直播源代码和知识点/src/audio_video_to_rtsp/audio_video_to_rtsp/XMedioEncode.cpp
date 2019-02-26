





//
//  XMedioEncode.cpp
//  rtsp_to_rtmp
//
//  Created by tangwei on 17/11/21.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include "XMedioEncode.hpp"

//class AVMedioEncode;

XMedioEncode * XMedioEncode::Get(){
    
    av_register_all();

    return new AVMedioEncode;

}

bool AVMedioEncode::initScale(){
    
    this->sws = sws_getCachedContext(this->sws, this->inWidth, this->inHeight, AV_PIX_FMT_BGR24, this->outWidth, this->outHeight, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    
    if(this->sws){
        
        yuv = av_frame_alloc();//分配yuv的空间
        
        yuv->format = AV_PIX_FMT_YUV420P;
        
        yuv->width = this->inWidth;
        
        yuv->height = this->inHeight;
        
        yuv->pts = 0;
        
        int re = av_frame_get_buffer(yuv, 32);//分配yuv的数据空间
        
        if(re != 0)return false;
        
        return true;
        
    }
    
    return false;
    
}

AVFrame *AVMedioEncode::RGBToYUV(char *rgb){
    
    uint8_t *indata[8] = {0};
    
    indata[0] = (uint8_t *)rgb;
    
    int insize[8] = {0};
    
    insize[0] = this->inWidth * this->PIXSize;
    
    int ret = sws_scale(this->sws, indata, insize, 0, this->inHeight, yuv->data, yuv->linesize);
    
    if(ret) return this->yuv;
    
    return NULL;
}



void AVMedioEncode::close(){
    
    if(this->sws) {
        
        sws_freeContext(this->sws);
        
        this->sws = NULL;
        
    }
    
    if(this->yuv){
        
        av_frame_free(&this->yuv);
        
        this->yuv = NULL;
        
    }
    
    if(this->avctx){
    
        avcodec_free_context(&this->avctx);
        
        this->avctx = NULL;
    }
    
    if(this->swr){
    
        swr_free(&this->swr);
        
        this->swr = NULL;
        
    }
    
    if(this->fltp){
    
        av_frame_free(&this->fltp);
        
        this->fltp = NULL;
        
    }
    
}

bool AVMedioEncode::initVideoCodec(){

    AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    
    if(!codec)return false;
    
    this->avctx = avcodec_alloc_context3(codec);
    
    if(this->avctx == NULL) return false;
    
    this->avctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;//全局参数
    
    this->avctx->codec_id = codec->id;//上面查找到的编码器的ID号
    
    this->avctx->thread_count = 8;//编码开启的线程数量
    
    this->avctx->bit_rate = 50*1024*8;//比特率，是压缩后每秒视频的位大小 我们在这儿设置为50KB
    
    this->avctx->width = this->outWidth;//视频的宽度，opencv打开的cam可以得到宽度
    
    this->avctx->height = this->outHeight;//视频的高度，opencv打开的cam可以得到高度
    
    this->avctx->framerate = {this->fps, 1};//设置帧率
    
    this->avctx->time_base = {1, 1000000};
    
    this->avctx->gop_size = 50;//画面组的大小，多少帧为一个关键帧，这值设置的越大，压缩率就越高，但是如果其中丢掉一帧，那么这个50帧的画面就GG了，这50个帧中，第一个一定是I帧，后面才是B帧和P帧。
    
    this->avctx->max_b_frames = 0;//B帧设置为0 这样上面设置的50帧中，就不会有B帧，都是I帧、P帧
    
    this->avctx->pix_fmt = AV_PIX_FMT_YUV420P;//设置像素格式
    
    int res = avcodec_open2(this->avctx, NULL, NULL);
    
    if(res != 0) return false;
    
    return true;
    
}

bool AVMedioEncode::initAudioCodec(){

    AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    
    this->auctx = avcodec_alloc_context3(codec);
    
    if(auctx == NULL)return false;
    
    this->auctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    
    this->auctx->thread_count = 8;
    
    this->auctx->bit_rate = 50 * 1024 * 8;//一秒钟的比特数
    
    this->auctx->sample_rate = this->sampleRate;//编码成什么样的样本率
    
    this->auctx->sample_fmt = AV_SAMPLE_FMT_FLTP;
    
    this->auctx->channels = this->channels;
    
    this->auctx->channel_layout = av_get_default_channel_layout(this->channels);
    
    this->auctx->time_base = {1, 1000000};
    
    int r = avcodec_open2(this->auctx, 0, 0);
    
    if(r == 0)return true;
    
    return false;
    
}

void AVMedioEncode::toH264(){

    avcodec_send_frame(this->avctx, this->yuv);
    
    av_packet_unref(&this->pkt);
    
    avcodec_receive_packet(this->avctx, &this->pkt);
    
}

void AVMedioEncode::toAac(){

    avcodec_send_frame(this->auctx, this->fltp);
    
    av_packet_unref(&this->aupkt);
    
    int res = avcodec_receive_packet(this->auctx, &this->aupkt);
    
    if(res != 0){
    
        std::cout << "xx" << std::endl;
        
    }
    
}

bool AVMedioEncode::initReSample(){
    
    this->swr = swr_alloc_set_opts(this->swr, av_get_default_channel_layout(this->channels), this->OutSampleFmt, this->sampleRate, av_get_default_channel_layout(this->channels), this->inSampleFmt, this->sampleRate, 0, 0);
    
    if(this->swr == NULL)return false;
    
    swr_init(this->swr);
    
    this->fltp = av_frame_alloc();
    
    if(this->fltp == NULL) return false;
    
    this->fltp->format = this->OutSampleFmt;
    
    this->fltp->channels = this->channels;
    
    this->fltp->channel_layout = av_get_default_channel_layout(this->channels);
    
    this->fltp->nb_samples = this->nb_samples;//一通道一帧的样本量
    
    this->fltp->pts = 0;
    
    int r = av_frame_get_buffer(this->fltp, 0);
    
    if(r == 0)return true;
    
    return false;
    
}

AVFrame *AVMedioEncode::toFltp(char *data){

    const uint8_t *indata[8] = {0};
    
    indata[0] = (uint8_t *)data;
    
    int r = swr_convert(this->swr, this->fltp->data, this->fltp->nb_samples, indata, this->fltp->nb_samples);
    
    if(r < 0)return NULL;
    
    return this->fltp;

}



