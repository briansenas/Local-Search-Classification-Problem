#!/bin/bash
if [ $# -lt 1 ]
  then
    echo "[ERROR]: You must specify a {seed} and if you want to shuffle the data [0-1]"
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

echo "[START-1]: Doing 1NN search in ionosphere.arrf"
echo "[START-1]: Doing 1NN search in ionosphere.arrf" > ../results/1NN_results.txt
./../bin/Fast1NN ../datos/ionosphere.arff $1 $2 >> ../results/1NN_results.txt
echo "[START-2]: Doing 1NN search in parkinsons.arrf"
echo "[START-2]: Doing 1NN search in parkinsons.arrf" >> ../results/1NN_results.txt
./../bin/Fast1NN ../datos/parkinsons.arff $1 $2 >> ../results/1NN_results.txt
echo "[START-3]: Doing 1NN search in spectf-heart.arrf"
echo "[START-3]: Doing 1NN search in spectf-heart.arrf" >> ../results/1NN_results.txt
./../bin/Fast1NN ../datos/spectf-heart.arff $1 $2 >> ../results/1NN_results.txt

