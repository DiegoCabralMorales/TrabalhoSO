#!/bin/bash

# Compile the C++ file with ncurses
g++ *.cpp -lncurses

if [ $? -eq 0 ]; then
    "./a.out"
fi
