# Configure the build
cmake -S . -B build/Debug -D CMAKE_BUILD_TYPE=Debug

# Actually build the binaries
cmake --build build/Debug