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
#ifndef commands_hpp
#define commands_hpp

/*----------WAV HEADER STRUCT----------*/
// A data structure is defined for the WAV header.
inline struct WAV_HEADER {
    // RIFF Chunk Descriptor
    uint8_t RIFF[4]; // RIFF chunk: "RIFF" in ASCII
    uint32_t fileSize; // Total size of the RIFF chunk in bytes
    uint8_t WAVE[4]; // 4-byte format identifier: "WAVE" in ASCII

    // fmt subchunk
    uint8_t fmt[4]; // 4-byte identifier of the format chunk: "fmt " in ASCII
    uint32_t fmtSize; // Total size of the format chunk in bytes
    uint16_t fmtType; // Audio format code, usually set to 1 (PCM)
    uint16_t numChannels; // Number of channels in the audio data
    uint32_t sampleRate; // Sample rate of the audio data in Hz
    uint32_t byteRate; // The byte rate of the audio data
    uint16_t blockAlign; // The block align of the audio data
    uint16_t bitsPerSample; // The number of bits per sample, usually set to 16 or 24
    
    // Data subchunk
    uint8_t DATA[4]; // 4-byte identifier of the data chunk: "data" in ASCII
    uint32_t dataSize; // Total size of the data chunk in bytes
} wav_hdr;

class commands {
public:
    struct WAV_HEADER wavHdr;
    // General functions.
    std::string mainMenu(std::string input); // Main Menu.
    void printHdr(int headerSize, FILE* wavFile); // Prints header.
};

#endif
