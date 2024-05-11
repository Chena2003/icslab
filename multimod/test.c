#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>

#define U64 "%" PRIu64 
#define BUFSIZE 1024

static char buf[BUFSIZE];
static char code_buf[BUFSIZE + 100];
static char code_fomat[] = "python3 -c" "'print((" U64 " * " U64 ") %% " U64 ")'";

uint64_t multimod_ref(uint64_t x, uint64_t y, uint64_t m) {
    sprintf(code_buf, code_fomat, x, y, m);

    FILE *fp = popen(code_buf, "r"); // 调用python计算
    assert(fp);
    fscanf(fp, "%s", buf); // 取出返回值（计算结果）
    pclose(fp);

    uint64_t ref;
    sscanf(buf, U64, &ref);

    return ref;
}