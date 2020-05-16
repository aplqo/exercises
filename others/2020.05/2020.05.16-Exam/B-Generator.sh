#!/bin/bash

for ((i=$1;$i<$2;i++))
do
    echo "$i" > ./bdata/$i.in
    ./B-GetAns ./bdata/$i.in ./bdata/$i.ans
done
