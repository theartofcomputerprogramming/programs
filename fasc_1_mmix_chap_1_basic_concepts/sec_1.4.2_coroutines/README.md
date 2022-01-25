# Section 1.4.2' Coroutines

## The decoding problem

In order to study coroutines in action, let us consider a contrived example. Suppose we want to write a program that translates one code into another. The input code to be translated is a sequence of 8-bit characters terminated by a period, such as

                          a2b5e3426fg0zyw3210pq89r.  (1)

This code appears on the standard input file, interspersed with whitespace characters in an arbitrary fashion. For our purposes a "whitespace character" will be any byte whose value is less than or equal to #20, the ASCII code for ' '. All whitespace characters in the input are ignored; the other characters should be interpreted as follows, when they are read in sequence:

(1) If the next character is one of the decimal digits 0 or 1 or ... or 9, say n, it indicates (n + 1) repetitions of the following character, whether the following character is a digit or not.

(2) A nondigit simply denotes itself. The output of our program is to consist of the resulting sequence separated into groups of three characters each, until a period appears; the last group may have fewer than three characters. For example, (1) should be translated into

               abb bee eee e44 446 66f gzy w22 220 0pq 999 999 999 r. (2)

Notice that 3426f does not mean 3427 repetitions of the letter f; it means 4 fours and 3 sixes followed by f. If the input sequence is '1.', the output is simply '.', not '..', because the first period terminates the output. The goal of our program is to produce a sequence of lines on the standard output file, with 16 three-character groups per line (except, of course, that the final line might be shorter). The three-character groups should be separated by blank spaces, and each line should end as usual with the ASCII newline character #a.

## Programs

[**`inout_c.onefunc.c:`**](inout_c.onefunc.c) A non-coroutine solution to the problem in C.

[**`inout_c.coroutinestruct.c:`**](inout_c.coroutinestruct.c) A C program that simulates coroutines using a structure based on `coroutine_struct` from Knuth's MMIX simulator.

[**`inout_cplusplus.cpp:`**](inout_cplusplus.cpp) A C++ program that uses C++20 coroutines.

## Test data

Test data is in (data/inout)
