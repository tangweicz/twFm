//
//  XVedioRecord.hpp
//  audioVedioToRtmp
//
//  Created by tangwei on 18/1/10.
//  Copyright © 2018年 tangwei. All rights reserved.
//

#ifndef XVedioRecord_hpp
#define XVedioRecord_hpp

#include <stdio.h>

#include "opencv2/core.hpp"

#include "opencv2/highgui.hpp"

#include "opencv2/videoio.hpp"

#include "QtCore/QThread"

#include "XData.hpp"

#include <list>

#include <iostream>

class XVedioRecord:public QThread{
    
public:
    static XVedioRecord *Get();
    
    bool start();
    
    void run();
    
    std::list<XData> datas;
    
    bool isStart = false;
    
    XData Pop();
    
    int frameHeight = 0;
    
    int frameWidth = 0;
    
    int colsNum = 0;
    
    int elemSize = 0;
    
private:
    
    cv::VideoCapture camera;
    
    int maxLength = 100;
    
};

#endif /* XVedioRecord_hpp */
