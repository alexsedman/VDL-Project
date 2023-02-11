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
#include <cmath>
#include "commands.hpp"
#include "delayLine.hpp"

int main() {
    // Classes initialised.
    commands cmd;
    delayLine dly;
    
    // Variables declared.
    int headerSize = sizeof(wav_hdr), numOfSamples;
    std::string input;
    const char* filePath;
    int16_t* audioData;
    FILE* wavFile;
    
    while (true) {
        input = cmd.mainMenu(input); // Calls main menu.
        
        if (input == "-quit") {
            std::cout << "\nQuitting...\n" << std::endl;
            return 0; // Quits program.
        }
    
        filePath = input.c_str(); // Sets the file path to the user input.
        wavFile = fopen(filePath, "rb"); // Attempts to open the inputted file.
        
        // Read fail error message.
        if (!wavFile) {
            std::cout << "\nERROR: File can't be read/invalid input.\n" << std::endl;
            continue; // If the input is invalid (i.e. 'fopen' returns a null pointer), then this error is thrown and the loop is skipped.
        }
        
        /*---READ---*/
        fread(&wav_hdr, 1, headerSize, wavFile); // Header information is read from 'wavFile'.
        fseek(wavFile, headerSize, SEEK_SET); // File pointer is set to the start of the audio stream.
        numOfSamples = wav_hdr.dataSize / 2;
        int16_t* audioData = new(std::nothrow) int16_t[numOfSamples]; // A pointer to a dynamic array is created; the audio stream will be stored here.
        fread(audioData, 2, numOfSamples, wavFile); // Audio stream is read from 'wavFile'.
        fclose(wavFile);

        // 16-bit/mono error message.
        if (wav_hdr.numChannels != 1 || wav_hdr.bitsPerSample != 16) {
            std::cout << "\nERROR: File is not mono/16-bit.\n" << std::endl;
            continue; // If the file is stereo, throw an error:
        }
        
        std::cout << "\nFile read successful.\n" << std::endl;
        cmd.printHdr(headerSize, wavFile); // File header information is printed.
        
        /// FUNCTION SECTION
        /// This part of the code will contain the different forms of delay line/sample interpolation.
        /// This includes a 0 order hold, linear, and bicubic solutions, as well as others.
        /// It also includes solutions and simulations of doppler provided by the STK and SAL libraries, linked below.
        /// WAVs with the various different solutions will be printed to the build folder.
        
        // Firstly, establish a read and write pointer, a buffer size, and a speed for the read pointer to shift at.
        int readPtr = 0, buffer = 1024, frame = 800; // 48000Hz/60fps.
        float writePtr = 0.0, speed = 1.6f, maxDelayTime = 0.1f, c = 343.0f, delay = 1000.0f; // Will set this to different speeds for each page of the test.
        int16_t* dopplerData = new(std::nothrow) int16_t[numOfSamples];
        int16_t* newAudioData = new(std::nothrow) int16_t[numOfSamples];
        
        // Zero hold
        for (int i = 0; i < numOfSamples; i++) {
        

            // Apply the delay to the audio data
            dopplerData[i] = audioData[std::max(i - std::floor(delay), 0)];

            // Update the data
            newAudioData[i] = audioData[i] = dopplerData[i];
        }
        
        ///
        ///
        ///
        
        
        // tempnote: THIS WAV SECTION NEEDS TO BE PUT INTO A FUNCTION. WORK OUT WHY ITS GOING WRONG
        /*---WRITE---*/
        std::string newFilePath = input.substr(0, input.length() - 4);
        FILE* newFile;
        newFile = fopen(newFilePath.c_str(), "wb");
        fwrite(&wav_hdr, 1, headerSize, newFile);
        fseek(newFile, headerSize, SEEK_SET);
        fwrite(newAudioData, 2, numOfSamples, newFile);
        fclose(newFile);
        std::cout << "\nNew WAVs created.\n" << std::endl;
    }
}
