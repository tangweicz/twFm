//
//  main.cpp
//  rtspToRtmp
//
//  Created by tangwei on 17/11/17.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include <iostream>

#include <opencv2/core.hpp>

#include <opencv2/highgui.hpp>

extern "C"{

#include <libswscale/swscale.h>
    
#include <libavformat/avformat.h>
    
#include <libavformat/avio.h>
    
#include <libavutil/frame.h>
    
#include <libavcodec/avcodec.h>
    

    
}

using namespace cv;//引用opencv的命名空间

using namespace std;//引用输入输出的命名空间

int main(int argc, const char * argv[]) {
    
    av_register_all();
    
    avformat_network_init();
    
    VideoCapture camera;//初始化摄像头类
    
    Mat img;//初始化画布类
    
    namedWindow("window");//创建名为window的窗口
    
    if(!camera.open(0)){//打开摄像头设备，如果打开失败，就直接结束掉
    
        cout << "open camera failed" << endl;//打开失败给出错误提示
        
        return -1;
        
    }
    
    SwsContext *sws = NULL;//创建像素格式转换上下文
    
    int cameraWidth = camera.get(CV_CAP_PROP_FRAME_WIDTH);
    
    int cameraHeight = camera.get(CV_CAP_PROP_FRAME_HEIGHT);
    
    sws = sws_getCachedContext(sws, cameraWidth, cameraHeight, AV_PIX_FMT_BGR24, cameraWidth, cameraHeight, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);//初始化像素转换上下文
    
    AVFrame *yuv = av_frame_alloc();
    
    yuv->format = AV_PIX_FMT_YUV420P;
    
    yuv->width = cameraWidth;
    
    yuv->height = cameraHeight;
    
    yuv->pts = 0;//这儿先设置为0，一会拿到它进行h264编码的时候 需要递增
    
    av_frame_get_buffer(yuv, 0);
    
    AVCodec *avcodec = avcodec_find_encoder(AV_CODEC_ID_H264);
    
    AVCodecContext *avctx = avcodec_alloc_context3(avcodec);
    
    avctx->flags = AV_CODEC_FLAG_GLOBAL_HEADER;//全局参数
    
    avctx->codec_id = avcodec->id;//上面查找到的编码器的ID号
    
    avctx->thread_count = 8;//编码开启的线程数量
    
    avctx->bit_rate = 50*1024*8;//比特率，是压缩后每秒视频的位大小 我们在这儿设置为50KB
    
    avctx->width = cameraWidth;//视频的宽度，opencv打开的cam可以得到宽度
    
    avctx->height = cameraHeight;//视频的高度，opencv打开的cam可以得到高度
    
    avctx->time_base = {1, static_cast<int>(camera.get(CAP_PROP_FPS))};//设置时间基
    
    avctx->framerate = {static_cast<int>(camera.get(CAP_PROP_FPS)), 1};//设置帧率
    
    avctx->gop_size = 50;//画面组的大小，多少帧为一个关键帧，这值设置的越大，压缩率就越高，但是如果其中丢掉一帧，那么这个50帧的画面就GG了，这50个帧中，第一个一定是I帧，后面才是B帧和P帧。
    
    avctx->max_b_frames = 0;//B帧设置为0 这样上面设置的50帧中，就不会有B帧，都是I帧、P帧
    
    avctx->pix_fmt = AV_PIX_FMT_YUV420P;//设置像素格式
    
    avcodec_open2(avctx, 0, 0);
    
    AVPacket pkt;
    
    memset(&pkt, 0, sizeof(pkt));
    
    AVFormatContext *avfc = NULL;
    
    int r = avformat_alloc_output_context2(&avfc, 0, "flv", "rtmp://120.55.186.199/live");
    
    if(r < 0){
    
        char buff[1024] = {0};
        
        av_strerror(r, buff, sizeof(buff));
        
        return -1;
        
    }
    
    AVStream *avstream = avformat_new_stream(avfc, avcodec);
    
    int m = avcodec_parameters_from_context(avstream->codecpar, avctx);
    
    if(m < 0){
        
        char buff[1024] = {0};
        
        av_strerror(m, buff, sizeof(buff));
        
        return -1;
        
    }
    
    int s = avio_open(&avfc->pb, "rtmp://120.55.186.199/live", AVIO_FLAG_WRITE);
    
    if(s < 0){
        
        char buff[1024] = {0};
        
        av_strerror(s, buff, sizeof(buff));
        
        return -1;
        
    }
    
    avformat_write_header(avfc, 0);
    
    while(true){
    
        camera.read(img);//读取摄像头的每一帧数据，放入到img的画布中
        
        const uint8_t *indata[8] = {0};
        
        indata[0] = img.data;
        
        int insize[8] = {0};
        
        insize[0] = img.cols * img.elemSize();
        
        int dstHeight = sws_scale(sws, indata, insize, 0, img.rows, yuv->data, yuv->linesize);//像素数据转换
        
        cout << dstHeight << endl;
        
        yuv->pts ++;
        
        avcodec_send_frame(avctx, yuv);
        
        avcodec_receive_packet(avctx, &pkt);
        
        pkt.pts = av_rescale_q(pkt.pts, avctx->time_base, avstream->time_base);
        
        pkt.dts = av_rescale_q(pkt.dts, avctx->time_base, avstream->time_base);
        
        av_interleaved_write_frame(avfc, &pkt);
        
    }
    
}
