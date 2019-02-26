//
//  XAudioRecord.hpp
//  audioVedioToRtmp
//
//  Created by tangwei on 17/12/28.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#ifndef XAudioRecord_hpp
#define XAudioRecord_hpp

#include <stdio.h>

#include "XData.hpp"

#include "QtCore/QThread"

#include "QtMultimedia/QAudioFormat"

#include "QtMultimedia/QAudioDeviceInfo"

#include "QtMultimedia/QAudioInput"

#include "QtCore/QMutex"

#include <iostream>

#include <fstream>

#include <list>

#endif /* XAudioRecord_hpp */

class XAudioRecord:public QThread{

public:
    static XAudioRecord *Get();
    
    bool start();//开始录制音频，并开启线程
    
    void run();//这儿是线程运行，将数据写入到队列中
    
    XData Pop();//这儿是取出队列中的数据
    
    bool isStart = false;//线程是否开启
    
    std::list<XData> datas;//存放数据的队列
    
private:
    
    int maxLength = 100;//在队列中最多能放多少个数据
    
    QAudioInput *input = NULL;
    
    QIODevice *io = NULL;
    
};
