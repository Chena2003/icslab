#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define U64 "%" "-19" PRIu64 // PRIu64保证可以在32位和64位上正确输出
#define DATASIZE INT64_MAX

uint64_t multimod(uint64_t, uint64_t, uint64_t);
uint64_t multimod_ref(uint64_t, uint64_t, uint64_t); 

int main(int argc, char *argv[]) {
    int seed = time(0);
    srand(seed);
    
    int loop = 1, count = 0;
    if(argc > 1)
        sscanf(argv[1], "%d", &loop);

    for(int i = 0; i < loop; i ++) {
        uint64_t x, y, m;

        x = ((uint64_t)rand() * rand()) % DATASIZE + 1;
        y = ((uint64_t)rand() * rand()) % DATASIZE + 1;
        m = ((uint64_t)rand() * rand()) % DATASIZE + 1; 

      uint64_t ref = multimod_ref(x, y, m);

      uint64_t res = multimod(x, y, m);

      if(res == ref)
        count ++;
      else
        printf("(" U64 " * " U64 ") mod " U64 " =  " U64 ", res = " U64 "\n", x, y, m, ref, res);
    }

    printf("total test %d times, pass %d times\n", loop, count);
}
