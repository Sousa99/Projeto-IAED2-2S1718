#!/bin/bash

set -e

gcc -Wall -ansi -pedantic -g -o ./Projeto *.c
valgrind --tool=memcheck --leak-check=yes ./Projeto