#!/bin/bash

# Install the dependencies.
sudo snap install emacs --classic
sudo snap install cmake --classic
sudo apt install g++ make -y

# Build the working directory.
mkdir workingDir
cd workingDir
mkdir execution storage1 storage2
cd ..

# Build the client workspaces and files.
cd tests/files/
mkdir {0..10}
for dir in */; do
    cp testfile3.txt testfile4.txt $dir
done
cd ../..

# Initiate the build.
mkdir build
cd build
sudo cmake ..
sudo make
cd ..

# Make the other bash scripts executable.
sudo chmod +x Rebuild.sh
sudo chmod +x CleanUpForNextRun.sh
