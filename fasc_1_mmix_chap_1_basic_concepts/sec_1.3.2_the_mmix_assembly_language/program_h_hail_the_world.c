// program_h_hail_the_world.c

// Program H (Hail the world)
// Section 1.3.2' The MMIX Assembly Language, Fascicle 1, MMIX

#include <stdio.h>

int main(int argc, char* argv[])
{

  const char* String = ", world\n";

  fputs(argv[0], stdout);
  fputs(String, stdout);

  return 0;
}

