#!/bin/bash

# Clean up the client workspaces and files.
mkdir temp
cp tests/files/testfile3.txt tests/files/testfile4.txt temp
sudo rm -r -f tests/files/*
sudo mv temp/* tests/files/
sudo rm -r -f temp

# Rebuild the client workspaces and files.
cd tests/files/
mkdir {0..10}
for dir in */; do
    cp testfile3.txt testfile4.txt $dir
done
cd ../..

# Clean up the working directory.
rm -r -f workingDir/execution1/* workingDir/execution2/* workingDir/storage1/* workingDir/storage2/* workingDir/storage3/* workingDir/storage4/*
