#!/bin/sh

# Getting dependencies for the compiler
sudo apt-get install flex
sudo apt-get install bison

# Installs gcc, g++ and make
sudo apt-get install build-essential

# Getting dependencies for the google test framework
sudo apt-get update
sudo apt-get install libgtest-dev
