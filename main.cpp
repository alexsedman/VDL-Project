//
// main.cpp
// Variable Delay Line (VDL) Project
//
// Created by Alex Sedman on 12/01/2023.
//
// This program requires minimal robustness here - the purpose of this program is to provide a way to generate new WAVs with the implementation of variable delays, using various-order interpolation techniques. It is a closed project, thus does not require debugging.
//

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
    int headerSize = sizeof(wavHdr), numOfSamples;
    std::string input;
    const char* filePath;
    int16_t* audioData;
    FILE* wavFile;
    
    while (true) {
        input = cmd.mainMenu(input); // Calls main menu.
        
        if (input == "-quit") {
            std::cout << "\nBye!\n" << std::endl;
            return 0; // Quits program.
            
        } else if (input == "-info") {
            cmd.infoMenu(); // Calls info menu.
            continue;
        }
    
        filePath = input.c_str(); // Sets the file path to the user input.
        wavFile = fopen(filePath, "rb"); // Attempts to open the inputted file.
        
        if (wavFile == nullptr) {
            std::cout << "\nERROR: File can't be read/input is invalid.\n" << std::endl;
            continue; // If the input is invalid (i.e. 'fopen' returns a null pointer), then this error is thrown and the loop is skipped.
        } else {
            
            /*---READ---*/
            fread(&wavHdr, 1, headerSize, wavFile); // Header information is read from 'wavFile'.
            fseek(wavFile, headerSize, SEEK_SET); // File pointer is set to the start of the audio stream.
            numOfSamples = wavHdr.dataSize / 2;
            int16_t* audioData = new(std::nothrow) int16_t[numOfSamples]; // A pointer to a dynamic array is created; the audio stream will be stored here.
            fread(audioData, 2, numOfSamples, wavFile); // Audio stream is read from 'wavFile'.
            fclose(wavFile); // Close the original file.
            
            if (wavHdr.numChannels != 1 || wavHdr.bitsPerSample != 16) {
                std::cout << "\nERROR: The inputted file is not a mono, 16-bit file! Please only supply this type of file.\n" << std::endl;
                continue; // If the file is stereo, throw an error:
            } else {
                std::cout << "\nFile read successful!\n" << std::endl;
            }

            input = cmd.optionsMenu(input); // Calls the options menu.
            
            if (input == "1") {
                edit.printHdr(headerSize, wavFile); // File information is printed.
                fclose(wavFile);
                continue; // Skips the write phase of the program, as it is not needed for this option.
            } else if (input == "2") {
                edit.changeSampleRate(input); // Change sample rate.
            } else if (input == "3") {
                edit.addPause(audioData, numOfSamples); // Adds a pause.
            } else if (input == "4") {
                edit.normalise(audioData, numOfSamples); // Normalises the audio.
            } else if (input == "5") {
                edit.reverse(audioData, numOfSamples); // Reverses the audio.
            } else {
                std::cout << "\nERROR: Invalid option selection. Returning to the main menu.\n" << std::endl; // Error if invalid input is selected.
                continue;
            }
            
            std::string newFilePath = cmd.filenameMenu(input, filePath); // Asks user for new filename.
            
            /*---WRITE---*/
            FILE* newFile;
            newFile = fopen(newFilePath.c_str(), "wb"); // Open the new file in write binary mode.
            fwrite(&wavHdr, 1, headerSize, newFile); // Write header.
            fseek(newFile, headerSize, SEEK_SET); // Set file pointer to the start of the audio stream.
            fwrite(audioData, 2, numOfSamples, newFile); // Write audio data.
            fclose(newFile); // Close file.
        }
        std::cout << std::endl; // Console new line.
    }
}
