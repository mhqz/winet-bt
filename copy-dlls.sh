#!/bin/bash

# Msys2
cp ./build/{gcrypt,gpg_error}/out/{lib,bin}/*dll* ./build/
cp ./build/*dll* ./build/test/

# Clion
cp ./cmake-build-debug/{gcrypt,gpg_error}/out/{lib,bin}/*dll* ./cmake-build-debug/
cp ./cmake-build-debug/*dll* ./cmake-build-debug/test/
