// inout_c.coroutinestruct.c

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

#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

static void usage()
{
  puts("usage: inout_c.coroutinestruct <in.txt >out.txt");
}

// all states needed for coroutines to suspend and resume
typedef enum {
  In1,
  In2,
  Out1,
  Out2,
  Out3,
  Out4
} State;

typedef struct coroutine {
// queue of coroutines to run
  struct coroutine* next;
// coroutine state to resume
  State state;
// variables to persist across coroutine suspensions
  void* stateVars;
// value yielded by coroutine on suspend
  void* yieldValue;
} coroutine;

// terminate line with newline and null and print
void finishLine(char* line, uint64_t len)
{
  line[len - 2] = '\n';
  line[len - 1] = '\0';
  fputs(line, stdout);
}

// return one valid character from input
// valid characters are non-control characters including whitespace, any above \x20
// returns period "." on eof or any read error
static char NextChar()
{
  static char InBuf[1000];
  static char* inptr = InBuf;
  const char Period = '.';
  char inchar;
  
  for(;;) {

    if(*inptr == '\0') {
      inptr = InBuf;
      if(fgets(InBuf, sizeof InBuf, stdin) == NULL)
        *inptr = Period;
    }

    inchar = *inptr;
    ++inptr;

    if(!isspace(inchar))
      break;
  }

  return inchar;
}

typedef struct InStateVars {
  char inchr;
  int count;
} InStateVars;

typedef struct OutStateVars {
  char OutBuf[16*4 + 1];
  char* outptr;
} OutStateVars;

static void runCoroutines()
{
  InStateVars inVars;
  OutStateVars outVars = {
// 15 space-terminated groups, 1 newline-terminated group, 1 terminating null for entire line
// cannot portably use bigendian mmix trick of uint32_t words initialized to single space because space ends up as first character in littleendian
    .OutBuf = {0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, '\n', 0}
  };

  coroutine In = {NULL, In1, &inVars, malloc(sizeof ((InStateVars*)NULL)->inchr)};
  coroutine Out = {NULL, Out1, &outVars, NULL};

  for(coroutine* coro = &Out; coro != NULL; coro = coro->next) {
    switch(coro->state) {

// coroutine In
      {
// local variables for In states
// can only declare not initialize inside switch
        InStateVars* vars;
        char inchr;
        int count;

        case In1:
          vars = In.stateVars;
          inchr = NextChar();
// yield nondigit character
          if(inchr > '9') {
            In.next = &Out;
            *(char*)In.yieldValue = inchr;
            break;
          }
          count = inchr - '0';
// yield nondigit character
          if(count < 0) {
            In.next = &Out;
            *(char*)In.yieldValue = inchr;
            break;
          }

// count has numeric value of digit character
// so get next character to repeat count times
          inchr = NextChar();
          In.next = &Out;
          *(char*)In.yieldValue = inchr;
          In.state = In2;
          vars->inchr = inchr;
          vars->count = count;
          break;

// state for repeating current character
        case In2:
          vars = In.stateVars;
          --vars->count;
          if(vars->count >= 0) {
            In.next = &Out;
            *(char*)In.yieldValue = vars->inchr;
            In.state = In2;
            break;
          }
// all done repeating, time for new character
          In.next = &In;
          In.state = In1;
          break;
      }

// coroutine Out
      {
// local variables for Out states
// can only declare not initialize inside switch
        OutStateVars* vars;
        char outchr;

        case Out1:
          vars = Out.stateVars;
          vars->outptr = vars->OutBuf;
          Out.next = &In;
          Out.state = Out2;
          break;

// state for storing first character of group of 3
        case Out2:
          vars = Out.stateVars;
          outchr = *(char*)In.yieldValue;
          vars->outptr[0] = outchr;
          if(outchr == '.') {
            finishLine(vars->OutBuf, vars->outptr + 3 - vars->OutBuf);
            Out.next = NULL;
            break;
          }
          Out.next = &In;
          Out.state = Out3;
          break;

// state for storing second character of group of 3
        case Out3:
          vars = Out.stateVars;
          outchr = *(char*)In.yieldValue;
          vars->outptr[1] = outchr;
          if(outchr == '.') {
            ++vars->outptr;
            finishLine(vars->OutBuf, vars->outptr + 3 - vars->OutBuf);
            Out.next = NULL;
            break;
          }
          Out.next = &In;
          Out.state = Out4;
          break;

// state for storing third character of group of 3
        case Out4:
          vars = Out.stateVars;
          outchr = *(char*)In.yieldValue;
          vars->outptr[2] = outchr;
          if(outchr == '.') {
            vars->outptr += 2;
            finishLine(vars->OutBuf, vars->outptr + 3 - vars->OutBuf);
            Out.next = NULL;
            break;
          }

// skip space character that ends group of 4
          vars->outptr += 4;

          if(vars->outptr != vars->OutBuf + sizeof(vars->OutBuf) - 1) {
            Out.next = &In;
            Out.state = Out2;
            break;
          }
          fputs(vars->OutBuf, stdout);
          Out.next = &Out;
          Out.state = Out1;
          break;

      }
    }
  }

}

int main(int argc, char* argv[])
{
  (void)argv;

  if(argc > 1) {
    usage();
    exit(0);
  }

  runCoroutines();

  return 0;
}
