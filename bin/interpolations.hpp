///
/// interpolations.hpp
/// Variable Delay Line Project
///
/// Created by Alex Sedman on 20/02/2023.
///

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#ifndef interpolations_hpp
#define interpolations_hpp

class interpolate {
public:
    // Interpolation methods.
    void unfiltered(); // No interpolation.
    void zeroOrderHold(); // 0 order
    void nearestNeighbour(); // 0 order
    void linear(); // 1 order
    void quadratic(); // 2 order
    void cubic(); // 3 order
    void sinc(); // Complex order
};

#endif /* interpolations_hpp */
