// polynomial.c

// Algorithm A (Addition of polynomials), 2.2.4 Circular Lists,
// The Art of Computer Programming, Donald Knuth

// Program A (Addition of polynomials), 2.2.4 Circular Lists,
// The MMIX Supplement, Martin Ruckert

// input format for each polynomial is sequence of terms terminated by single int64_t zero
// each term is int64_t coefficient followed by single int64_t for the exponents of the four variables in the term
// each exponent uses 2 bytes except the first exponent which is allowed only 15 bits of the 2 high bytes excluding the high sign bit

#include "polynomial.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void polynomial_init(struct Term* p)
{
  p->ABC = -1;
  p->COEF = 0;
  p->LINK = NULL;
}

// not as nice as the simple erase described in TAOCP 2.2.4 because each node is separately malloced
void polynomial_clear(struct Term* P)
{

  for(struct Term* p = P->LINK; p->ABC != -1;) {
    struct Term* prev = p;
    p = p->LINK;
    free(prev);
  }

}

void polynomial_write(const struct Term* P, FILE* out)
{
  for(const struct Term* p = P->LINK; p->ABC != -1; p = p->LINK) {
    fwrite(&p->COEF, sizeof(p->COEF), 1, out);
    fwrite(&p->ABC, sizeof(p->ABC), 1, out);
  }

  uint64_t zero = 0;
  fwrite(&zero, sizeof(zero), 1, out);
}

// each polynomial is sequence of pairs of int64_t, first is coefficient, second is exponents of the variables, sequence is terminated with single zero
// count is buffer size, returned with number of items processed
// complete is true when terminating zero for complete polynomial is found
// new terms are appended to tail, and tail is updated
static int load_polynomial(const int64_t* buffer, size_t* count, struct Term** tail, bool* complete)
{
  if(!buffer || !tail || !*tail || !count || !complete)
    return -1;

  const uint32_t n = *count;
  *complete = false;

  if(n == 0) {
    return 0;
  }

  uint32_t i;
  for(i = 0; i < n; i += 2) {

// found polynomial terminator
    if(buffer[i] == 0) {
      *count = i + 1;
      *complete = true;
      return 0;
    }

// need pair of values for new term
    if(i + 1 == n) {
      *count = i;
      return 0;
    }

    struct Term* term = malloc(sizeof(*term));
    if(!term) {
      *count = i;
      fprintf(stderr, "error: malloc failure\n");
      return -1;
    }

// set coefficient and exponents fields of new term
// setting the link is not necessary here since only caller can complete the circular list
    term->COEF = buffer[i];
    term->ABC = buffer[i + 1];
    term->LINK = NULL;

// append to tail and update tail
    (*tail)->LINK = term;
    *tail = term;

  }

  *count = i;

  return 0;
}

int get_polynomials(FILE* in, struct Term* polys[], uint32_t npolys)
{

  if(!in)
    return -1;

#define BUFSZ 64
  int64_t buffer[BUFSZ];

  ssize_t unprocessed_vals = 0;
  size_t processed_vals = 0;
  size_t copied_vals = 0;

  for(int i = 0; i < npolys; ++i) {

    struct Term* tail = polys[i];
    bool complete = false;

    while(!complete) {

// refill buffer
      if(unprocessed_vals == 0) {

        if(feof(in)) {
          fprintf(stderr, "error: unexpected EOF\n");
          return -2;
        }

        const size_t nread = fread(buffer + copied_vals, sizeof(*buffer), BUFSZ - copied_vals, in);

        if(ferror(in)) {
          fprintf(stderr, "error: read error\n");
          return -3;
        }

        unprocessed_vals = copied_vals + nread;
        processed_vals = 0;
        copied_vals = 0;
      }

      size_t n = unprocessed_vals;

// n is passed number of unprocessed values and holds processed count on return
      if(load_polynomial(buffer + processed_vals, &n, &tail, &complete) != 0)
        return -4;

      processed_vals += n;
      unprocessed_vals -= n;

// move unprocessed values to start of buffer to make room for more data because polynomial is still incomplete
      if(!complete && unprocessed_vals != 0) {
        memcpy(buffer, buffer + processed_vals, unprocessed_vals * sizeof(*buffer));
        copied_vals = unprocessed_vals;
        unprocessed_vals = 0;
      }

    }

// last term should point to sentinel node
    if(tail->LINK != polys[i])
      tail->LINK = polys[i];

  }

  return 0;
}

