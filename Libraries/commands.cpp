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

/*---MAIN MENU---*/
// Function to output main menu and return the user input.
void commands::mainMenu() {
    using namespace std;
    
    //Print the main menu.
    cout << "---MAIN MENU---" << endl;
    cout << "Program starting, generating WAVs..." << endl;
}

/*---WRITE---*/
void commands::writeFile(int headerSize, int16_t *newAudioData, int numOfSamples, std::string newFilePath) {
    FILE *newFile;
    newFile = fopen(newFilePath.c_str(), "wb");
    fwrite(&wav_hdr, 1, headerSize, newFile);
    fseek(newFile, headerSize, SEEK_SET);
    fwrite(newAudioData, 2, numOfSamples, newFile);
    fclose(newFile);
    std::cout << "\nNew WAV '" << newFilePath << "' created.\n" << std::endl;
}
