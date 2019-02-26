
//
//  xdata.hpp
//  audio_video_to_rtsp
//
//  Created by tangwei on 17/12/4.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#ifndef xdata_hpp
#define xdata_hpp

#include <stdio.h>

#include<string.h>

extern "C"{

#include <libavutil/time.h>
    
}

class XDATA{
    
public:
    char *data = NULL;
    
    int dataSize = 0;
    
    void drop();
    
    XDATA();
    
    XDATA(char *da, int size);
    
    long long pts = 0;
    
};

#endif /* xdata_hpp */
