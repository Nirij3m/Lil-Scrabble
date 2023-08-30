cmake_minimum_required(VERSION 3.18)
project(Lil_Scrabble C)

set(CMAKE_C_STANDARD 99)

add_executable(Lil_Scrabble main.c src/nodetrie.c src/nodetrie.h src/scrabble.c src/scrabble.h)
