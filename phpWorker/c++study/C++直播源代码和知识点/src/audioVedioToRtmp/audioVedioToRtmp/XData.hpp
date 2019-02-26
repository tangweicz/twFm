//
//  XData.hpp
//  audioVedioToRtmp
//
//  Created by tangwei on 18/1/2.
//  Copyright © 2018年 tangwei. All rights reserved.
//

#ifndef XData_hpp
#define XData_hpp

#include <stdio.h>

#include <string>

#include <iostream>

extern "C"{
    
#include <libavutil/time.h>
    
}

class XData{

public:
    char *data = NULL;
    
    int leng;
    
    void Drop();
    
    XData();
    
    XData(int size, char *da);
    
    XData(const XData &another);
    
    ~XData();
    
    long long timestamp = 0;
    
};
 
 #endif /* xdata_hpp */

