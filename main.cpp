///
/// main.cpp
/// Variable Delay Line (VDL) Project
///
/// Created by Alex Sedman on 12/01/2023.
///
/// This program requires minimal robustness here - the purpose of this program is to provide a way to generate new WAVs with the implementation of variable delays, using various-order interpolation techniques. It is a closed project, thus does not require as much debugging.
/// Furthermore, I've chosen to use a few shortcuts here - though having a lot of void functions and duplicate code is generally bad practice, considering that this code is a quick solution to produce various WAVs, the structure here will suffice.
///

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <cmath>
#include "commands.hpp"

int main() {
    // Classes initialised.
    commands cmd;
    
    // Variables declared.
    int headerSize = sizeof(wav_hdr), numOfSamples = wav_hdr.dataSize / 2;
    std::string filename;
    
    // Main menu called (modified from a different project).
    cmd.menu();
    
    /// FUNCTION SECTION
    /// This part of the code will contain the different forms of delay line/sample interpolation.
    /// This includes a 0 to many order interpolation solutions as well as others.
    /// It also includes solutions and simulations of doppler, inspired by the STK and SAL libraries, linked above.
    /// WAVs with the various different solutions will be printed to the build folder.

    // Data stream and buffer stream are created.
    int16_t *newAudioData = new(std::nothrow) int16_t[numOfSamples];
    int bufferLen = 2048;
    int16_t *buffer = new(std::nothrow) int16_t[bufferLen]; // Buffer array of length 'bufferLen' created in the memory.
    
    // Pointers created, as well as pointer properties.
    int writePtr = 0;
    double readPtr = 0.0;
    
    // For the sake of convenience, I have separated the various interpolation methods into separate functions. This means there may be some duplicate code here, as some methods may share very similar setups, but it makes the code more readable and simpler to implement for now.
    
    // For loop to iterate through the various methods of sample interpolation, and write to appropriate files.
    for (int method = 0; method < 2; method++) {
        switch (method) {
            case 0:
                cmd.zeroOrderHold(newAudioData, numOfSamples, buffer, bufferLen, writePtr, readPtr);
                filename = "/Users/alexsedman/Documents/Xcode Projects/Variable Delay Line Project/Test WAVs/zeroOrderHold.wav";
                break;
            case 1:
                cmd.nearestNeighbour(newAudioData, numOfSamples, buffer, bufferLen, writePtr, readPtr);
                filename = "/Users/alexsedman/Documents/Xcode Projects/Variable Delay Line Project/Test WAVs/nearestNeighbour.wav";
                break;
        }
        /// TODO:
        /// 0 order hold
        /// nearest neighbour [DONE]
        /// linear
        /// higher order filters
        /// get enzo's code running for an anchor test point
        
        cmd.write(headerSize, newAudioData, numOfSamples, filename);
        cmd.reset(buffer, bufferLen);
    }
    
    ///
    ///
    ///
    
    std::cout << "Program terminating..." << std::endl;
    return 0;
}
