#!/bin/bash

sudo snap install emacs --classic
sudo snap install cmake --classic
sudo apt install g++
mkdir build
cd build
cmake ..
make
