@echo on

mkdir build

cmake -B build -S %SRC_DIR% -G "Ninja" -DCMAKE_INSTALL_PREFIX=%LIBRARY_PREFIX% -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS:bool=off
cmake --build build --config Release
cmake --install build --prefix %LIBRARY_PREFIX%