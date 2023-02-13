///
/// commands.hpp
/// VDL Project
///
/// Created by Alex Sedman.
///

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <cmath>
#include "commands.hpp"

/*---MAIN MENU---*/
// Function to output main menu and return the user input.
void commands::menu() {
    using namespace std;
    
    //Print the main menu.
    cout << "---MAIN MENU---" << endl;
    cout << "Program starting, generating WAVs..." << endl;
}

/*---WRITE---*/
// Function to write to file.
void commands::write(int headerSize, int16_t *newAudioData, int numOfSamples, std::string newFilePath) {
    FILE *newFile;
    newFile = fopen(newFilePath.c_str(), "wb");
    fwrite(&wav_hdr, 1, headerSize, newFile);
    fseek(newFile, headerSize, SEEK_SET);
    fwrite(newAudioData, 2, numOfSamples, newFile);
    fclose(newFile);
    std::cout << "New WAV '" << newFilePath << "' created." << std::endl;
}

/*---RESET---*/
// Resets the buffer, and any other values, ready for a different interpolation method.
void commands::reset(int16_t *buffer, int bufferLen) {
    for (int i = 0; i < bufferLen; i++) {
        buffer[i] = NULL;
    }
}

///
/// INTERPOLATION METHODS:
///

/*---ZERO ORDER HOLD---*/
// 0th order: Holds previous sample.
void commands::zeroOrderHold(int16_t *newAudioData, int numOfSamples, int16_t *buffer, int bufferLen, int writePtr, double readPtr) {
    int f0 = init.freq;
    double d = init.dist, v = init.vel, a = init.accel;
    
    for (int sampleIndex = 0; sampleIndex < numOfSamples; sampleIndex++) {
        writePtr = sampleIndex % bufferLen; // Write pointer position defined within the circular buffer.
        
        // Read pointer position defined within the circular buffer.
        readPtr = writePtr - d;
        while (readPtr < 0) {
            readPtr += bufferLen;
        }
        
        // Write buffer creates sine wav in the buffer.
        // I have used amplitude A = 21954 here, roughly 2/3 of the full scale. I feel like this is an appropriate volume for listening.
        buffer[writePtr] = 21954 * sin(2 * M_PI * f0 * sampleIndex / wav_hdr.sampleRate);
        
        int holdIndex = floor(readPtr);
        newAudioData[sampleIndex] = buffer[holdIndex];
        
        d += v;
        v += a;
    }
}

/*---NEAREST NEIGHBOUR---*/
// 0th order: Uses closest sample.
void commands::nearestNeighbour(int16_t *newAudioData, int numOfSamples, int16_t *buffer, int bufferLen, int writePtr, double readPtr) {
    int f0 = init.freq;
    double d = init.dist, v = init.vel, a = init.accel;
    
    for (int sampleIndex = 0; sampleIndex < numOfSamples; sampleIndex++) {
        writePtr = sampleIndex % bufferLen; // Write pointer position defined within the circular buffer.
        
        // Read pointer position defined within the circular buffer.
        readPtr = writePtr - d;
        while (readPtr < 0) {
            readPtr += bufferLen;
        }
        
        // Write buffer creates sine wav in the buffer.
        // I have used amplitude A = 21954 here, roughly 2/3 of the full scale. I feel like this is an appropriate volume for listening.
        buffer[writePtr] = 21954 * sin(2 * M_PI * f0 * sampleIndex / wav_hdr.sampleRate);
        
        int nearestNeighbour = round(readPtr);
        newAudioData[sampleIndex] = buffer[nearestNeighbour];
        
        d += v;
        v += a;
    }
}

/*---LINEAR---*/
// 1st order: finds the linear point between samples.
void commands::linear(int16_t *newAudioData, int numOfSamples, int16_t *buffer, int bufferLen, int writePtr, double readPtr) {
    int f0 = init.freq;
    double d = init.dist, v = init.vel, a = init.accel;
    
    for (int sampleIndex = 0; sampleIndex < numOfSamples; sampleIndex++) {
        writePtr = sampleIndex % bufferLen; // Write pointer position defined within the circular buffer.
        
        // Read pointer position defined within the circular buffer.
        readPtr = writePtr - d;
        while (readPtr < 0) {
            readPtr += bufferLen;
        }
        
        // Write buffer creates sine wav in the buffer.
        // I have used amplitude A = 21954 here, roughly 2/3 of the full scale. I feel like this is an appropriate volume for listening.
        buffer[writePtr] = 21954 * sin(2 * M_PI * f0 * sampleIndex / wav_hdr.sampleRate);
        
        int prevSampleIndex = floor(readPtr);
        int nextSampleIndex = ceil(readPtr);
        int16_t prevSample = buffer[prevSampleIndex];
        int16_t nextSample = buffer[nextSampleIndex];
        
        double readPtrPos = readPtr - (int)readPtr;
        int16_t outputSample = (1 - readPtrPos) * prevSample + readPtrPos * nextSample;
        
        newAudioData[sampleIndex] = outputSample;
        
        d += v;
        v += a;
    }
}

///
///
///
