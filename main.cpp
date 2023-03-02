//
// main.cpp
// Variable Delay Line (VDL) Project
//
// Created by Alex Sedman on 12/01/2023.
//
// This program requires minimal robustness here - the purpose of this program is to provide a way to generate new WAVs with the implementation of variable delays, using various-order interpolation techniques. It is a closed project, thus does not require as much debugging.
// Furthermore, I've chosen to use a few shortcuts here - though having a lot of void functions and duplicate code is generally bad practice, considering that this code is a quick solution to produce various WAVs, the structure here will suffice.
//

#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <cmath>
#include "commands.hpp"
#include "interpolations.hpp"

int main() {
    // Classes
    commands cmd;
    interpolate filter;
    
    // Variables
    std::string input, outputName;
    const char* filePath;
    
    while (true) {
        input = cmd.mainMenu(input);
        
        /*---QUIT---*/
        if (input == "-quit") {
            std::cout << "\nQuitting...\n" << std::endl;
            return 0; // Quits program.
        }
        
        /*---FOPEN---*/
        filePath = input.c_str(); // Sets the file path to the user input.
        FILE* wavFile = fopen(filePath, "rb"); // Attempts to open the inputted file.
        
        /*---FOPEN ERROR---*/
        if (!wavFile) {
            std::cout << "\nERROR: File can't be read/invalid input.\n" << std::endl;
            continue; // If the input is invalid (i.e. 'fopen' returns a null pointer), then this error is thrown and the loop is skipped.
        }
        
        cmd.read(wavFile);
        
        /*---HDR INF ERROR---*/
        if (wav_hdr.numChannels != 1 || wav_hdr.bitsPerSample != 16) {
            std::cout << "\nERROR: File is not a WAV, or is not mono/16-bit.\n" << std::endl;
            continue; // If the file is stereo, throw an error:
        }
        
        // File read successful:
        std::cout << "\nFile read successful.\n" << std::endl;
        cmd.printHdr(wavFile); // File header information is printed.
        
        /*---FUNCTION SECTION---*/
        /// This part of the code will contain the different forms of delay line/sample interpolation.
        /// This includes a 0 to many order interpolation solutions as well as others.
        /// It also includes solutions and simulations of doppler, inspired by the STK and SAL libraries, linked above.
        /// WAVs with the various different solutions will be printed to the build folder.
        /// For the sake of convenience, I have separated the various interpolation methods into separate functions. This means there may be some duplicate code here, as some methods may share very similar setups, but it makes the code more readable and simpler to implement for now.
        
        /*---INTERPOLATION---*/
        
        
        
        for (int method = 0; method < 9; method++) {
            auto clockStart = std::chrono::high_resolution_clock::now();
            
            switch (method) {
                case 0:
                    filter.unfiltered();
                    outputName = cmd.rename(input, "-1-Unfiltered.wav");
                    break;
                case 1:
                    filter.zeroOrderHold();
                    outputName = cmd.rename(input, "-2-ZeroOrderHold.wav");
                    break;
                case 2:
                    filter.nearestNeighbour();
                    outputName = cmd.rename(input, "-3-NearestNeighbour.wav");
                    break;
                case 3:
                    filter.linear();
                    outputName = cmd.rename(input, "-4-Linear.wav");
                    break;
                case 4:
                    filter.quadratic();
                    outputName = cmd.rename(input, "-5-Quadratic.wav");
                    break;
                case 5:
                    filter.cubic();
                    outputName = cmd.rename(input, "-6-Cubic.wav");
                    break;
                case 6:
                    filter.sinc();
                    outputName = cmd.rename(input, "-7-Sinc.wav");
                    break;
                case 7:
                    filter.anchor();
                    outputName = cmd.rename(input, "-0-Anchor.wav");
                    break;
                case 8:
                    filter.reference();
                    outputName = cmd.rename(input, "-8-Reference.wav");
                    break;
            }
            
            auto clockEnd = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(clockEnd - clockStart);
            std::cout << "Time taken to generate " << outputName << ": " << duration.count() << "μs" << std::endl;
            
            /*---WRITE & RESET---*/
            cmd.write(outputName);
            cmd.reset();
        }
        std::cout << "VALUES: d = " << init.dist << "; v = " << init.vel << "; a = " << init.accel << "\n\n";
    }
}
