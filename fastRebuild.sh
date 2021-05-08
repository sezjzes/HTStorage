#!/bin/bash

# Run the cleanup script.
sudo ./CleanUpForNextRun.sh

# Clean up and redo the build.
cd build
sudo cmake ..
sudo make
