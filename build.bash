#!/bin/bash
bison -d -o build/parser.cpp parser.ypp && 
flex -o build/lexer.c lexer.l &&
clang++ -std=c++11 build/parser.cpp build/lexer.c -o lib/langd
