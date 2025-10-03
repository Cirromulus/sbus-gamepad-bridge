#!/bin/bash
set -e

make -C build
less build/*.elf.map | grep \\.stack
cat ./build/CMakeFiles/*.dir/src/main.cpp.su
sudo picotool load build/*.uf2 -fx
