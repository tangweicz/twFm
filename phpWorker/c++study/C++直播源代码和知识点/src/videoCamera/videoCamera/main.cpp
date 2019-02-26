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

#include <string>

int main(int argc, const char * argv[]) {//opencv采集的rtsp流，音频默认就被抛弃掉了。opencv处理摄像头，usb的摄像机，调用的是系统的驱动。而rtsp的相机，调用的是ffmpeg的接口（等同于用ffmpeg打开rtsp的摄像头）。那么opencv调用rtsp的摄像头，除了打开rtsp摄像头以外，还对视频做了YUV、解码的工作（即给我们的是RGB的数据）。所以，我们拿到opencv采集的数据之后，通常需要处理，处理流程为：1、opencv采集rtsp解码 2、ffmpeg缩放转像素格式 3、ffmpeg编码h264 4、ffmpeg推流rtsp
    
    cv::VideoCapture cam;
    
    //std::string url = "rtsp://admin:@192.168.6.116";//"rtsp://用户名:密码@相机地址:端口号"打开rtsp的相机
    
    cv::Mat img;
    
    cv::namedWindow("imageWindow");//实际项目中不用这个，只是测试采用这个

    if(cam.open(0)){//cam.open(url) 这个是打开rtsp的摄像头 cam.open(0)打开本地摄像头
    
        std::cout << "open camera success" << std::endl;
        
    }else{
    
        std::cout << "open carema faild" << std::endl;
        
        return -1;
        
    }
    
    while(true){
    
        cam.read(img);//如果用read，在opencv中，采集rtsp的数据和解码成RGB都已经做了。 grab采集视频并调用ffmpeg接口解码（解码是必须的，中途不能扔数据，因为h264是连续的，丢任何一帧都会导致数据错误） retrieve尺寸转换,就是对应的YUV转成RGB，这步可以跳着做。
        
        cv::imshow("imageWindow", img);
        
        cv::waitKey(1);//在imshow之后如果没有waitKey语句则不会正常显示图像 参数为0则会一直等待下去，不停止， 非0则会等待对应的毫秒数，然后返回按键值，如果没有按键则返回-1
        
    }
    
    return 0;
    
    
}
