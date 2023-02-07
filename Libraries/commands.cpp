//
// commands.hpp
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

/*----------MAIN MENU----------*/
// Function to output main menu and return the user input.
std::string commandLib::mainMenu(std::string input) {
    using namespace std;
    
    //Print the main menu.
    cout << "---MAIN MENU---" << endl;
    cout << "Enter the pathname for the mono WAV file that you would like to edit." << endl;
    cout << "Pathname: ";
    
    getline(cin, input); // Gets user input.
    return input;
}

/*----------INFO MENU----------*/
// Function to print info section.
void commandLib::infoMenu() {
    using namespace std;
    //Print info.
    cout << endl;
    cout << "---INFO---" << endl;
    cout << "This program can read a .wav file, perform some basic processed to the audio data and write the resulting audio data to a new .wav file. Created by Alex Sedman." << endl;
    cout << "At the main menu, please find a pathname for a mono WAV file that you would like to use. For example, an example pathname could be '/Users/username/Documents/testwav.wav'" << endl;
    cout << "You can also enter '-quit' to end the program, or 'info' to bring this menu up again!" << endl;
    cout << endl;
}

/*----------OPTIONS MENU----------*/
// Function to output options once file has been read.
std::string commandLib::optionsMenu(std::string input) {
    using namespace std;
    
    //Print the option menu.
    cout << "---OPTIONS---" << endl;
    cout << "- Enter '1' to read the WAV header info." << endl;
    cout << "- Enter '2' to change the sample rate (CREATES A NEW FILE)." << endl;
    cout << "- Enter '3' to insert a pause (CREATES A NEW FILE)." << endl;
    cout << "- Enter '4' to peak normalise (CREATES A NEW FILE)." << endl;
    cout << "- Enter '5' to add a mystery audio effect (CREATES A NEW FILE)." << endl;
    cout << "- Enter anything else to return to the main menu." << endl;
    cout << "Please input your choice: ";
    
    getline(cin, input); // Gets user input.
    return input;
}

/*----------NEW FILENAME----------*/
// Gets a file name from user for the new WAV and converts this input to a new file path.
std::string commandLib::filenameMenu(std::string input, std::string filePath) {
    std::string newFilePath;
    std::cout << "\n---OPTIONS---" << std::endl;
    std::cout << "Please select a new filename." << std::endl;
    std::cout << "NOTE: Files can not: \n- Contain a colon (':') \n- Contain a forward slash ('/') \n- Start with a period ('.') \n- Be more than 32 characters long \n- Be an empty input\nThe extension '.wav' will be automatically appended." << std::endl;
    std::cout << "The new WAV will be created in the build folder." << std::endl;
   
    // This while loop demands a valid filename input from the user.
    while (true) {
        std::cout << "Filename: ";
        std::getline(std::cin, input); // Gets user input.
        
        // Parity checks: Make sure the requirements for a legal filename are met, as outlined above by the console output.
        if ((input.find(':') != std::string::npos) || (input.find('/') != std::string::npos) || (input.length() == 0) || (input.at(0) == '.') || (input.length() > 32)) {
            std::cout << "ERROR: Invalid filename." << std::endl;
            continue;
        }
        
        newFilePath = input.std::string::append(".wav"); // Appends '.wav' to user input.
        
        // The following checks whether the inputted filepath already exists, and throws an error if it does.
        FILE* testFile;
        testFile = fopen(newFilePath.c_str(), "rb+");
        if (testFile) {
            std::cout << "ERROR: File already exists!" << std::endl;
            fclose(testFile);
            continue;
        }
        break;
    }
    return newFilePath;
}
