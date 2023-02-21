///
/// main.cpp
/// Variable Delay Line (VDL) Project
///
/// Created by Alex Sedman on 12/01/2023.
///
/// This program requires minimal robustness here - the purpose of this program is to provide a way to generate new WAVs with the implementation of variable delays, using various-order interpolation techniques. It is a closed project, thus does not require as much debugging.
/// Furthermore, I've chosen to use a few shortcuts here - though having a lot of void functions and duplicate code is generally bad practice, considering that this code is a quick solution to produce various WAVs, the structure here will suffice.
///
/// TEST PATHNAME: /Users/alexsedman/Documents/Xcode Projects/Variable Delay Line Project/RealSpringMono.wav
///

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <cmath>
#include "commands.hpp"
#include "interpolations.hpp"

int main() {
    /*---CLASSES---*/
    commands cmd;
    interpolate filter;
    
    /*---VARIABLES---*/
    std::string inputName, outputName;
    const char* filePath;
    FILE* wavFile;
    
    
    while (true) {
        /*---MENU---*/
        inputName = cmd.mainMenu(inputName);
        
        /*---QUIT---*/
        if (inputName == "-quit") {
            std::cout << "\nQuitting...\n" << std::endl;
            return 0; // Quits program.
        }
        
        /*---FOPEN---*/
        filePath = inputName.c_str(); // Sets the file path to the user input.
        wavFile = fopen(filePath, "rb"); // Attempts to open the inputted file.
        
        /*---FOPEN ERROR---*/
        if (!wavFile) {
            std::cout << "\nERROR: File can't be read/invalid input.\n" << std::endl;
            continue; // If the input is invalid (i.e. 'fopen' returns a null pointer), then this error is thrown and the loop is skipped.
        }
        
        /*---READ FILE---*/
        cmd.read(wavFile);
        
        /*---HDR INF ERROR---*/
        if (wav_hdr.numChannels != 1 || wav_hdr.bitsPerSample != 16) {
            std::cout << "\nERROR: File is not a WAV, or is not mono/16-bit.\n" << std::endl;
            continue;
        }
        
        // File read successful:
        std::cout << "\nFile read successful." << std::endl;
        cmd.printHdr(wavFile);
        
        /*---FUNCTION SECTION---*/
        /// This part of the code will contain the different forms of delay line/sample interpolation.
        /// This includes a 0 to many order interpolation solutions as well as others.
        /// It also includes solutions and simulations of doppler, inspired by the STK and SAL libraries, linked above.
        /// WAVs with the various different solutions will be printed to the build folder.
        /// For the sake of convenience, I have separated the various interpolation methods into separate functions.
        /// This means there may be some duplicate code here,  but it makes the code more readable and simpler to implement for now.
        
        /*---INTERPOLATION---*/
        for (int method = 0; method < 7; method++) {
            switch (method) {
                case 0:
                    filter.unfiltered();
                    outputName = cmd.rename(inputName, "-0-Unfiltered");
                    break;
                case 1:
                    filter.zeroOrderHold();
                    outputName = cmd.rename(inputName, "-0-ZeroOrderHold");
                    break;
                case 2:
                    filter.nearestNeighbour();
                    outputName = cmd.rename(inputName, "-0-NearestNeighbour");
                    break;
                case 3:
                    filter.linear();
                    outputName = cmd.rename(inputName, "-1-Linear");
                    break;
                case 4:
                    filter.quadratic();
                    outputName = cmd.rename(inputName, "-2-Quadratic");
                    break;
                case 5:
                    filter.cubic();
                    outputName = cmd.rename(inputName, "-3-Cubic");
                    break;
                case 6:
                    filter.sinc();
                    outputName = cmd.rename(inputName, "-#-Sinc");
                    break;
            }
            
            /*---WRITE & RESET---*/
            cmd.write(outputName);
            cmd.reset();
        }
    }
}
