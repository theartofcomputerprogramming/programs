#!/bin/bash

# program_m_find_the_maximum.test.sh

# test for program_m_find_the_maximum.c
# usage: program_m_find_the_maximum.test.sh

set -o pipefail

let n=10

if ((n < 0 || n > 255)); then
  echo >&2 "error: n $n does not fit in one byte"
  exit 1
fi

outfile=testdata.$$.dat

echo "test pid $$ program_m_find_the_maximum $n"
seq $n |
sort -R |
while IFS= read -r i; do
  printf -vhex "%.2x" $i
  printf "\x00\x00\x00\x00\x00\x00\x00\x$hex"
done |
tee $outfile |
program_m_find_the_maximum $n |
od -An -td8 -w8 --endian=big |
tr -d ' '

es=$?
echo "test run es $es"
((es)) && exit $es

echo -n "$outfile: "
od -An -td8 -w8 --endian=big $outfile |
tr -d ' ' |
nl |
sed -nE '/\t'$n'$/ { s/^\s+//; s/\t/ /; p; q }'

if ((es == 0)); then
  rm $outfile
fi
