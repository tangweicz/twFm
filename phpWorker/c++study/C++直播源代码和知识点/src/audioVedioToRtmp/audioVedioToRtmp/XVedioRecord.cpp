//
//  XVedioRecord.cpp
//  audioVedioToRtmp
//
//  Created by tangwei on 18/1/10.
//  Copyright © 2018年 tangwei. All rights reserved.
//

#include "XVedioRecord.hpp"

XVedioRecord *XVedioRecord::Get(){

    return new XVedioRecord();
    
}

bool XVedioRecord::start(){
    
    this->camera.open(0);//打开摄像头
    
    if(!this->camera.isOpened()){
    
        std::cout << "camera open failed" << std::endl;
        
        return false;
        
    }
    
    this->frameWidth = camera.get(CV_CAP_PROP_FRAME_WIDTH);
    
    this->frameHeight = camera.get(CV_CAP_PROP_FRAME_HEIGHT);
    
    this->isStart = true;
    
    QThread::start();
    
    return true;
}

void XVedioRecord::run(){

    cv::Mat img;
    
    while(this->isStart){
        
        this->camera.read(img);
        
        if(datas.size() > this->maxLength){//如果有数据在队列中，一直没取走，那么达到100个链路表，就需要对前面的先清理掉一个
            
            std::cout << "video full" << std::endl;
        
            datas.front().Drop();
            
            datas.pop_front();
            
        }
        
        int size = img.cols * img.rows * img.elemSize();
        
        XData da(size, (char *)img.data);
        
        datas.push_back(da);
        
        if(this->colsNum == 0){
        
            this->colsNum = img.cols;
            
        }
        
        if(this->elemSize == 0){
        
            this->elemSize = img.elemSize();
            
            
        }
        
    }
    
}

XData XVedioRecord::Pop(){

    if(datas.empty()){
        
        return XData();
        
    }
    
    XData da = datas.front();
    
    datas.front().Drop();
    
    datas.pop_front();
    
    return da;
    
    
    
}
