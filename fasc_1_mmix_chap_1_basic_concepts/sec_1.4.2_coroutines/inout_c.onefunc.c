// inout_c.onefunc.c

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
