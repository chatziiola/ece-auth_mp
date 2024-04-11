#include "test-hash.c"
#include <stdio.h>


int main()
{
  char myString[50];
  printf("Hello!\n");
  printf("Give me a string\n");
  scanf("%s", myString);
  printf("hash is %d", hashfunc(myString));
}
