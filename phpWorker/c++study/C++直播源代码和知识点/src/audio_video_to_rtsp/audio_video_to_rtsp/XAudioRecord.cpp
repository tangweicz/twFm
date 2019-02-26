

//
//  XAudioRecord.cpp
//  audio_video_to_rtsp
//
//  Created by tangwei on 17/11/29.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include "XAudioRecord.hpp"





XAudioRecord *XAudioRecord::Get(XAUDIOTYPE type){

    if (type == X_AUDIO_QT5) {
        
        return new CXAudioRecord;
        
    }
    
    return NULL;
}

bool CXAudioRecord::start(){

    QAudioFormat fmt;
    
    fmt.setChannelCount(this->channels);
    
    fmt.setSampleSize(this->sampleByte * 8);
    
    fmt.setSampleRate(this->sampleRate);
    
    fmt.setSampleType(QAudioFormat::UnSignedInt);
    
    fmt.setByteOrder(QAudioFormat::LittleEndian);
    
    fmt.setCodec("audio/pcm");
    
    QAudioDeviceInfo device = QAudioDeviceInfo::defaultInputDevice();
    
    if(!device.isFormatSupported(fmt)){
    
        cout << "device is not support" << endl;
        
        return false;
        
    }
    
    this->input = new QAudioInput(fmt);
    
    this->io = input->start();
    
    this->isStart = true;
    
    QThread::start();
    
    return true;
};

void CXAudioRecord::run(){

    cout << "this is thread process" << endl;
    
    int readSize = this->nbSamples*this->channels*this->sampleByte * 2;
    
    while(this->isStart){
        if(this->input->bytesReady() < readSize) {
            
            QThread::msleep(1);
            
            continue;
            
        }
        
        char *buf = new char[readSize];
        
        int size = 0;
        
        while(size != readSize){
            
            int len = io->read(buf+size, readSize-size);
            
            if(len <0)break;
            
            size += len;
            
        }
        
        if(size != readSize){
            
            delete[] buf;
            
            continue;
            
        }
        
        XDATA d(buf, readSize);
        
        mutex.lock();
        
        if(datas.size() > this->maxlist){
            
            datas.front().drop();
            
            datas.pop_front();
            
        }
        
        datas.push_back(d);

        mutex.unlock();
        
    }
    
}

XDATA CXAudioRecord::Pop(){
    
    if(datas.empty()){
        
        return XDATA();
        
    }
    
    XDATA d = datas.front();
    
    datas.pop_front();
    
    return d;
    
}

void CXAudioRecord::stop(){
    this->isStart = false;
    
    wait();

    if(this->input)this->input->stop();
    
    if(this->io)this->io->close();
    
    this->input = NULL;
    
    this->io = NULL;
    
}

