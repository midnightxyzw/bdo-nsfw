@echo off
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
copy build\Release\ice_decipher.dll ice_decipher.dll
rm -rf build