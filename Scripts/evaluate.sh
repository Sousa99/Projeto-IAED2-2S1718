#!/bin/bash

set -e

gcc -Wall -ansi -pedantic -g -o ../Projeto *.c

. Exemplos/run_examples.sh
. Testes\ Publicos/
. all.sh ../Projeto