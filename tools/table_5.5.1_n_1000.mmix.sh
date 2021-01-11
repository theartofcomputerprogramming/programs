#!/bin/bash

# table_5.5.1_n_1000.mmix.sh

# generates data of 1000 values used in table 1 from section 5.5 of the mmix supplement for taocp vol 3

# The case N = 1000 refers to the sequence K_1, K_2,..., K_1000 of 32-bit keys defined by
#    X_0 = 0
#    X_(n+1) = (6364136223846793005 * X_n + 9754186451795953191) mod 2^64
#    K_n = floor(X_n/2^32)
# For the multiplier, see Section 3.3.4, page 108; 9754186451795953191 is some random increment value

# numbers are too big for bash, use bc or awk

bc <<EOF

# insure integer mod
scale = 0

two_32 = 2^32
two_64 = 2^64

x = 0
for(n = 0; n < 1000; ++n) {
  x = (6364136223846793005 * x + 9754186451795953191) % two_64
  k = x / two_32
  print k, "\n"
}

EOF

