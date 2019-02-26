//
//  XVideoRecord.cpp
//  audio_video_to_rtsp
//
//  Created by tangwei on 17/12/4.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include "XVideoRecord.hpp"

XVideoRecord * XVideoRecord::Get(){

    return new CXVideoRecord();
    
}

bool CXVideoRecord::init(int cameraIndex){
    
    camera.open(cameraIndex);
    
    if(!camera.isOpened()){
    
        std::cout << "打开摄像头失败" << std::endl;
        
        return false;
        
    }
    
    this->frameWidth = camera.get(CAP_PROP_FRAME_WIDTH);
    
    this->frameHeight = camera.get(CAP_PROP_FRAME_HEIGHT);
    
    this->fps = camera.get(CAP_PROP_FPS);
    
    return true;
    
}

bool CXVideoRecord::init(const char *url){
    
    camera.open(url);
    
    if(!camera.isOpened()){
        
        std::cout << "打开摄像头失败" << std::endl;
        
        return false;
        
    }
    
    this->frameWidth = camera.get(CAP_PROP_FRAME_WIDTH);
    
    this->frameHeight = camera.get(CAP_PROP_FRAME_HEIGHT);
    
    this->fps = camera.get(CAP_PROP_FPS);
    
    return true;

}

void CXVideoRecord::stop(){
    
    this->isStart = false;

    if(this->camera.isOpened()){
    
        this->camera.release();
        
    }
    
}

bool CXVideoRecord::start(){

    this->isStart = true;
    
    QThread::start();
    
    return true;
}

void CXVideoRecord::run(){

    Mat img;
    
    while(this->isStart){
    
        this->camera.read(img);
        
        XDATA d((char *)img.data, img.cols * img.rows * img.elemSize());
        
        mutex.lock();
        
        if(datas.size() > this->maxsize){
            
            cout << "end stop" << endl;
        
            datas.front().drop();
            
            datas.pop_front();
            
        }
        
        datas.push_back(d);
        
        mutex.unlock();
        
    }
    
};

XDATA CXVideoRecord::Pop(){
    
    if(datas.empty()){
        
        return XDATA();
        
    }
    
    XDATA d = datas.front();
    
    datas.pop_front();
    
    return d;
    
}

