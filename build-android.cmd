mkdir build_android && cd build_android

cmake -G "Ninja" -DCMAKE_TOOLCHAIN_FILE="D:/develop/android/android-sdk-windows/ndk/29.0.13599879/build/cmake/android.toolchain.cmake" -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-26 -DCMAKE_BUILD_TYPE=Release ..

cmake --build . --config Release

cmake --install . --prefix dist/android