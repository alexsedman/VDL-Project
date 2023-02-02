//
// main.cpp
// Variable Delay Line Project
//
// Created by Alex Sedman on 12/01/2023.
//
// test change for github commit
//
// NOTE: program requires minimal robustness here - the purpose of this program is to provide a way to generate new WAVs with the implementation of variable delays, using various-order interpolation techniques. It is a closed project, thus does not require debugging.

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

/* Function to print WAV header.
void print_header() {
    using namespace std;
    //Print the header.
    cout << "Chunk ID (File Spec): " << wav_hdr.RIFF[0] << wav_hdr.RIFF[1] << wav_hdr.RIFF[2] << wav_hdr.RIFF[3] << endl;
    cout << "Chunk Size (File Size): " << wav_hdr.file_size << endl;
    cout << "File Type (FourCC Tag): " << wav_hdr.WAVE[0] << wav_hdr.WAVE[1] << wav_hdr.WAVE[2] << wav_hdr.WAVE[3] << endl;
    
    cout << "Subchunk 1 ID (Format): " << wav_hdr.fmt[0] << wav_hdr.fmt[1] << wav_hdr.fmt[2] << wav_hdr.fmt[3] << endl;
    cout << "Subchunk Size: " << wav_hdr.fmt_len << endl;
    cout << "Audio Format: " << wav_hdr.fmt_type << endl;
    cout << "Number of Channels: " << wav_hdr.no_channels << endl;
    cout << "Sample Rate: " << wav_hdr.sample_rate << endl;
    cout << "Data Rate (Bps): " << wav_hdr.byte_rate << endl;
    cout << "Block Align: " << wav_hdr.block_align << endl;
    cout << "Bits Per Sample: " << wav_hdr.sample_len << endl;
    
    cout << "Subchunk 2 ID (Data): " << wav_hdr.DATA[0] << wav_hdr.DATA[1] << wav_hdr.DATA[2] << wav_hdr.DATA[3] << endl;
    cout << "Sub-Chank2 Size: " << wav_hdr.data_size << endl;
}
*/

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
    
    // The inputted file is
    FILE* wav_file;
    wav_file = fopen(file_path, "rb+"); // 'rb+' is a binary read/update parameter.

    //Read the header
    size_t bytes_read = fread(&wav_hdr, 1, hdr_size, wav_file); // An unsigned integer 'bytes_read' is assigned the value using the 'fread' function,
    
    fclose(wav_file);
    return 0;
}
