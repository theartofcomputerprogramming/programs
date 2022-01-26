// inout_cplusplus.cpp

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

#include <stdio.h>

#include <coroutine>
#include <string>

using namespace std;

static void usage()
{
  puts("usage: inout_cplusplus <in.txt >out.txt");
}

// terminate and output a line
static void finishLine(char* line, uint64_t len)
{
  line[len - 2] = '\n';
  line[len - 1] = '\0';
  fputs(line, stdout);
}

// return next character from input stream
// whitespace and control characters are skipped
static char NextChar()
{
// process input stream in chunks of 100 bytes
  static char InBuf[100];

// position of next character to process
  static char* inptr = InBuf;
  const char Period = '.';
  char inchar;
  
  for(;;) {

// refill buffer if empty
    if(*inptr == '\0') {
      inptr = InBuf;
      if(fgets(InBuf, sizeof InBuf, stdin) == NULL)
// set input terminator if no more input
        *inptr = Period;
    }

// examine current character
    inchar = *inptr;
    ++inptr;

// ignore whitespace and control characters in input
    if(!isspace(inchar) && !iscntrl(inchar))
      break;
  }

  return inchar;
}

// return object for In() coroutine
struct InG {
  struct Promise;
  using promise_type = Promise;
  coroutine_handle<Promise> coro;

  InG(coroutine_handle<Promise> h): coro(h) {}

  ~InG() {
    if(coro)
      coro.destroy();
  }

  struct Promise {
// last value yielded by coroutine
    char val;

    InG get_return_object() {
      return InG{coroutine_handle<Promise>::from_promise(*this)};
    }

// In() starts out suspended
    suspend_always initial_suspend() noexcept {
      return {};
    }

// store yielded value and suspend the coroutine
    suspend_always yield_value(char x) {
      val = x;
      return {};
    }

    void return_void() {}

    suspend_always final_suspend() noexcept {
      return {};
    }

    void unhandled_exception() noexcept {}

  };

// takes awaiting code directly to await_resume because In() computes result synchronously
  bool await_ready() const noexcept {
    return true;
  }

// skipped because await_ready returns true
  void await_suspend(coroutine_handle<>) {}

// resume own coroutine In() to compute and return result
  char await_resume() const noexcept {
    coro.resume();
// value to yield will have been updated by yield_value
    return coro.promise().val;
  }

};

// return object for Out() coroutine
struct OutG {
  struct Promise;
  using promise_type = Promise;
  coroutine_handle<Promise> coro;

  OutG(coroutine_handle<Promise> h): coro(h) {}

  ~OutG() {
    if(coro)
      coro.destroy();
  }

  struct Promise {

    OutG get_return_object() {
      return OutG{coroutine_handle<Promise>::from_promise(*this)};
    }

// program starts with Out() executing
    suspend_never initial_suspend() noexcept {
      return {};
    }

    void return_void() {}

    suspend_always final_suspend() noexcept {
      return {};
    }

    void unhandled_exception() noexcept {}

  };

};

InG In()
{
// suspension point 0
// suspended by initial_suspend
// resumed by own await_resume
  char inchr;
  int count;

  for(;;) {

    inchr = NextChar();

// yield nondigit character
    if(inchr > '9') {

// suspension point 1
// suspended by suspend_always returned by yield_value
// control transfers to resumer in own await_resume
      co_yield inchr;
      continue;
    }

    count = inchr - '0';

// yield nondigit character
    if(count < 0) {
// suspension point 2
      co_yield inchr;
      continue;
    }

// count has numeric value of digit character
// get next character to repeat count times
    inchr = NextChar();

// suspension point 3
    co_yield inchr;

// repeat current character
    for(--count; count >= 0; --count) {
// suspension point 4
      co_yield inchr;
    }

  }

}

// takes In coroutine as parameter
OutG Out(InG&& In)
{
// suspension point 0
// not suspended by initial_suspend

  char OutBuf[] = {0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, '\n', 0};
  char* outptr = OutBuf;

// obtain a group of 3 characters, one at a time, from In coroutine
// terminate each group with space
// emit lines of 16 groups, terminating 16th group with newline
// terminate output with period
  for(;;) {

// get first character of group of 3
// suspension point 1
// not suspended
// waits synchronously for result
    char outchr = co_await In;

    outptr[0] = outchr;
// no more input, emit last line
    if(outchr == '.') {
      finishLine(OutBuf, (uint64_t)(outptr + 3 - OutBuf));
      break;
    }

// get second character of group of 3
// suspension point 2
    outchr = co_await In;

    outptr[1] = outchr;
// no more input, emit last line
    if(outchr == '.') {
      ++outptr;
      finishLine(OutBuf, (uint64_t)(outptr + 3 - OutBuf));
      break;
    }

// get third character of group of 3
// suspension point 3
    outchr = co_await In;

    outptr[2] = outchr;
// no more input, emit last line
    if(outchr == '.') {
      outptr += 2;
      finishLine(OutBuf, (uint64_t)(outptr + 3 - OutBuf));
      break;
    }

// add group terminator character, space or newline
    outptr += 4;

// keep going if line has room for another group
    if(outptr != OutBuf + sizeof(OutBuf) - 1) {
      continue;
    }

// emit line full of 16 groups
    fputs(OutBuf, stdout);
    outptr = OutBuf;

  }

}

int main(int argc, char* argv[])
{
  (void)argv;

  if(argc > 1) {
    usage();
    return 1;
  }

// In is initially suspended
// Out is initially not suspended
  Out(move(In()));

}
