# Binance
This library has been forked from the official binance library available at https://github.com/binance-exchange/binacpp.git.  
We converting the library to c++ 17 and we're going to implement new features.
The work is still in progress, if you wanna contribute, contact me on linkedin https://www.linkedin.com/in/benedetto-l-1b9b0815a

## steps to install cmake 3.23:

- sudo apt-get install build-essentials libssl-dev
- wget https://github.com/Kitware/CMake/releases/download/v3.23.0-rc1/cmake-3.23.0-rc1.tar.gz
- tar -zxvf cmake-3.23.0-rc1.tar.gz
- cd cmake-3.23.0-rc1
- sudo ./bootstrap
- make
- sudo make install
- sudo ln -s /usr/local/bin/cmake /usr/bin/cmake
- cmake --version

## step to build and install the library:

- mkdir build_dynamic
- cd build_dynamic
- cmake ../Sources/ -DBUILD_SHARED_LIBS=ON
- cmake --build .
- cmake --install .
