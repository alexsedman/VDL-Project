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

/*----------MAIN MENU----------*/
// Function to output main menu and return the user input.
std::string commands::mainMenu(std::string input) {
    using namespace std;
    
    //Print the main menu.
    cout << "---MAIN MENU---" << endl;
    cout << "WAV Pathname (16-bit mono only): ";
    
    getline(cin, input); // Gets user input.
    return input;
}
