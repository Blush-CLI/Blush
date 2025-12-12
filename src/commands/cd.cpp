/*
 File: src/commands/cd.cpp
 Created: [12.12.2025]
 Command: cd
 Description: Changing directory
 Part-of: built-in
*/

#include <iostream>
#include <filesystem>
#include "../includes/cmd.h"
#include "../includes/bcolors.h"

void c_cd(const std::vector<std::string>& args) {
    if(args.empty()){
        setColor(Color::Red);
        std::cout << "Please provide a path! <e.g: .., someFolder>\n";
        setColor(chosenColor);
        return;
    }

    try {
        std::filesystem::current_path(args[0]);
    } catch(const std::filesystem::filesystem_error& e){
        setColor(Color::Red);
        std::cout << "Error:" << e.what() << "\n";
        setColor(chosenColor);
    }
}
