#!/bin/bash

# gendata.sh

# numbers are too big for bash, use bc or awk

function usage {
  echo "usage: gendata.sh -d dataset [-n count] [-h]"
  echo "generates various datasets by name"
  echo
  echo "-d: dataset name"
  echo "-c: number of values to generate"
  echo "supported datasets"
  echo "taocp_table_551_mmix, default count 1000"
  echo "taocp_table_551_original, default count 1000"
}

# generates data of 1000 values used in table 1 from section 5.5 of the mmix supplement for taocp vol 3
# The case N = 1000 refers to the sequence K_1, K_2,..., K_1000 of 32-bit keys defined by
#    X_0 = 0
#    X_(n+1) = (6364136223846793005 * X_n + 9754186451795953191) mod 2^64
#    K_n = floor(X_n/2^32)
# For the multiplier, see Section 3.3.4, page 108; 9754186451795953191 is some random increment value

function taocp_table_551_mmix {

  local n=$1
  bc <<EOF

# insure integer mod
scale = 0

two_32 = 2^32
two_64 = 2^64

x = 0
for(n = 0; n < $n; ++n) {
  x = (6364136223846793005 * x + 9754186451795953191) % two_64
  k = x / two_32
  print k, "\n"
}

EOF

}

# generates data of 1000 values used in table 1 from section 5.5 of taocp vol 3
# data is generated by the formula:
#  K_1001 = 0
#  K_(n-1) = (3141592621 * K_n + 2113148651) mod 10^10, for n = 1001,...,2

# numbers are too big for bash, use bc or awk

function taocp_table_551_original {

  local count=$1
  bc <<EOF

# insure integer mod
scale = 0

ten_10 = 10^10

count = $count
k[count + 1] = 0
for(n = count + 1; n >= 2; --n) {
  k[n - 1] = (3141592621 * k[n] + 2113148651) % ten_10
}

for(i = 1; i <= $count; ++i) {
  print k[i], "\n"
}

EOF

}

# main

while getopts "d:h" opt; do
  case $opt in
    d) dataset=$OPTARG;;
    n) n=$OPTARG;;
    h) usage; exit 0;;
    *) usage; exit 1
  esac
done
shift $((OPTIND-1))

[[ -z $dataset ]] && usage && exit 1

case $dataset in
  taocp_table_551_mmix)
    taocp_table_551_mmix ${n:=1000}
    ;;
  taocp_table_551_original)
    taocp_table_551_original ${n:=1000}
esac
