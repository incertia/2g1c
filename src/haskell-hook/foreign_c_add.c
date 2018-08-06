#include <stdio.h>

int
foreign_add(int a, int b)
{
  int c = a + b;
  printf("%d + %d = %d\n", a, b, c);
  return c;
}
