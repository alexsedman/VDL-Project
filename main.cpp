///
/// main.cpp
/// Variable Delay Line (VDL) Project
///
/// Created by Alex Sedman on 12/01/2023.
///
/// This program requires minimal robustness here - the purpose of this program is to provide a way to generate new WAVs with the implementation of variable delays, using various-order interpolation techniques. It is a closed project, thus does not require as much debugging.
///

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include "commands.hpp"
#include "interpolate.hpp"

int main() {
    // Classes initialised.
    commands cmd;
    interpolate edit;
    
    // Variables declared.
    int headerSize = sizeof(wav_hdr), numOfSamples;
    std::string input;
    const char* filePath;
    int16_t* audioData;
    FILE* wavFile;
    
    while (true) {
        input = cmd.mainMenu(input); // Calls main menu.
        
        if (input == "-quit") {
            std::cout << "\nBye!\n" << std::endl;
            return 0; // Quits program.
        }
    
        filePath = input.c_str(); // Sets the file path to the user input.
        wavFile = fopen(filePath, "rb"); // Attempts to open the inputted file.
        
        if (!wavFile) {
            std::cout << "\nERROR: File can't be read/input is invalid.\n" << std::endl;
            continue; // If the input is invalid (i.e. 'fopen' returns a null pointer), then this error is thrown and the loop is skipped.
        }
        
        /*---READ---*/
        fread(&wav_hdr, 1, headerSize, wavFile); // Header information is read from 'wavFile'.
        fseek(wavFile, headerSize, SEEK_SET); // File pointer is set to the start of the audio stream.
        numOfSamples = wav_hdr.dataSize / 2;
        int16_t* audioData = new(std::nothrow) int16_t[numOfSamples]; // A pointer to a dynamic array is created; the audio stream will be stored here.
        fread(audioData, 2, numOfSamples, wavFile); // Audio stream is read from 'wavFile'.
        fclose(wavFile);

        if (wav_hdr.numChannels != 1 || wav_hdr.bitsPerSample != 16) {
            std::cout << "\nERROR: File not mono 16-bit.\n" << std::endl;
            continue; // If the file is stereo, throw an error:
        }
        std::cout << "\nFile read successful!\n" << std::endl;
        cmd.printHdr(headerSize, wavFile); // File header information is printed.
        input = cmd.optionsMenu(input); // Calls the options menu.
        
        /// FUNCTION SECTION
        /// This part of the code will contain the different forms of delay line/sample interpolation.
        /// This includes a 0 order hold, linear, and bicubic solutions, as well as others.
        /// It also includes solutions and simulations of doppler provided by the STK and SAL libraries, linked below.
        /// WAVs with the various different solutions will be printed to the build folder.
        
        
        
        ///
        ///
        ///
        
        std::string newFilePath = cmd.filenameMenu(input, filePath); // Asks user for new filename.
        
        /*---WRITE---*/
        FILE* newFile;
        newFile = fopen(newFilePath.c_str(), "wb"); // Open the new file in write binary mode.
        fwrite(&wav_hdr, 1, headerSize, newFile); // Write header.
        fseek(newFile, headerSize, SEEK_SET); // Set file pointer to the start of the audio stream.
        fwrite(audioData, 2, numOfSamples, newFile); // Write audio data.
        fclose(newFile); // Close file.
        std::cout << std::endl; // Console new line.
    }
}
