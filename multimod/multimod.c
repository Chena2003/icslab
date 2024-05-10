#include <stdint.h>

uint64_t mymod(uint64_t x, uint64_t m) {
  while(x >= m)
    x -= m;

  return x;
}

uint64_t muti(uint64_t x, int hbit, int shift, uint64_t m) { // 乘法
  uint64_t r1, r2;

  r1 = x >> (128 - hbit - shift);
  r2 = x << shift;
  
  while(r1 > 0 || r2 >= m) {
    if(r1 == 0 && r2 < m) break;

    if(r2 < m) {
      r1 --;
      r2 -= m;
    }
    else
      r2 -= m;

  }

  return r2;
}


uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  if(b > a) { // 将更大的数进行分解
    uint64_t tmp = a;
    a = b;
    b = tmp;
  } 

  int h; 
  for(h = 64; h >= 1; h --) // 找到b的最高位
    if((b >> (h - 1)) & 0x1)
      break;

  uint64_t result = 0;
  for(int i = 0; i < 64; i ++) {
    int flag = (a >> i) & 0x1;
    if(!flag)
      continue;

    uint64_t y = muti(b, h, i, m);

    uint64_t t = result + y;

    if(t < result || t < y) { // x + y发生了溢出
      while(t >= m) 
        t -= m;

      t -= m;
    }

    result = mymod(t, m);
  }

  return result;
}
