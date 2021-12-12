# Configure the build
cmake -S . -B build/Release -D CMAKE_BUILD_TYPE=Release

# Actually build the binaries
cmake --build build/Release