#include "asm.h"
#include <assert.h>
#include <stdio.h>

int main() {
  asm_jmp_buf buf;
  int r = asm_setjmp(buf);
  if (r == 0) {
    assert(asm_add(1234, 5678) == 6912);

    assert(asm_popcnt(0x0123456789abcdefULL) == 32);
    assert(asm_popcnt(~0ULL) == 64);
    assert(asm_popcnt(0ULL) == 0);

    char str[] = "hello world";
    char s[128];
    asm_memcpy(s, str, sizeof(str));
    puts(s);

    // TODO: add more tests here.
    asm_longjmp(buf, 123);
  } else {
    assert(r == 123);
    printf("PASSED.\n");
  }
}
