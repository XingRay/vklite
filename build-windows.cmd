
cmake -S . -B build -G "Visual Studio 17 2022" -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=build/dist

cmake --build ./build --config Release

cmake --install ./build --config Release
