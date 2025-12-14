@echo off

mkdir build
cd build
cmake ..
cmake --build .
echo "Build complete!"