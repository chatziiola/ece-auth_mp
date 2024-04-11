// #define DEBUG 

#if defined(DEBUG)
#include <stdio.h>
#endif

int hashfunc(char inputString[]) {
#if defined(DEBUG)
    printf("\tStarting hashfunc with %s\n", inputString);
#endif
  // Verified proper length
  int values[] = {10, 42, 12, 21, 7, 5,  67, 48, 69, 2, 36, 3,  19,
                  1,  14, 51, 71, 8, 26, 54, 75, 15, 6, 59, 13, 25};
  int hash = 0;
  
  for (int ind = 0; inputString[ind]!='\0'; ind++) {
    // ASCII:
    // - Caps: 64-91 (non-inclusive ranges here)
    // - Lower: 96 - 123 (non-inclusive)
    // - Digits: 47-58 (non-inclusive)
#if defined(DEBUG)
    printf("\tAt char \'%c\', ascii %d\n", inputString[ind], inputString[ind]);
#endif
    if (inputString[ind] > 64 && inputString[ind] < 91) {
      // Meaning caps
      hash += values[inputString[ind] - 65];
#if defined(DEBUG)
      printf("\t\tAdding %d (%d)\n",
	     values[inputString[ind] - 65],
             inputString[ind] - 65);
#endif
    } else if (inputString[ind] > 96 && inputString[ind] < 123) {
      // Meaning lowercase
      hash -= values[inputString[ind] - 97];
#if defined(DEBUG)
      printf("\t\tSubtracting %d (%d)\n",
	     values[inputString[ind] - 97],
             inputString[ind] - 97);
#endif
    } else if (inputString[ind] > 47 && inputString[ind] < 58) {
      // Meaning integer
      hash += inputString[ind] - 48;
#if defined(DEBUG)
      printf("\t\tAdding %d (int)\n",
             inputString[ind] - 48);
#endif
    }
  }
  return hash;
}
