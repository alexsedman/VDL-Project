///
/// interpolate.hpp
/// VDL Project
///
/// Created by Alex Sedman.
///

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#ifndef interpolate_hpp
#define interpolate_hpp

class interpolate {
public:
    // WAV file functions.
    
    /*---INTERPOLATION FUNCTIONS---*/
    void SALdoppler(std::string input); // Enzo's Doppler model.
    void STKdoppler(std::string input); // STK Doppler model - this is likely not required depending on the contents of the library.
};

#endif
