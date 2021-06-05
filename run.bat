@echo off
g++ -c *.cpp
g++ *.o -o Main -lsfml-graphics -lsfml-window -lsfml-system
Main.exe