if exist build\ rmdir /s /q build
mkdir build
cd build
cmake .. -G "Unix Makefiles"
make
make test ARGS="-V"
cd ..