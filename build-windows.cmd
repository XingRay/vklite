mkdir build_windows && cd build_windows

cmake -G "Visual Studio 17 2022" -A x64 ..

cmake --build . --config Release

cmake --install . --prefix dist/windows