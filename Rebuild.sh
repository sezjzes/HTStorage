#!/bin/bash

sudo rm -r -f build/
mkdir build
cd build
sudo cmake ..
sudo make
