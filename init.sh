#! /bin/bash

# insert plunge
rm -r plunge
git clone https://github.com/MeiK-h/plunge.git plunge_tmp
cd plunge_tmp/src
make
cd ../..
mv plunge_tmp/src/plunge ./
rm -r plunge_tmp

# init env
rm -r work
mkdir work