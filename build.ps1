mkdir build
cd build
cmake -A x64 -G "Visual Studio 16 2019" ..
cmake --build . --config Release --parallel 6 --verbose