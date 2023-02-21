//
//  pointercmd.hpp
//  Variable Delay Line Project
//
//  Created by Alex Sedman on 21/02/2023.
//

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#ifndef pointercmd_hpp
#define pointercmd_hpp

class pointercmd {
public:
    int defineWritePointer(int i);
    double defineReadPointer(int writePtr, double d);
    
    void writeToBuffer(int i, int writePtr);
};

#endif /* pointercmd_hpp */
