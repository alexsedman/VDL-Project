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
    double dis = init.dist, vel = init.vel, acc = init.accel;
    
    for (int i = 0; i < data_inf.numOfSamples; i++) {
        int writePtr = ptrcmd.defineWritePointer(i); // Write pointer defined.
        double readPtr = ptrcmd.defineReadPointer(writePtr, dis); // Read pointer defined.
        ptrcmd.writeToBuffer(i, writePtr); // Write to buffer.
        
        // ZERO ORDER HOLD: read pointer is assigned to the previous sample (round down using 'floor()')
        int hold_i = floor(readPtr);
        data_inf.outputStream[i] = data_inf.buffer[hold_i]; //
        
        // Distance increased via velocity.
        dis += vel;
        vel += acc;
    }
}

/*---CASE 2: NEAREST NEIGHBOUR---*/
// 0th order doppler shift: Uses closest sample.
void interpolate::nearestNeighbour() {
    double dis = init.dist, vel = init.vel, acc = init.accel;
    
    for (int i = 0; i < data_inf.numOfSamples; i++) {
        int writePtr = ptrcmd.defineWritePointer(i); // Write pointer defined.
        double readPtr = ptrcmd.defineReadPointer(writePtr, dis); // Read pointer defined.
        ptrcmd.writeToBuffer(i, writePtr); // Write to buffer.
        
        // NEAREST NEIGHBOUR: read pointer is rounded to the nearest sample.
        int nearest_i = (int)round(readPtr) % data_inf.bufferLen;
        data_inf.outputStream[i] = data_inf.buffer[nearest_i];
        
        // Distance increased via velocity.
        dis += vel;
        vel += acc;
    }
}

/*---CASE 3: LINEAR---*/
// 1st order Doppler shift: finds the linear point between samples.
void interpolate::linear() {
    double dis = init.dist, vel = init.vel, acc = init.accel;
    
    for (int i = 0; i < data_inf.numOfSamples; i++) {
        int writePtr = ptrcmd.defineWritePointer(i); // Write pointer defined.
        double readPtr = ptrcmd.defineReadPointer(writePtr, dis); // Read pointer defined.
        ptrcmd.writeToBuffer(i, writePtr); // Write to buffer.
        
        int prev_i = floor(readPtr);
        int next_i = (int)ceil(readPtr) % data_inf.bufferLen;
        int16_t prevSample = data_inf.buffer[prev_i];
        int16_t nextSample = data_inf.buffer[next_i];
        
        double readPtrPos = readPtr - (int)readPtr;
        int16_t outputSample = (1 - readPtrPos) * prevSample + readPtrPos * nextSample;
        
        data_inf.outputStream[i] = outputSample;
        
        dis += vel;
        vel += acc;
    }
}

// 2nd order Doppler shift: finds the quadratic point between samples.
void interpolate::quadratic() {
    double dis = init.dist, vel = init.vel, acc = init.accel;
    double s0, s1, s2, a, b, c, x, outputSample;

    for (int i = 0; i < data_inf.numOfSamples; i++) {
        int writePtr = ptrcmd.defineWritePointer(i); // Write pointer defined.
        double readPtr = ptrcmd.defineReadPointer(writePtr, dis); // Read pointer defined.
        ptrcmd.writeToBuffer(i, writePtr); // Write to buffer.
        
        // Calculate the nearest sample indices.
        int i1 = (int)floor(readPtr) % data_inf.bufferLen;
        int i0 = (i1 + data_inf.bufferLen - 1) % data_inf.bufferLen;
        int i2 = (i1 + 1) % data_inf.bufferLen;
        
        // Find the amplitude for these 3 nearest samples.
        s0 = data_inf.buffer[i0];
        s1 = data_inf.buffer[i1];
        s2 = data_inf.buffer[i2];
        
        // Calculate coefficients and read pointer position (read pointer should be located between i1 and i2).
        x = readPtr - (int)readPtr + 1;
        a = (s2 - 2 * s1 + s0) / 2.0;
        b = (s2 - s0) / 2.0;
        c = s1;
        
        // 32-bit signed integer calculated first to account for overflow/underflow.
        int32_t outputSample32 = (int32_t)round(a * x * x + b * x + c);

        // Saturation arithmetic
        if (outputSample32 > 32767) {
            outputSample = 32767;
        } else if (outputSample32 < -32768) {
            outputSample = -32768;
        } else {
            outputSample = (int16_t)outputSample32;
        }
                
        data_inf.outputStream[i] = outputSample;
        
        dis += vel;
        vel += acc;
    }
}

/*---CASE 5: CUBIC---*/
// 3rd order Doppler shift: finds the cubic point between samples.
void interpolate::cubic() {
    double dis = init.dist, vel = init.vel, acc = init.accel;

    for (int i = 0; i < data_inf.numOfSamples; i++) {
        int writePtr = ptrcmd.defineWritePointer(i); // Write pointer defined.
        double readPtr = ptrcmd.defineReadPointer(writePtr, dis); // Read pointer defined.
        ptrcmd.writeToBuffer(i, writePtr); // Write to buffer.

        // Calculate the nearest sample indeces.
        int i0 = ((int)floor(readPtr) % data_inf.bufferLen) - 1;
        int i1 = floor(readPtr);
        int i2 = ((int)floor(readPtr) % data_inf.bufferLen) + 1;
        int i3 = ((int)floor(readPtr) % data_inf.bufferLen) + 2;

        // Find the amplitude for these 4 nearest samples. Treating polynomial as if i0 = 0, i1 = 1, i2 = 2, i3 = 3.
        int16_t s0 = data_inf.buffer[i0];
        int16_t s1 = data_inf.buffer[i1];
        int16_t s2 = data_inf.buffer[i2];
        int16_t s3 = data_inf.buffer[i3];

        // Calculate coefficients and read pointer position (read pointer should be located between i1 and i2).
        double a = -0.5 * s0 + 1.5 * s1 - 1.5 * s2 + 0.5 * s3;
        double b = s0 - 2.5 * s1 + 2 * s2 - 0.5 * s3;
        double c = -0.5 * s0 + 0.5 * s2;
        double d = s1;
        double x = readPtr - (int)readPtr + 1;

        // 32-bit signed integer calculated first to account for overflow/underflow.
        int32_t outputSample32 = a * x * x * x + b * x * x + c * x + d;
        int16_t outputSample;

        // Saturation arithmetic
        if (outputSample32 > 32767) {
            outputSample = 32767;
        } else if (outputSample32 < -32768) {
            outputSample = -32768;
        } else {
            outputSample = static_cast<int16_t>(outputSample32);
        }

        data_inf.outputStream[i] = outputSample;

        dis += vel;
        vel += acc;
    }
}

/*---CASE 6: SINC---*/
void interpolate::sinc() {
    double dis = init.dist, vel = init.vel, acc = init.accel;
    double weightedSum = 0.0;

    for (int i = 0; i < data_inf.numOfSamples; i++) {
        int writePtr = ptrcmd.defineWritePointer(i); // Define write pointer.
        double readPtr = ptrcmd.defineReadPointer(writePtr, dis); // Define read pointer.
        ptrcmd.writeToBuffer(i, writePtr); // Write to buffer.

        int n = floor(readPtr);
        double frac = readPtr - floor(readPtr); // Fractional part of read pointer.

        // SINC WINDOW FUNCTION (triangular).
        for (int k = n - 2; k <= n + 2; k++) {
            if (k >= 0 && k < data_inf.bufferLen) {
                double sincVal = (k == n) ? 1.0 : sin(M_PI * (k - n)) / (M_PI * (k - n));
                double sampleVal = data_inf.buffer[k];
                double windowVal = 1 - fabs((k - n) / 4.0);
                weightedSum += sincVal * sampleVal * windowVal;
            }
        }

        // Interpolate the sample.
        double nextSample = data_inf.buffer[n+1]; // Get the next sample.
        double outputSample = weightedSum * (1 - frac) + nextSample * frac; // Interpolate the sample.
        data_inf.outputStream[i] = outputSample; // Store the interpolated sample.

        dis += vel;
        vel += acc;
        weightedSum = 0.0;
    }
}

/*---CASE 7: ANCHOR---*/
void interpolate::anchor() {
    double dis = init.dist, vel = init.vel, acc = init.accel;
    double weightedSum = 0.0;

    for (int i = 0; i < data_inf.numOfSamples; i++) {
        int writePtr = ptrcmd.defineWritePointer(i); // Define write pointer.
        double readPtr = ptrcmd.defineReadPointer(writePtr, dis); // Define read pointer.
        ptrcmd.writeToBuffer(i, writePtr); // Write to buffer.

        int n = floor(readPtr);
        double frac = readPtr - floor(readPtr); // Fractional part of read pointer.

        // SINC WINDOW FUNCTION (triangular).
        for (int k = n - 2; k <= n + 2; k++) {
            if (k >= 0 && k < data_inf.bufferLen) {
                double sincVal = (k == n) ? 1.0 : sin(M_PI * (k - n)) / (M_PI * (k - n));
                double sampleVal = data_inf.buffer[k];
                double windowVal = 0.5 * (1 - cos(2 * M_PI * (k - n) / (2 * 4)));
                weightedSum += sincVal * sampleVal * windowVal;
            }
        }

        // Interpolate the sample.
        double nextSample = data_inf.buffer[n+1]; // Get the next sample.
        double outputSample = weightedSum * (1 - frac) + nextSample * frac; // Interpolate the sample.
        data_inf.outputStream[i] = outputSample; // Store the interpolated sample.

        dis += vel;
        vel += acc;
        weightedSum = 0.0;
    }
}

/*---CASE 8: REFERENCE---*/
void interpolate::reference() {
    double dis = init.dist, vel = init.vel, acc = init.accel;
    double weightedSum = 0.0;

    for (int i = 0; i < data_inf.numOfSamples; i++) {
        int writePtr = ptrcmd.defineWritePointer(i); // Define write pointer.
        double readPtr = ptrcmd.defineReadPointer(writePtr, dis); // Define read pointer.
        ptrcmd.writeToBuffer(i, writePtr); // Write to buffer.

        int n = floor(readPtr);
        double frac = readPtr - floor(readPtr); // Fractional part of read pointer.

        // SINC WINDOW FUNCTION (triangular).
        for (int k = n - 256; k <= n + 256; k++) {
            if (k >= 0 && k < data_inf.bufferLen) {
                double sincVal = (k == n) ? 1.0 : sin(M_PI * (k - n)) / (M_PI * (k - n));
                double sampleVal = data_inf.buffer[k];
                double windowVal = 1 - fabs((k - n) / 4.0);
                weightedSum += sincVal * sampleVal * windowVal;
            }
        }

        // Interpolate the sample.
        double nextSample = data_inf.buffer[n+1]; // Get the next sample.
        double outputSample = weightedSum * (1 - frac) + nextSample * frac; // Interpolate the sample.
        data_inf.outputStream[i] = outputSample; // Store the interpolated sample.

        dis += vel;
        vel += acc;
        weightedSum = 0.0;
    }
}
