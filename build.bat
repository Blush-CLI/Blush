@echo off

mkdir build
cd build
cmake ..
cmake --build . --parallel 8
echo "Build complete!"