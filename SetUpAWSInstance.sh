#!/bin/bash

sudo snap install emacs --classic
sudo snap install cmake --classic
sudo apt install g++ make -y
mkdir build
cd build
sudo cmake ..
sudo make
