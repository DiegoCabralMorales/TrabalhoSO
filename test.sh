#!/bin/bash

# Compile the C++ file with ncurses
g++ *.cpp -lncursesw

if [ $? -eq 0 ]; then
    "./a.out"
fi
