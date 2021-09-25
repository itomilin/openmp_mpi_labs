#!/bin/bash +x

rm -f ./player*
mpirun --hostfile ./host -np 3 ./build/openmpi10_11

