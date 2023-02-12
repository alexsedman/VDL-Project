///
/// main.cpp
/// Variable Delay Line (VDL) Project
///
/// Created by Alex Sedman on 12/01/2023.
///
/// This program requires minimal robustness here - the purpose of this program is to provide a way to generate new WAVs with the implementation of variable delays, using various-order interpolation techniques. It is a closed project, thus does not require as much debugging.
///
/// Test filepath:
/// /Users/alexsedman/Downloads/test.wav
///

/// TODO:
/// 0 order hold
/// nearest neighbour [DONE]
/// linear
/// higher order filters
/// get enzo's code running for an anchor test point

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
    int headerSize = sizeof(wav_hdr), numOfSamples;
    std::string input; // not needed
    
    while (true) {
        input = cmd.mainMenu(input); // Calls main menu.
        
        if (input == "-quit") {
            std::cout << "\nQuitting...\n" << std::endl;
            return 0; // Quits program.
        }
 
        /*---READ---*/
        numOfSamples = wav_hdr.dataSize / 2;
        
        /// FUNCTION SECTION
        /// This part of the code will contain the different forms of delay line/sample interpolation.
        /// This includes a 0 order hold, linear, and bicubic solutions, as well as others.
        /// It also includes solutions and simulations of doppler provided by the STK and SAL libraries, linked below.
        /// WAVs with the various different solutions will be printed to the build folder.

        // Data stream and buffer stream are created.
        int16_t *newAudioData = new(std::nothrow) int16_t[numOfSamples];
        int bufferLen = 1000;
        int16_t *buffer = new(std::nothrow) int16_t[bufferLen]; // Buffer array created in the memory.
        
        // Pointers created, as well as pointer distance and velocity
        int writePtr;
        static double readPtr;
        static double dist = 500;
        static double vel = 0.3; // Read pointer velocity.
        int f0 = 500; // Source frequency.

        for (int sampleIndex = 0; sampleIndex < numOfSamples; sampleIndex++) {
            writePtr = sampleIndex % bufferLen; // Write pointer position defined within the circular buffer.
            
            // Read pointer position defined within the circular buffer.
            readPtr = writePtr - dist;
            while (readPtr < 0) {
                readPtr += bufferLen;
            }
            
            // Write buffer creates sine wav in the buffer.
            buffer[writePtr] = 32767 * sin(2 * M_PI * f0 * sampleIndex / wav_hdr.sampleRate);

            int nearestNeighbour = round(readPtr);
            newAudioData[sampleIndex] = buffer[nearestNeighbour];
            
            dist += vel;
        }
        
        ///
        ///
        ///
        
        
        /*---WRITE---*/
        std::string newFilePath = "/Users/alexsedman/Downloads/test.wav";
        FILE *newFile;
        newFile = fopen(newFilePath.c_str(), "wb");
        fwrite(&wav_hdr, 1, headerSize, newFile);
        fseek(newFile, headerSize, SEEK_SET);
        fwrite(newAudioData, 2, numOfSamples, newFile);
        fclose(newFile);
        std::cout << "\nNew WAVs created.\n" << std::endl;
    }
}
