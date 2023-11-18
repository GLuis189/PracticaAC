#!/bin/sh

. /etc/profile
module avail
module load gcc/12.1.0
cmake --version
cmake -S PracticaAc -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build