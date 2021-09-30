#!/bin/bash +x

rm -f ./player*
mpirun --hostfile ./host -np 3 ./build/openmpi10_11

echo "<<<<<cat player1 info>>>>>"
cat ./player1.txt

echo "<<<<<cat player2 info>>>>>"
cat ./player2.txt
