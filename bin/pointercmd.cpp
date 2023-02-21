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
    int writePtr = i % data.bufferLen;
    return writePtr;
}

double pointercmd::defineReadPointer(int writePtr, double d) {
    double readPtr = writePtr - d;
    while (readPtr < 0) {readPtr += data.bufferLen;}
    return readPtr;
}

void pointercmd::writeToBuffer(int i, int writePtr) {
    data.buffer[writePtr] = data.inputStream[i]; // Write audio data to buffer.
}
