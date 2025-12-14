#pragma once
#include <string>


class Shell {
public:
    /**
      * Main Shell class - contains all abstracted functions to run and use Blush
     */
    Shell();

    /**
      * Run the shell
      * @returns `void`
     */
    void run();
    
    /**
      * Shell cleanup and pre-exit state
     */
    ~Shell();
private:
    std::string fullWorkingPath;
    // we'll put some state here later
};