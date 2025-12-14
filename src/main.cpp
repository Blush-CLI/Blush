#include <print>
#include <filesystem>

int main() {
    // FIRST: we have to make a skeleton (tree) how the structure of files and folders will look

    /*
    CMakeLists.txt
    src/
      core/
        - 
      commands/
        -
      utils/
        - 
      - main.cpp
      - 
    */
    std::print("{}\n> ", std::filesystem::current_path().filename().string());
    return 0;
}