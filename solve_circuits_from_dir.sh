#!/bin/bash

WORKDIR=$(pwd)
EXECDIR=$WORKDIR/build/

cd $1
for FILE in *; do
  answer=$($WORKDIR/build/solveCircuitSAT -i $FILE --time)
  echo $FILE: $answer;
done