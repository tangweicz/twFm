//
//  main.cpp
//  rtsp_to_rtmp
//
//  Created by tangwei on 17/10/30.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include <iostream>

#include <string>

#include "opencv2/core.hpp"

#include "opencv2/videoio.hpp"

#include "opencv2/highgui.hpp"

#include "XMedioEncode.hpp"

#include "XRtmp.hpp"

using namespace cv;

int main(int argc, const char * argv[]) {
    
    cv::VideoCapture camera;
    
    cv::Mat img;
    
    camera.open(0);
    
    if(!camera.isOpened()) return -1;
    
    XMedioEncode *xmedio = XMedioEncode::Get();
    
    XRtmp *xrtmp = XRtmp::Get();
    
    xrtmp->url = "rtmp://120.55.186.199/live";
    
    xmedio->PIXSize = 3;
    
    xmedio->inHeight = camera.get(CAP_PROP_FRAME_HEIGHT);
    
    xmedio->inWidth = camera.get(CAP_PROP_FRAME_WIDTH);
    
    xmedio->outHeight = camera.get(CAP_PROP_FRAME_HEIGHT);
    
    xmedio->outWidth = camera.get(CAP_PROP_FRAME_WIDTH);
    
    bool initres = xmedio->initScale();
    
    if(!initres)xmedio->close();
    
    bool res = xmedio->initVideoCodec();
    
    if(!res) xmedio->close();
    
    xrtmp->initForamt();
    
    xrtmp->addStream(xmedio->avctx);
    
    xrtmp->sendHeader();
    
    while(true){
        
        camera.read(img);
        
        xmedio->RGBToYUV((char *)img.data);
        
        if(xmedio->yuv == NULL) xmedio->close();
        
        xmedio->yuv->pts++;
        
        xmedio->toH264();
        
        std::cout << xmedio->pkt.size << std::endl;
        
        xrtmp->writeFrame(&xmedio->pkt, 0);
        
    }
    return 0;
}
