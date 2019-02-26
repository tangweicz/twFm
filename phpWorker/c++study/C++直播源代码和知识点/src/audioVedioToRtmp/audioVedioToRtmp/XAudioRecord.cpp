//
//  XAudioRecord.cpp
//  audioVedioToRtmp
//
//  Created by tangwei on 17/12/28.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include "XAudioRecord.hpp"

XAudioRecord * XAudioRecord::Get(){
    
    return new XAudioRecord;
    
}

bool XAudioRecord::start(){

    QAudioFormat fmt;
    
    fmt.setSampleRate(44100);//设置一次采样采多少个样本
    
    fmt.setChannelCount(2);//音频通道数量
    
    fmt.setSampleSize(8);//样本存放的大小，一般是8或者16
    
    fmt.setSampleType(QAudioFormat::UnSignedInt);//样本类型，无符号整型
    
    fmt.setCodec("audio/pcm");
    
    fmt.setByteOrder(QAudioFormat::LittleEndian);//字节序采用小尾端的形式
    
    QAudioDeviceInfo device = QAudioDeviceInfo::defaultInputDevice();
    
    if(!device.isFormatSupported(fmt)){
    
        std::cout << "no device support audio" << std::endl;
        
        return false;
        
    }
    
    this->isStart = true;
    
    this->input = new QAudioInput(fmt);
    
    this->io = input->start();//开始录制音频
    
    QThread::start();
    
    return true;
    
}

void XAudioRecord::run(){
    
    std::cout << "thread process start" << std::endl;
    
    char *buf = new char[4096];
    
    while(this->isStart){//循环开始往队列中放入数据
    
        if(this->input->bytesReady() < 4096) continue;//以字节为单位返回可读取的音频数据量
        
        int size = 0;
        
        while(size != 4096){
            
            int len = this->io->read(buf+size, 4096-size);
            
            size += len;
            
            if(size != 4096) {
                
                delete[] buf;
                
                continue;
                
            }
            
        }
        
        XData da(size, buf);
        
        if(datas.size() > this->maxLength){//如果数据队列的长度超过了最长的长度，那么从头开始先删掉一个
            
            std::cout << "audio full" << std::endl;
            
            datas.front().Drop();
        
            datas.pop_front();
            
        }
        
        datas.push_back(da);//将音频数据写入到队列
        
    }
    
}

XData XAudioRecord::Pop(){
    
    if(datas.empty()){
        
        return XData();
        
    }
    
    XData d = datas.front();
    
    datas.front().Drop();
    
    datas.pop_front();
    
    return d;
}



























