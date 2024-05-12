#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b) {
  int64_t sum;
  asm("leaq (%1, %2, 1), %0" // 64位数据，用q
      : "=r"(sum)
      : "r"(a), "r"(b)
  );
  return sum;
}

int asm_popcnt(uint64_t x) {
  int s = 0;
  asm("movl $0, %%ecx;" 
      "1:\t movq %1, %%rax;" // 设置标号
      "shr %%cl, %%rax;"
      "and $0x1, %%rax;"
      "test %%rax, %%rax;"
      "je 2f;" // 2f表示当前代码段后一个标号
      "incl %0;"
      "2:\t incl %%ecx;"
      "cmpl $0x3f, %%ecx;"
      "jle 1b" // 1b表示当前代码段前一个标号
      : "+r"(s)
      : "r"(x)
      : "%ecx", "%rax"
  );

  // chatgpt生成版本
  //   asm(
  //     "movq %1, %%rax;"   // 将x加载到rax中
  //     "xorq %%rcx, %%rcx;" // 用于计数的寄存器清零
  //     "popcntq %%rax, %%rax;" // 对rax中的位进行计数，结果存储在rax中
  //     "movl %%eax, %0;" // 将rax的低32位（popcntq结果）存储到s中
  //     : "=r"(s) // 输出：s对应rax的低32位
  //     : "r"(x) // 输入：x对应rax
  //     : "%rax", "%rcx" // 用到的寄存器
  // ); 

  return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  asm(
      "movq %1, %%rdi;"
      "movq %2, %%rsi;"
      "movq %3, %%rdx;"  // n存储在64位寄存器中，故要用q
      "call memcpy;"
      : "+r"(dest)
      : "r"(dest), "r"(src), "r"(n)
  );

  return dest;
}

int asm_setjmp(asm_jmp_buf env) {
  return setjmp(env);
}

void asm_longjmp(asm_jmp_buf env, int val) {
  longjmp(env, val);
}
