#!/bin/bash

# texttobinary.sh

# echo "0,257,9,2,3,7,7,5,5,8,8,6,4,6,1,3,7,4,9,5,2,8,0,0"|
# texttobinary.sh |
# od -An -td4 -w4 -v

function usage {
  echo "usage: texttobinary.sh [-w width] [-e endian] < in.txt > out.dat"
  echo "converts comma-separated numbers on stdin to binary data on stdout"
  echo "-w: width in bytes of binary word, default is 4"
  echo "-e: endian format big|little, default is little"
  echo "in.txt: numbers separated by commas without whitespace"
}

while getopts "e:hw:" opt; do
  case $opt in
    e) endian=$OPTARG;;
    h) usage; exit 0;;
    w) width=$OPTARG;;
    *) usage; exit 1
  esac
done
shift $((OPTIND-1))

: ${width:=4} ${endian:=little}

[[ $endian != big && $endian != little ]] && usage && exit 1

let len=2*width

while IFS=, read -a vals; do
  for((i=0; i < ${#vals[*]}; ++i)); do
    printf -vhex "%.${len}x" ${vals[i]}
    if [[ $endian == little ]]; then
      for((j=0; j < ${#hex}; j+=2)); do
        str+="\\x${hex: -j-2:2}"
      done
    elif [[ $endian == big ]]; then
      for((j=0; j < ${#hex}; j+=2)); do
        str+="\\x${hex:j:2}"
      done
    fi
  done
done

echo -en "$str"

