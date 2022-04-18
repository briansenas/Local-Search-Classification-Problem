#!/bin/bash
if [ $# -lt 1 ]
  then
    echo "[ERROR]: You must specify a {seed} and if you want to shuffle the data [0-2]"
    exit
fi
if [ -z "$1" ]
  then
    echo "[ERROR]: Couldn't read the seed value for some reason"
    exit
fi

if [ -z "$2" ]
  then
    echo "[ERROR]: Couldn't read the shuffle value for some reason"
    exit
fi


./run1NN.sh $1 $2
./runGreedy.sh $1 $2
./runLSAlg.sh $1 $2
