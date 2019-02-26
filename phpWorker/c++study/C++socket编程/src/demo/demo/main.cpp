//
//  main.cpp
//  demo
//
//  Created by tangwei on 17/10/11.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include <iostream>



int main(int argc, const char * argv[]) {
    
    struct aaa{
    
        int age;
        
    };
    
    aaa *b = new aaa;
    
    b->age = 10;
    
    std::cout << b->age << std::endl;
}
