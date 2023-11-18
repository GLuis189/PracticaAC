#!/bin/sh

. /etc/profile
module avail
module load gcc/12.1.0
echo "*** BASE ***"
perf stat  build/fluid/fluid_exec 1000 large.fld out-base.fld