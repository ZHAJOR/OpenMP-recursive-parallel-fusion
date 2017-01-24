#! /usr/bin/env sh

num=1000000
while test $num != 21000000
do
    echo $num
    cat values/$num | ./d2s --time --check
    cat values/$num | ./d2p 12 --time --check
    cat values/$num | ./d2p 24 --time --check
    num=$((num+1000000))
done
