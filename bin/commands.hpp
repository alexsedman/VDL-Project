#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#ifndef commands_hpp
#define commands_hpp

/*---WAV HEADER STRUCT---*/
// A data structure is defined for the WAV header.
inline struct WAV_HEADER {
    // RIFF Chunk Descriptor
    uint8_t RIFF[4] = {'R', 'I', 'F', 'F'};
    uint32_t fileSize = 1764036; // This field needs to be updated later to reflect the actual size of the file
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
    uint32_t dataSize = 20 * 44100 * 2; // 882000
} wav_hdr;

/*---DOPPLER PROPERTIES---*/
// A data structure for the test values.
// NOTE: TO REMOVE AND REPLACE WITH BUFFER/DATASTREAM PROPERTIES INSTEAD.
inline struct INIT_VALS {
    double dist = 88200; // Start distance between pointers.
    double vel = -0.2; // Read pointer velocity.
    double accel = 0.000002; // -0.000001
} init;

/*---DATA INF---*/
// A data structure containing data and buffer stream information.
inline struct DATA_INF {
    int headerSize = sizeof(wav_hdr), numOfSamples = wav_hdr.dataSize / 2;
    
    int16_t *inputStream = new(std::nothrow) int16_t[numOfSamples];
    int16_t *outputStream = new(std::nothrow) int16_t[numOfSamples];
    
    const int bufferLen = 2000000;
    int16_t *buffer = new(std::nothrow) int16_t[bufferLen]; // Buffer array of length 'bufferLen' created in the memory.
} data_inf;

class commands {
public:
    // General functions.
    std::string mainMenu(std::string input); // Main Menu.
    void printHdr(FILE* wavFile); // Prints header.
    void read(FILE* wavFile);
    void write(std::string newFilePath); // Write.
    std::string rename(std::string input, std::string filterType);
    void reset(); // Reset the buffer.
};

#endif
