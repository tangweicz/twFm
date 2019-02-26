



//
//  XAudioRecord.hpp
//  audio_video_to_rtsp
//
//  Created by tangwei on 17/11/29.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#ifndef XAudioRecord_hpp
#define XAudioRecord_hpp

#include <stdio.h>

#include "QtCore/QCoreApplication"

#include "QtCore/QThread"

#include "QtMultimedia/QAudioFormat"

#include "QtMultimedia/QAudioInput"

#include "QtMultimedia/QAudioOutput"

#include "QtCore/QMutex"

#include <list>

#include <iostream>

#include <string>

#include "xdata.hpp"

#include <fstream>

using namespace std;


#endif /* XAudioRecord_hpp */

enum XAUDIOTYPE{

    X_AUDIO_QT5
    
};

class XAudioRecord:public QThread{

public:
    
    int channels = 2;
    
    int sampleRate = 44100;
    
    int sampleByte = 1;
    
    int nbSamples = 1024;
    
    static XAudioRecord *Get(XAUDIOTYPE type);
    
    virtual bool start() = 0;//开始录制
    
    virtual void stop() = 0;//停止录制
    
    virtual XDATA Pop() = 0;
    
    list<XDATA> datas;
    
    QMutex mutex;
    
    int maxlist = 100;
    
};


class CXAudioRecord :public XAudioRecord{

public:
    
    
    
    bool start();//开始录制
    
    void stop();//停止录制
    
    void run();//线程需要使用的
    
    bool isStart = false;
    
    XDATA Pop();
    
private:
    
    QAudioInput *input = NULL;
    
    QIODevice *io = NULL;
};
