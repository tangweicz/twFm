






//
//  xdata.cpp
//  audio_video_to_rtsp
//
//  Created by tangwei on 17/12/4.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include "xdata.hpp"


void XDATA::drop(){
    
    if(data)
        delete data;
        
    data = 0;
        
    dataSize = 0;
    
}

XDATA::XDATA(){

}

XDATA::XDATA(char *da, int size){
        
    this->data = new char[size];
        
    memcpy(this->data, da, size);
        
    this->dataSize = size;
    
    this->pts = av_gettime();
    
}
