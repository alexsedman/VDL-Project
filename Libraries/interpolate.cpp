//
// interpolate.hpp
// VDL Project
//
// Created by Alex Sedman.
//

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include "commands.hpp"
#include "interpolate.hpp"

/*----------(1) PRINT WAV HEADER----------*/
// Function to print WAV header.
void wavEdit::printHdr(int headerSize, FILE* wavFile) {
    using namespace std;
    commandLib hdr;
    //Print the header.
    cout << endl;
    cout << "---OPTION 1: PRINT---" << endl;
    cout << "Chunk ID (File Spec): " << wavHdr.RIFF[0] << wavHdr.RIFF[1] << wavHdr.RIFF[2] << wavHdr.RIFF[3] << endl;
    cout << "Chunk Size: " << wavHdr.fileSize << endl;
    cout << "File Type (FourCC Tag): " << wavHdr.WAVE[0] << wavHdr.WAVE[1] << wavHdr.WAVE[2] << wavHdr.WAVE[3] << endl;
    
    cout << "Subchunk 1 ID (Format): " << wavHdr.fmt[0] << wavHdr.fmt[1] << wavHdr.fmt[2] << wavHdr.fmt[3] << endl;
    cout << "Subchunk Size: " << wavHdr.fmtSize << endl;
    cout << "Audio Format: " << wavHdr.fmtType << endl;
    cout << "Number of Channels: " << wavHdr.numChannels << endl;
    cout << "Sample Rate: " << wavHdr.sampleRate << endl;
    cout << "Data Rate (Bps): " << wavHdr.byteRate << endl;
    cout << "Block Align: " << wavHdr.blockAlign << endl;
    cout << "Bits Per Sample: " << wavHdr.bitsPerSample << endl;
    
    cout << "Subchunk 2 ID (Data): " << wavHdr.DATA[0] << wavHdr.DATA[1] << wavHdr.DATA[2] << wavHdr.DATA[3] << endl;
    cout << "Subchunk 2 Size: " << wavHdr.dataSize << endl << endl;
}

/*----------(2) CHANGE SAMPLE RATE----------*/
// Changes sample rate of the WAV file according to user input.
void wavEdit::changeSampleRate(std::string input) {
    std::cout << "\n---OPTION 2: CHANGE SAMPLE RATE---" << std::endl;
    std::cout << "Please choose a new sample rate between 1-192000Hz." << std::endl;
    
    while (true) {
        std::cout << "New sample rate: ";
        getline(std::cin, input); // Gets user input.
        try {
            uint32_t sampleRange = std::stoul(input); // Tries to convert the input to an unsigned 32-bit integer.
            if (sampleRange < 1 || sampleRange > 192000) {
                throw std::out_of_range("Value is outside of allowed range (1-192000Hz)"); // Throws an error if the input is out of range.
            }
        } catch (const std::invalid_argument) {
            std::cout << "ERROR: Invalid input." << std::endl; // This error is thrown if the input is not valid (i.e. not assignable as an integer).
            continue;
        } catch (const std::out_of_range) {
            std::cout << "ERROR: Value out of range! (1-192000Hz)" << std::endl; // Range error message.
            continue;
        }
        break;
    }
    std::cout << "\nSample rate changed!" << std::endl;
    wavHdr.sampleRate = std::stoul(input); // Sample rate changed.
}

/*----------(3) ADD PAUSE----------*/
void wavEdit::addPause(int16_t* audioData, int numOfSamples) {
    int pauseStart = ((numOfSamples/2) + (numOfSamples/100)), pauseEnd = ((numOfSamples/2) + (numOfSamples/30)); // Start and end samples of pause are defined.
    for (int i = pauseStart; i <= pauseEnd; i++) {
        audioData[i] = 0; // Pause created. Values between the start and end point are converted to 0, creating a gap.
    }
    std::cout << "\nPause added!" << std::endl;
}

/*----------(4) NORMALISE----------*/
void wavEdit::normalise(int16_t* audioData, int numOfSamples) {
    int16_t maxValue = audioData[0]; // Max value set.
      for (int i = 1; i < numOfSamples; i++) {
        if (abs(audioData[i]) > abs(maxValue)) {
          maxValue = audioData[i]; // For each value in the data loop, check whether it is larger than max value, and if so, reassign it as the max value.
        }
      }
    
    // Normalize the data using a for loop.
    for (int i = 0; i < numOfSamples; i++) {
        audioData[i] = (audioData[i] * -32767) / maxValue; // This maths calculation sets the max value sample to the max 16-bit value.
      }
    std::cout << "\nAudio normalised!" << std::endl;
}

/*----------(5) FILTER----------*/
// Reverses the data stream.
void wavEdit::reverse(int16_t* audioData, int numOfSamples) {
    int start = 0, end = numOfSamples - 1;
    // The following while loop swaps the audio samples from back to front, by flipping them until the while loop meets in the middle.
    while (start < end) {
        int temp = audioData[start]; // A temp placeholder variable is used to store samples starting from the beginning.
        audioData[start] = audioData[end]; // The beginning audio samples are replaced with the ending audio samples.
        audioData[end] = temp; // The end audio samples are replaced with the beginning audio samples (from the placeholder variable).
        start++; // Next beginning sample, moving forwards.
        end--; // Next ending sample, moving backwards.
    }
    std::cout << "\nAudio Reversed!" << std::endl;
}
