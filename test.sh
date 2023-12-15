#!/bin/bash

# Compile the C++ file with ncurses and C++20
g++ -std=c++20 *.cpp -lncurses

if [ $? -eq 0 ]; then
    "./a.out"
fi
