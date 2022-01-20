#!/bin/bash
make
echo


./check tests/p1testinput1.txt
echo
./check tests/p1testinput2.txt
echo
./check tests/p1testinput3.txt
echo
./check tests/p1testinput4.txt
echo
./check tests/p1testinput5.txt


echo
./advance -m 999 tests/testmoves1.txt
echo
./advance -m 15 tests/testmoves1.txt
echo
./advance -m 15 tests/testmoves1.txt -o testoutput.txt
echo
./advance < tests/testmoves2.txt
echo
cat tests/testmoves2.txt | ./advance -m 0
echo
./advance tests/testmoves2.txt -m 1 -x -o testoutput.txt