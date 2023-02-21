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
#include "interpolations.hpp"

/*----------MAIN MENU----------*/
// Function to output main menu and return the user input.
std::string commands::mainMenu(std::string inputName) {
    using namespace std;
    
    //Print the main menu.
    cout << "---MAIN MENU---" << endl;
    cout << "WAV Pathname (16-bit mono only): ";
    
    getline(cin, inputName); // Gets user input.
    return inputName;
}

/*----------PRINT WAV HEADER----------*/
// Function to print WAV header.
void commands::printHdr(FILE* wavFile) {
    using namespace std;
    
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

std::string commands::rename(std::string inputName, std::string filterType) {
    std::string outputName = inputName;
    outputName.erase (outputName.end()-4, outputName.end());
    outputName.append(filterType);
    outputName.append(".wav");
    return outputName;
}

/*---READ---*/
void commands::read(FILE* wavFile) {
    fread(&wav_hdr, 1, data.headerSize, wavFile); // Header information is read from 'wavFile'.
    fseek(wavFile, data.headerSize, SEEK_SET); // File pointer is set to the start of the audio stream.
    fread(data.inputStream, 2, data.numOfSamples, wavFile); // Audio stream is read from 'wavFile'.
    fclose(wavFile);
}

/*---WRITE---*/
// Function to write to file.
void commands::write(std::string newFilePath) {
    FILE *newFile;
    newFile = fopen(newFilePath.c_str(), "wb");
    fwrite(&wav_hdr, 1, data.headerSize, newFile);
    fseek(newFile, data.headerSize, SEEK_SET);
    fwrite(data.outputStream, 2, data.numOfSamples, newFile);
    fclose(newFile);
    std::cout << "New WAV '" << newFilePath << "' created." << std::endl;
}

/*---RESET---*/
// Resets the buffer, and any other values, ready for a different interpolation method.
void commands::reset() {
    for (int i = 0; i < data.bufferLen; i++) {
        data.buffer[i] = NULL;
    }
}
