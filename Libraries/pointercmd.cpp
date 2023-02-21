//
//  pointercmd.cpp
//  Variable Delay Line Project
//
//  Created by Alex Sedman on 21/02/2023.
//

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <cmath>
#include "commands.hpp"
#include "interpolations.hpp"
#include "pointercmd.hpp"

int pointercmd::defineWritePointer(int i) {
    int writePtr = i % data_inf.bufferLen;
    return writePtr;
}

double pointercmd::defineReadPointer(int writePtr, double d) {
    double readPtr = writePtr - d;
    while (readPtr < 0) {readPtr += data_inf.bufferLen;}
    return readPtr;
}

void pointercmd::writeToBuffer(int i, int writePtr) {
    data_inf.buffer[writePtr] = data_inf.inputStream[i]; // Write audio data to buffer.
}
