//
//  XVideoRecord.hpp
//  audio_video_to_rtsp
//
//  Created by tangwei on 17/12/4.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#ifndef XVideoRecord_hpp
#define XVideoRecord_hpp

#include <stdio.h>

#include <iostream>

#include "opencv2/core.hpp"

#include "opencv2/videoio.hpp"

#include "opencv2/highgui.hpp"

#include <QtCore/QThread>

#include <list>

#include "QtCore/QMutex"

#include "xdata.hpp"

#endif /* XVideoRecord_hpp */

using namespace cv;

using namespace std;

class XVideoRecord:public QThread{
    
public:
    static XVideoRecord *Get();
    
    virtual bool init(int cameraIndex) = 0;
    
    virtual bool init(const char *url) = 0;
    
    virtual void stop() = 0;
    
    virtual XDATA Pop() = 0;
    
    virtual bool start() = 0;
    
    list<XDATA> datas;
    
    int maxsize = 100;
    
    int frameWidth;
    
    int frameHeight;
    
    int fps = 0;
    
    bool isStart = false;
    
    Mutex mutex;
    
    VideoCapture camera;
};

class CXVideoRecord:public XVideoRecord{
    
public:
    
    bool init(int cameraIndex);
    
    bool init(const char *url);
    
    void stop();
    
    XDATA Pop();
    
    void run();
    
    bool start();
    
    
    
};

