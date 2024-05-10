#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define BUFSIZE 1024

static char buf[BUFSIZE];
static char code_buf[BUFSIZE + 100];
static char code_fomat[] = "python3 -c 'print((%lu * %lu) %% %lu)'";

uint64_t multimod(uint64_t, uint64_t, uint64_t);

int main(int argc, char *argv[]) {
    int seed = time(0);
    srand(seed);
    
    int loop = 1, count = 0;
    if(argc > 1)
        sscanf(argv[1], "%d", &loop);
 
    for(int i = 0; i < loop; i ++) {
        uint64_t x, y, m;

        x = rand() % INT32_MAX + 1;
        y = rand() % INT32_MAX + 1;
        m = rand() % INT32_MAX + 1;

        sprintf(code_buf, code_fomat, x, y, m);

        FILE *fp = popen(code_buf, "r");
        assert(fp);

        fscanf(fp, "%s", buf);
        pclose(fp);

        uint64_t res = multimod(x, y, m);
        uint64_t ref;
        sscanf(buf, "%lu", &ref);

        printf("(%lu * %lu) mod %lu =  %s, res = %lu\n", x, y, m, buf, res);
        // assert(ref != ref);
        if(res == ref)
            count ++;
    }

    printf("total test %d times, pass %d times\n", loop, count);

    return 0;
}