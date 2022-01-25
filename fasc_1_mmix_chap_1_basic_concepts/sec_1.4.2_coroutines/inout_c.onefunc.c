// inout_c.onefunc.c

////////////////////////////////////////////////////////////////////////
// MIT License
//
// Copyright (c) 2020-2022 Zartaj Majeed
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////

// Section 1.4.2' Coroutines, Fascicle 1, MMIX

// sample input on stdin: a2b5e3426fg0zyw3210pq89r.
// sample output on stdout: abb bee eee e44 446 66f gzy w22 220 0pq 999 999 999 r.

#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

static void usage()
{
  puts("usage: inout_c.onefunc <in.txt >out.txt");
}

// print groups of 3 characters separated by space
// print newline after 16 groups
void print_char(char chr, unsigned count)
{
  if(count > 0 && count % 3 == 0) {
    char separator = count % 48 == 0? '\n': ' ';
    putchar(separator);
  }
  putchar(chr);
}

static void InNOut()
{
  char InBuf[1000] = "";
  char* inptr = InBuf;
  const char Period = '.';
  char inchar;
  
  int repeatCount = -1;
  unsigned outCount = 0;

  for(;;) {

// fill buffer
    if(*inptr == '\0') {
      inptr = InBuf;
      if(fgets(InBuf, sizeof InBuf, stdin) == NULL)
        *inptr = Period;
    }

    inchar = *inptr;
    ++inptr;

// skip whitespace and control characters
    if(isspace(inchar) || iscntrl(inchar))
      continue;

// finish processing on input terminator
    if(inchar == Period) {
      puts(".\n");
      break;
    }

// single digit is repeat count of following character
    if(isdigit(inchar) && repeatCount < 0) {
      sscanf(&inchar, "%1d", &repeatCount);
      continue;
    }

// emit repeats
    for(; repeatCount > 0; --repeatCount) {
      print_char(inchar, outCount);
      ++outCount;
    }

// emit the character
    print_char(inchar, outCount);
    ++outCount;

// reset repeat count without checking for unrepeated case
    repeatCount = -1;
  }

}

int main(int argc, char* argv[])
{
  (void)argv;

  if(argc > 1) {
    usage();
    exit(0);
  }

  InNOut();

  return 0;
}
