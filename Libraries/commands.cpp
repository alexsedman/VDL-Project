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
#include "commands.hpp"
#include "interpolate.hpp"

/*----------MAIN MENU----------*/
// Function to output main menu and return the user input.
std::string commands::mainMenu(std::string input) {
    using namespace std;
    
    //Print the main menu.
    cout << "---MAIN MENU---" << endl;
    cout << "Enter the pathname for the mono WAV file that you would like to edit." << endl;
    cout << "Pathname: ";
    
    getline(cin, input); // Gets user input.
    return input;
}

/*----------OPTIONS MENU----------*/
// Function to output options once file has been read.
std::string commands::optionsMenu(std::string input) {
    using namespace std;
    
    //Print the option menu.
    cout << "---OPTIONS---" << endl;
    // Options here
    cout << "- 1: Default Doppler (Enzo's version)." << endl;
    cout << "- 2: Standford variable delay." << endl;
    cout << "- 3: 0th to multiple order interpolation (Multiple WAVs)." << endl;
    cout << "- Enter anything else to return to the main menu." << endl;
    cout << "Please input your choice: ";
    
    getline(cin, input); // Gets user input.
    return input;
}

/*----------NEW FILENAME----------*/
// Gets a file name from user for the new WAV and converts this input to a new file path.
std::string commands::filenameMenu(std::string input, std::string filePath) {
    std::string newFilePath;
    std::cout << "\n---FILENAME---" << std::endl;
   
    // This while loop demands a valid filename input from the user.
    while (true) {
        std::cout << "Filename: ";
        std::getline(std::cin, input); // Gets user input.
        
        // Parity checks: Make sure the requirements for a legal filename are met, as outlined above by the console output.
        if ((input.find(':') != std::string::npos) || (input.find('/') != std::string::npos) || (input.length() == 0) || (input.at(0) == '.') || (input.length() > 32)) {
            std::cout << "ERROR: Invalid filename." << std::endl;
            continue;
        }
        
        newFilePath = input.std::string::append(".wav"); // Appends '.wav' to user input.
        
        // The following checks whether the inputted filepath already exists, and throws an error if it does.
        FILE* testFile;
        testFile = fopen(newFilePath.c_str(), "rb+");
        if (testFile) {
            std::cout << "ERROR: File already exists!" << std::endl;
            fclose(testFile);
            continue;
        }
        break;
    }
    return newFilePath;
}

/*----------PRINT WAV HEADER----------*/
// Function to print WAV header.
void commands::printHdr(int headerSize, FILE* wavFile) {
    using namespace std;
    commands hdr;
    //Print the header.
    cout << endl;
    cout << "---OPTION 1: PRINT---" << endl;
    cout << "Chunk ID (File Spec): " << wav_hdr.RIFF[0] << wav_hdr.RIFF[1] << wav_hdr.RIFF[2] << wav_hdr.RIFF[3] << endl;
    cout << "Chunk Size: " << wav_hdr.fileSize << endl;
    cout << "File Type (FourCC Tag): " << wav_hdr.WAVE[0] << wav_hdr.WAVE[1] << wav_hdr.WAVE[2] << wav_hdr.WAVE[3] << endl;
    
    cout << "Subchunk 1 ID (Format): " << wav_hdr.fmt[0] << wav_hdr.fmt[1] << wav_hdr.fmt[2] << wav_hdr.fmt[3] << endl;
    cout << "Subchunk Size: " << wav_hdr.fmtSize << endl;
    cout << "Audio Format: " << wav_hdr.fmtType << endl;
    cout << "Number of Channels: " << wav_hdr.numChannels << endl;
    cout << "Sample Rate: " << wav_hdr.sampleRate << endl;
    cout << "Data Rate (Bps): " << wav_hdr.byteRate << endl;
    cout << "Block Align: " << wav_hdr.blockAlign << endl;
    cout << "Bits Per Sample: " << wav_hdr.bitsPerSample << endl;
    
    cout << "Subchunk 2 ID (Data): " << wav_hdr.DATA[0] << wav_hdr.DATA[1] << wav_hdr.DATA[2] << wav_hdr.DATA[3] << endl;
    cout << "Subchunk 2 Size: " << wav_hdr.dataSize << endl << endl;
}
