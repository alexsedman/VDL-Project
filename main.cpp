//
// main.cpp
// Variable Delay Line Project
//
// Created by Alex Sedman on 12/01/2023.
//
// This program requires minimal robustness here - the purpose of this program is to provide a way to generate new WAVs with the implementation of variable delays, using various-order interpolation techniques. It is a closed project, thus does not require debugging.
//
// Test file pathname: /Users/alexsedman/Downloads/fragmentary sample v2.wav

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>

// A data structure is defined for the WAV header.
struct WAV_HEADER {
    // RIFF Chunk Descriptor
    uint8_t RIFF[4];
    uint32_t file_size;
    uint8_t WAVE[4];

    // fmt subchunk
    uint8_t fmt[4];
    uint32_t fmt_len;
    uint16_t fmt_type;
    uint16_t no_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t sample_len;
    
    // Data subchunk
    uint8_t DATA[4];
    uint32_t data_size;
} wav_hdr;

int main() {
    // Variables declared: pointer to the filepath name and an input string.
    int hdr_size = sizeof(wav_hdr); // A variable to measure the size of the WAV header (this should be equivalent to 44 bytes).
    const char* file_path;
    std::string input;
    
    // User is asked to input the file location of the desired WAV file.
    std::cout << "Input wave file name: ";
    getline(std::cin, input);
    file_path = input.c_str();
    // Here, the filename is separated into characters and stored as a data stream in the memory. the variable 'file_path' points to the beginning of the data stream, which can be referenced when opening the file.
    
    // The inputted file is allocated to the memory as a stream.
    FILE* wav_file;
    wav_file = fopen(file_path, "rb+"); // 'rb+' is a binary read/update parameter.

    // Read the header.
    size_t bytes_read = fread(&wav_hdr, 1, hdr_size, wav_file); // An unsigned integer 'bytes_read' is assigned the value using the 'fread' function,
    
    // cout checks for the WAV.
    std::cout << "WAV header memory stream pointer: " << &wav_file << std::endl;
    std::cout << "Header bytes read: " << bytes_read << std::endl;
    std::cout << "File size: " << wav_hdr.file_size << std::endl;
    std::cout << "Size of data stream (excluding header: " << wav_hdr.data_size << std::endl;
    
    /* Stanford Variable Delay code below:
    static double A[N];
    static double *rptr = A; // read ptr
    static double *wptr = A; // write ptr

    double setdelay(int M) {
        rptr = wptr - M;
        while (rptr < A) {
            rptr += N
        }
    }

    double delayline(double x) {
        double y;
        A[wptr++] = x;
        y = A[rptr++];
        if ((wptr-A) >= N) {
            wptr -= N
        }
        if ((rptr-A) >= N) {
            rptr -= N
        }
        return y;
    }
    */
    
    fclose(wav_file);
    return 0;
}
