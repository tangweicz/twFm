//
//  tcpThread.hpp
//  sockettw
//
//  Created by tangwei on 17/9/18.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#ifndef tcpThread_hpp
#define tcpThread_hpp

#include <stdio.h>

#include "XTcp.hpp"

class tcpThread{

public:
    void run();
    
    XTcp sock;
    
};

#endif /* tcpThread_hpp */
