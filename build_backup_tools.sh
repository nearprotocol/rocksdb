#!/bin/bash
export DEBUG=0
export PORTABLE=1
j=$((`nproc` - 1))
if (($j == 0)); then j=1; fi
make -j $j static_lib
cd examples
make backup
make restore
cd ..