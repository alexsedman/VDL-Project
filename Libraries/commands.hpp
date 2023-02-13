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
    uint8_t RIFF[4] = {'R', 'I', 'F', 'F'};
    uint32_t fileSize = 882036; // This field needs to be updated later to reflect the actual size of the file
    uint8_t WAVE[4] = {'W', 'A', 'V', 'E'};

    // fmt subchunk
    uint8_t fmt[4] = {'f', 'm', 't', ' '};
    uint32_t fmtSize = 16; // This is the size of the rest of the fmt subchunk (not including the identifier or the size field)
    uint16_t fmtType = 1; // Set to 1 for PCM format
    uint16_t numChannels = 1; // Number of channels, for example 1 for mono
    uint32_t sampleRate = 44100; // Sample rate in Hz, for example 44100 Hz
    uint32_t byteRate = 44100 * 2; // Calculated as sampleRate * numChannels * bitsPerSample / 8
    uint16_t blockAlign = 2; // Calculated as numChannels * bitsPerSample / 8
    uint16_t bitsPerSample = 16; // Number of bits per sample, for example 16 for 16-bit audio

    // Data subchunk
    uint8_t DATA[4] = {'d', 'a', 't', 'a'};
    uint32_t dataSize = 10 * 44100 * 2; // 882000
} wav_hdr;


class commands {
public:
    struct WAV_HEADER wavHdr; // WAV header data structure.
    
    // General functions.
    void mainMenu(); // Main Menu.
    void writeFile(int headerSize, int16_t *newAudioData, int numOfSamples, std::string newFilePath); // Write.
};

#endif
