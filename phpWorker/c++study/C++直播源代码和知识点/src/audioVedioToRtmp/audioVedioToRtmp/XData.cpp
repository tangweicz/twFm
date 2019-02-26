
//
//  XData.cpp
//  audioVedioToRtmp
//
//  Created by tangwei on 18/1/2.
//  Copyright © 2018年 tangwei. All rights reserved.
//


#include "XData.hpp"

void XData::Drop(){

    if(this->data != NULL)
        delete this->data;
        
    this->data = NULL;
    
    this->leng = 0;
    
}

XData::XData(int size, char *da){

    this->data = new char[size];
        
    memcpy(this->data, da, size);
    
    this->leng = size;

    
}

XData::XData(const XData & another){

    if(this != &another){//只有当拷贝的两者不是自己的时候，才会处理
        
        if(another.data != NULL){
            
            this->data = new char[another.leng];
            
            memcpy(this->data, another.data, another.leng);
            
            this->leng = another.leng;
            
            this->timestamp = av_gettime();
            
        }
    
        
    }
    
}

XData::XData(){

}

XData::~XData(){

    if(this->data != NULL){
    
        delete this->data;
    }
    
    this->data = NULL;
    
    this->leng = 0;
    
}



