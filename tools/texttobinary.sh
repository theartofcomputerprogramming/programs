#!/bin/bash

# texttobinary.sh

function usage {
  echo "usage: texttobinary.sh [-d] [-w width] [-e endian] < in.txt > out.dat"
  echo "converts comma-separated numbers on stdin to binary data on stdout"
  echo "-d: debug print string of hex escapes"
  echo "-w: width in bytes of binary word, default is 4"
  echo "-e: endian format big|little, default is little"
  echo "in.txt: numbers separated by commas or whitespace"
  echo "examples:"
  echo
  echo "texttobinary.sh -d < <(echo -e \"9, 2, 3, -7\n-5 100 1024\")"
  echo "\x09\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\xf9\xff\xff\xff\xfb\xff\xff\xff\x64\x00\x00\x00\x00\x04\x00\x00"
  echo
  echo "echo -e \"9, 2, 3, -7\n-5 100 1024\" | texttobinary.sh | od -An -td4 -w4 -v"
}

while getopts "de:hw:" opt; do
  case $opt in
    d) debug=true;;
    e) endian=$OPTARG;;
    h) usage; exit 0;;
    w) width=$OPTARG;;
    *) usage; exit 1
  esac
done
shift $((OPTIND-1))

: ${width:=4} ${endian:=little} ${debug:=false}

[[ $endian != big && $endian != little ]] && usage && exit 1

let len=2*width

while IFS+=, read -a vals; do
  for((i=0; i < ${#vals[*]}; ++i)); do
    val=${vals[i]}

# printf output is 64-bit bigendian
# printf "0x%.2x" -7 prints 0xfffffffffffffff9
    printf -vhex "%.${len}x" $val

# truncate sign extension from right for 64-bit negative numbers past maximum length eg -7 should be 0xfffffff9 for 4 bytes
    if ((val < 0)); then
      hex=${hex: -len}
    fi

# reverse bytes for littleendian
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

# prints hex escaped string for debugging
if $debug; then
  echo "$str"
  exit 0
fi

# print binary data
echo -en "$str"

