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


echo "[START-1]: Doing local search in ionosphere.arrf"
echo "[START-1]: Doing local search in ionosphere.arrf" > ../results/LSalg_results.txt
./../bin/LSalg ../datos/ionosphere.arff b g $1 $2>>../results/LSalg_results.txt
echo "[START-2]: Doing local search in parkinson.arrf"
echo "[START-2]: Doing local search in parkinson.arrf" >> ../results/LSalg_results.txt
./../bin/LSalg ../datos/parkinsons.arff 1 2 $1 $2>>../results/LSalg_results.txt
echo "[START-3]: Doing local search in spectf-heart.arrf"
echo "[START-3]: Doing local search in spectf-heart.arrf" >> ../results/LSalg_results.txt
./../bin/LSalg ../datos/spectf-heart.arff 1 2 $1 $2>>../results/LSalg_results.txt

