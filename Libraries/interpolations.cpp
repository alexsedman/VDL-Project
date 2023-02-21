///
/// interpolations.cpp
/// Variable Delay Line Project
///
/// Created by Alex Sedman on 20/02/2023.
///

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <cmath>
#include "commands.hpp"
#include "interpolations.hpp"
#include "pointercmd.hpp"

pointercmd ptrcmd;

/*---CASE 0: NO DOPPLER---*/
// Output without doppler.
void interpolate::unfiltered() {
    for (int i = 0; i < data_inf.numOfSamples; i++) {
        data_inf.outputStream[i] = data_inf.inputStream[i];
    }
}

/*---CASE 1: ZERO ORDER HOLD---*/
// 0th order Doppler shift: Holds previous sample.
void interpolate::zeroOrderHold() {
    double d = init.dist, v = init.vel;
    
    for (int i = 0; i < data_inf.numOfSamples; i++) {
        int writePtr = ptrcmd.defineWritePointer(i); // Write pointer defined.
        double readPtr = ptrcmd.defineReadPointer(writePtr, d); // Read pointer defined.
        ptrcmd.writeToBuffer(i, writePtr); // Write to buffer.
        
        // ZERO ORDER HOLD: read pointer is assigned to the previous sample (round down using 'floor()')
        int hold_i = floor(readPtr);
        data_inf.outputStream[i] = data_inf.buffer[hold_i]; //
        
        // Distance increased via velocity.
        d += v;
    }
}

/*---CASE 2: NEAREST NEIGHBOUR---*/
// 0th order doppler shift: Uses closest sample.
void interpolate::nearestNeighbour() {
    double d = init.dist, v = init.vel;
    
    for (int i = 0; i < data_inf.numOfSamples; i++) {
        int writePtr = ptrcmd.defineWritePointer(i); // Write pointer defined.
        double readPtr = ptrcmd.defineReadPointer(writePtr, d); // Read pointer defined.
        ptrcmd.writeToBuffer(i, writePtr); // Write to buffer.
        
        // NEAREST NEIGHBOUR: read pointer is rounded to the nearest sample.
        int nearest_i = round(readPtr);
        data_inf.outputStream[i] = data_inf.buffer[nearest_i];
        
        // Distance increased via velocity.
        d += v;
    }
}

/*---CASE 3: LINEAR---*/
// 1st order Doppler shift: finds the linear point between samples.
void interpolate::linear() {
    double d = init.dist, v = init.vel;
    
    for (int i = 0; i < data_inf.numOfSamples; i++) {
        int writePtr = ptrcmd.defineWritePointer(i); // Write pointer defined.
        double readPtr = ptrcmd.defineReadPointer(writePtr, d); // Read pointer defined.
        ptrcmd.writeToBuffer(i, writePtr); // Write to buffer.
        
        int prev_i = floor(readPtr);
        int next_i = ceil(readPtr);
        int16_t prevSample = data_inf.buffer[prev_i];
        int16_t nextSample = data_inf.buffer[next_i];
        
        double readPtrPos = readPtr - (int)readPtr;
        int16_t outputSample = (1 - readPtrPos) * prevSample + readPtrPos * nextSample;
        
        data_inf.outputStream[i] = outputSample;
        
        d += v;
    }
}

/*---CASE 4: QUADRATIC---*/
// 2nd order Doppler shift: finds the quadratic point between samples.
void interpolate::quadratic() {
    double d = init.dist, v = init.vel;

    for (int i = 0; i < data_inf.numOfSamples; i++) {
        int writePtr = ptrcmd.defineWritePointer(i); // Write pointer defined.
        double readPtr = ptrcmd.defineReadPointer(writePtr, d); // Read pointer defined.
        ptrcmd.writeToBuffer(i, writePtr); // Write to buffer.
        
        int prev_i = floor(readPtr) - 1;
        int next_i = ceil(readPtr) + 1;
        int16_t prevSample = data_inf.buffer[(prev_i + data_inf.bufferLen) % data_inf.bufferLen];
        int16_t curSample = data_inf.buffer[(static_cast<int>(floor(readPtr)) + data_inf.bufferLen) % data_inf.bufferLen];
        int16_t nextSample = data_inf.buffer[(next_i) % data_inf.bufferLen];
        
        double readPtrPos = readPtr - floor(readPtr);
        
        // Calculate the coefficients for the quadratic equation
        double a = 0.5 * (prevSample - 2 * curSample + nextSample);
        double b = 0.5 * (nextSample - prevSample);
        double c = curSample;
        
        int16_t outputSample = a * readPtrPos * readPtrPos + b * readPtrPos + c;
        
        data_inf.outputStream[i] = outputSample;
        
        d += v;
    }
}

/*---CASE 5: CUBIC---*/
// 3rd order Doppler shift: finds the cubic point between samples.
void interpolate::cubic() {
    double d = init.dist, v = init.vel;

    for (int i = 0; i < data_inf.numOfSamples; i++) {
        int writePtr = ptrcmd.defineWritePointer(i); // Write pointer defined.
        double readPtr = ptrcmd.defineReadPointer(writePtr, d); // Read pointer defined.
        ptrcmd.writeToBuffer(i, writePtr); // Write to buffer.
        
        int prev2SampleIndex = floor(readPtr) - 2;
        int prevSampleIndex = floor(readPtr) - 1;
        int nextSampleIndex = ceil(readPtr) + 1;
        int next2SampleIndex = ceil(readPtr) + 2;
        
        int16_t prev2Sample = data_inf.buffer[(prev2SampleIndex + data_inf.bufferLen) % data_inf.bufferLen];
        int16_t prevSample = data_inf.buffer[(prevSampleIndex + data_inf.bufferLen) % data_inf.bufferLen];
        int16_t curSample = data_inf.buffer[(static_cast<int>(floor(readPtr)) + data_inf.bufferLen) % data_inf.bufferLen];
        int16_t nextSample = data_inf.buffer[(nextSampleIndex) % data_inf.bufferLen];
        int16_t next2Sample = data_inf.buffer[(next2SampleIndex) % data_inf.bufferLen];
        
        double readPtrPos = readPtr - floor(readPtr);
        
        // Calculate the coefficients for the cubic equation
        double a = (-prev2Sample + 3 * prevSample - 3 * nextSample + next2Sample) / 6.0;
        double b = (prev2Sample - 2 * prevSample + 2 * nextSample - next2Sample) / 2.0;
        double c = (-prev2Sample + next2Sample) / 2.0;
        double d = (prevSample + nextSample) / 2.0;
        
        int16_t outputSample = a * readPtrPos * readPtrPos * readPtrPos + b * readPtrPos * readPtrPos + c * readPtrPos + d;
        
        data_inf.outputStream[i] = outputSample;
        
        d += v;
    }
}

/*---CASE 6: SINC---*/
// Complex order Doppler shift: uses the sinc function to find the ideal sampled value for a given read pointer position and applies a low-pass filter to avoid aliasing.
void interpolate::sinc() {
    double d = init.dist, v = init.vel;
    const double alpha = 0.95;
    double s1 = 0, s2 = 0, s3 = 0, s4 = 0;

    for (int i = 0; i < data_inf.numOfSamples; i++) {
        int writePtr = ptrcmd.defineWritePointer(i); // Write pointer defined.
        double readPtr = ptrcmd.defineReadPointer(writePtr, d); // Read pointer defined.
        ptrcmd.writeToBuffer(i, writePtr); // Write to buffer.
        
        int n = floor(readPtr);
        for (int k = n - 4; k <= n + 4; k++) {
            if (k >= 0 && k < data_inf.bufferLen) {
                double sincVal = (k == n) ? 1.0 : sin(M_PI * (k - n)) / (M_PI * (k - n));
                double sampleVal = data_inf.buffer[k];
                double windowVal = 0.5 * (1 - cos(2 * M_PI * (k - n) / (2 * 4)));
                s1 += sincVal * sampleVal * windowVal;
                s2 += sincVal * windowVal;
            }
        }
        
        double outputSample = s1 / s2;
        data_inf.outputStream[i] = alpha * outputSample + (1 - alpha) * s3;
        s4 = alpha * s2 + (1 - alpha) * s4;
        s3 = alpha * outputSample + (1 - alpha) * s4;
        
        d += v;
    }
}
