# Create build directory and navigate into it
mkdir build
cd build

# Run CMake to configure and generate the build system with cache variables
cmake -A x64 -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=Release `
      -DCMAKE_PREFIX_PATH="C:\Qt\6.7.1\msvc2019_64" `
      -DOpenCV_DIR="C:\Program Files\opencv\build" `
      -DOpenVINO_DIR="C:\Program Files (x86)\Intel\OpenVINO\runtime\cmake" `
      ..

# Build the project
cmake --build . --config Release --parallel %NUMBER_OF_PROCESSORS% --verbose
