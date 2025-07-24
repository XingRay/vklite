
cmake -S . -B build -G "Ninja" -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=build/dist -DCMAKE_TOOLCHAIN_FILE="D:/develop/android/android-sdk-windows/ndk/29.0.13599879/build/cmake/android.toolchain.cmake" -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-26

cmake --build ./build --config Release

cmake --install ./build --config Release
