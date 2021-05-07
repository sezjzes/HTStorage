#!/bin/bash

# Run the cleanup script.
sudo ./CleanUpForNextRun.sh

# Clean up and redo the build.
sudo rm -r -f build/
mkdir build
cd build
sudo cmake ..
sudo make
