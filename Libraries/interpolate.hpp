//
// interpolate.hpp
// VDL Project
//
// Created by Alex Sedman.
//

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#ifndef interpolate_hpp
#define interpolate_hpp

class interpolate {
public:
    // WAV file edit functions (options).
    void printHdr(int headerSize, FILE* wavFile); // Prints header.
    void changeSampleRate(std::string input); // Changes sample rate.
    void addPause(int16_t* audioData, int numOfSamples); // Adds a pause.
    void normalise(int16_t* audioData, int numOfSamples); // Normalises audio.
    void reverse(int16_t* audioDaa, int numOfSamples); // Reverses audio.
};

#endif
