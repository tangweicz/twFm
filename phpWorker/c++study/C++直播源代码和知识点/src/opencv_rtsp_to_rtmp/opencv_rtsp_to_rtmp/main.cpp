//
//  main.cpp
//  videoCamera
//
//  Created by tangwei on 17/10/20.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include <iostream>

#include "opencv2/highgui.hpp"

#include "opencv2/videoio.hpp"

#include "opencv2/core.hpp"//Mat

#include "opencv2/imgcodecs.hpp"//imread

#include "opencv2/videoio.hpp"

extern "C" {

#include "libswscale/swscale.h"
    
#include "libavcodec/avcodec.h"
    
#include "libavutil/frame.h"
    
#include "libavformat/avformat.h"
    
}

#include <string>


#pragma comment("lib", "swscale")

int main(int argc, const char * argv[]) {//opencv采集的rtsp流，音频默认就被抛弃掉了。opencv处理摄像头，usb的摄像机，调用的是系统的驱动。而rtsp的相机，调用的是ffmpeg的接口（等同于用ffmpeg打开rtsp的摄像头）。那么opencv调用rtsp的摄像头，除了打开rtsp摄像头以外，还对视频做了YUV、解码的工作（即给我们的是RGB的数据）。所以，我们拿到opencv采集的数据之后，通常需要处理，处理流程为：1、opencv采集rtsp解码 2、ffmpeg缩放转像素格式 3、ffmpeg编码h264 4、ffmpeg推流rtsp
    
    avcodec_register_all();//注册所有的视频的编解码器
    
    av_register_all();
    
    avformat_network_init();
    
    cv::VideoCapture cam;
    
    //std::string url = "rtsp://admin:@192.168.6.116";//"rtsp://用户名:密码@相机地址:端口号"打开rtsp的相机
    
    cv::namedWindow("imageWindow");//实际项目中不用这个，只是测试采用这个
    
    SwsContext *vsl = NULL;//像素格式转换、图像大小的上下文，一定要赋值NULL
    
    try{
        
        cam.open(0);
        
        if(cam.isOpened()){
        
            std::cout << "open carema success" << std::endl;
            
            cv::Mat img;
            
            vsl = sws_getCachedContext(vsl, cam.get(CV_CAP_PROP_FRAME_WIDTH), cam.get(CV_CAP_PROP_FRAME_HEIGHT), AV_PIX_FMT_BGR24, cam.get(CV_CAP_PROP_FRAME_WIDTH), cam.get(CV_CAP_PROP_FRAME_HEIGHT), AV_PIX_FMT_YUV420P, SWS_BICUBIC, 0, 0, 0);// sws_getCachedContext();//参数列表 1、struct SwsContext *cntext 转换前的资源上下文 2、int srcW 原宽度 3、int srcH 原高度 4、enum AVPixelFormat srcFormat 原格式 5、int dstW 目标宽度 6、int dstH 目标高度 7、enum AVPixelFormat dstFormat 目标格式 8、int flags 指算法，尺寸变化才使用算法 SWS_BICUBIC，如果不变可以写0 9、srcFilter 暂时不用 10、dstFilter 暂时不同 11、param暂时不用  #include "libswscale/swscale.h"
            
            AVFrame *yuv = NULL;// #include "libavcodec/avcodec.h"
            
            AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_H264);
            
            AVCodecContext *cv = NULL;
            
            cv = avcodec_alloc_context3(codec);
            
            cv->flags = AV_CODEC_FLAG_GLOBAL_HEADER;//全局参数
            
            cv->codec_id = codec->id;//上面查找到的编码器的ID号
            
            cv->thread_count = 8;//编码开启的线程数量
            
            cv->bit_rate = 50*1024*8;//比特率，是压缩后每秒视频的位大小 我们在这儿设置为50KB
            
            cv->width = cam.get(CV_CAP_PROP_FRAME_WIDTH);//视频的宽度，opencv打开的cam可以得到宽度
            
            cv->height = cam.get(CV_CAP_PROP_FRAME_HEIGHT);//视频的高度，opencv打开的cam可以得到高度
            
            cv->time_base = {1, static_cast<int>(cam.get(CV_CAP_PROP_FPS))};//设置时间基
            
            cv->framerate = {static_cast<int>(cam.get(CV_CAP_PROP_FPS)), 1};//设置帧率
            
            cv->gop_size = 50;//画面组的大小，多少帧为一个关键帧，这值设置的越大，压缩率就越高，但是如果其中丢掉一帧，那么这个50帧的画面就GG了，这50个帧中，第一个一定是I帧，后面才是B帧和P帧。
            
            cv->max_b_frames = 0;//B帧设置为0 这样上面设置的50帧中，就不会有B帧，都是I帧、P帧
            
            cv->pix_fmt = AV_PIX_FMT_YUV420P;//设置像素格式
            
            avcodec_open2(cv, 0, 0);
            
            int vpts = 0;
            
            AVPacket packet;
            
            memset(&packet, 0, sizeof(packet));
            
            AVFormatContext *octx;
            
            avformat_alloc_output_context2(&octx, 0, "flv", "rtmp://192.168.6.109/live");
            
            AVStream *out = avformat_new_stream(octx, NULL);
            
            out->codecpar->codec_tag = 0;
            
            avcodec_parameters_from_context(out->codecpar, cv);
            
            avio_open(&octx->pb, "rtmp://192.168.6.109/live", AVIO_FLAG_WRITE);
            
            avformat_write_header(octx, 0);
            
            while(true){
                
                cam.read(img);//如果用read，在opencv中，采集rtsp的数据和解码成RGB都已经做了。 grab采集视频并调用ffmpeg接口解码（解码是必须的，中途不能扔数据，因为h264是连续的，丢任何一帧都会导致数据错误） retrieve尺寸转换,就是对应的YUV转成RGB，这步可以跳着做。
                
                //if(cam.grab()){//拿到一帧数据、调用ffmpeg接口解码视频帧
                
                //    continue;
                    
                //}
                
                //if(!cam.retrieve(img)){//拿到解码之后的视频帧
                
                //    continue;
                    
                //}
                
                //cv::imshow("imageWindow", img);
                
                //cv::waitKey(1);//在imshow之后如果没有waitKey语句则不会正常显示图像 参数为0则会一直等待下去，不停止， 非0则会等待对应的毫秒数，然后返回按键值，如果没有按键则返回-1
                
                
                //开始RGB转换到YUV的过程开始，RGB与YUV的关系和转换方式，参考https://wenku.baidu.com/view/b8ec93aa27d3240c8547ef6c.html
                
                yuv = av_frame_alloc();//分配yuv的空间
                
                yuv->format = AV_PIX_FMT_YUV420P;
                
                yuv->width = cam.get(CV_CAP_PROP_FRAME_WIDTH);
                
                yuv->height = cam.get(CV_CAP_PROP_FRAME_HEIGHT);
                
                yuv->pts = 0;
                
                int ret = av_frame_get_buffer(yuv, 32);//分配yuv的数据空间
                
                if(ret != 0){
                
                    char str[1024] = {0};
                    
                    av_strerror(ret, str, sizeof(str)-1);
                    
                    return -2;
                    
                }
                
                
                uint8_t *indata[AV_NUM_DATA_POINTERS] = {0};
                
                indata[0] = img.data;
                
                int insize[AV_NUM_DATA_POINTERS] = {0};
                
                insize[0] = img.cols*img.elemSize();
                
                int hei = sws_scale(vsl, indata, insize, 0, img.rows, yuv->data, yuv->linesize);//sws_scale();对每一帧数据进行转换//参数列表 1、sws的上下文的指针，即上面的vsl 2、srcSlice 这个值是一个数组，该函数，之用这个数组的第一个下标，因为RGB的数据是存放在一起的，即该数组的下标0下的数据在RGB的时候为：BGRBGRBGRBGRBGR.....这样的，而如果是YUV的数据呢，该数租的下标0位Y，是亮度 1位U，是色差，2位V，是图像。3、srcStride，与srcSlice成对出现，也是一个数组，一般只用下标为0的数据，是一行是多少字节，因为，在srcSlice中，RGB的数据全部存在0的下标之下，那么具体怎么排序，该图像是多少行多少列？就需要根据srcSlice与srcStride换算得到。4、srcSliceY 一般不用，表示从哪个位置开始计算。5、srcSliceH 原视频的高度 6、dst 一个数组，与srcSlice一样 7、dstStride与srcStride一样。             该函数转码成功最终返回出该图像的高度。
                
                if(hei <= 0){
                
                    continue;
                    
                }
                
                yuv->pts = vpts;
                
                vpts++;
               
                avcodec_send_frame(cv, yuv);
                
                avcodec_receive_packet(cv, &packet);
                
                std::cout << packet.size << std::endl;
                
                packet.pts = av_rescale_q(packet.pts, cv->time_base, out->time_base);
                
                packet.dts = av_rescale_q(packet.dts, cv->time_base, out->time_base);
                
                av_interleaved_write_frame(octx, &packet);
            }
            
        }else{
        
            std::cout << "open carema failed" << std::endl;
            
        }
    }catch(std::exception &ex){
        
        if(cam.isOpened())cam.release();
        
        if(vsl != NULL) {
         
            sws_freeContext(vsl);
            
            vsl = NULL;
            
        }
    
        std::cerr << ex.what() << std::endl;
        
    }
    
    
    
    return 0;
}
