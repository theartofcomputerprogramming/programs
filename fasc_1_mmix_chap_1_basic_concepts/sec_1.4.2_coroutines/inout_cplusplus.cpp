// inout_cplusplus.cpp

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
static char NextChar()
{
  static char InBuf[100];
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
OutG Out(InG&& in)
{
// suspension point 0
// not suspended by initial_suspend

  char OutBuf[] = {0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, ' ', 0, 0, 0, '\n', 0};
  char* outptr = OutBuf;

  for(;;) {

// suspension point 1
// not suspended
// waits synchronously for result
    char outchr = co_await in;

    outptr[0] = outchr;
    if(outchr == '.') {
      finishLine(OutBuf, (uint64_t)(outptr + 3 - OutBuf));
      fprintf(stderr, "Out2: done OutBuf \"%s\"\n", OutBuf);
      break;
    }

// suspension point 2
    outchr = co_await in;

    outptr[1] = outchr;
    if(outchr == '.') {
      ++outptr;
      finishLine(OutBuf, (uint64_t)(outptr + 3 - OutBuf));
      break;
    }

// suspension point 3
    outchr = co_await in;

    outptr[2] = outchr;
    if(outchr == '.') {
      outptr += 2;
      finishLine(OutBuf, (uint64_t)(outptr + 3 - OutBuf));
      break;
    }

    outptr += 4;

    if(outptr != OutBuf + sizeof(OutBuf) - 1) {
      continue;
    }

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
