#include <stdint.h>

// 求最高位的位置
int highbit(uint64_t x) {
  int h; 
  for(h = 64; h >= 1; h --) // 找到b的最高位
    if((x >> (h - 1)) & 0x1)
      break;

  return h;
}

// 右移操作
uint64_t rshift(uint64_t x, int shift) {
  if(shift >= 64)
    return 0;

  return x >> shift;
}

// 利用减法实现mod
uint64_t mymod(uint64_t x, uint64_t m) {
  while(x >= m)
    x -= m;

  int hbit = highbit(m);
  while(x >= m) {
    uint64_t tmpm = m;
    int shift = 1;
    while(x >= tmpm) { // 找到最大tmpm
      if(hbit + shift >= 64) // 避免发生溢出
        break;

      tmpm = m << shift;
      shift ++;
    }

    x -= m << (shift - 1);
  }

  return x;
}

uint64_t multi(uint64_t x, int shift, uint64_t m) { // 乘法
  uint64_t r1, r2;

  // r1 = (x >> (128 - hbit - shift)); // 不能这样实现，当右移位数超出数据位数时，发生未定义行为。gcc采取循环移位
  r1 = rshift(x, 64 - shift); // 溢出字段
  r2 = x << shift;
  while(r1 > 0 || r2 >= m) {
    if(r1 == 0 && r2 < m) break;

    uint64_t t1 = 0, t2 = m;
    uint64_t pt1 = 0, pt2 = 0;
    int p = 0;
    while(r1 > t1 || (r1 == t1 && r2 >= t2)) { // 找到最大t1, t2
      p ++;

      pt1 = t1;
      pt2 = t2;

      if(p > 64) break; // 防止r1,r2过大，p超出64

      t1 = rshift(m, 64 - p);
      t2 = m << p;
    }

    t1 = pt1; // 回溯
    t2 = pt2;

    r1 -= t1; // 减去t1, t2
    if(r1 != 0 && r2 < t2) {
      r1 --;
      r2 -= t2;
    }
    else if(r2 >= t2)
      r2 -= t2;
  }

  return r2; // 返回余数
}


uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  if(b > a) { // 将更大的数进行分解
    uint64_t tmp = a;
    a = b;
    b = tmp;
  } 

  uint64_t result = 0;
  for(int i = 0; i < 64; i ++) {
    int flag = (a >> i) & 0x1; // 判断a的每一位
    if(!flag)
      continue;

    uint64_t y = multi(b, i, m); // 乘法

    uint64_t t = result + y;

    if(t < result || t < y) { // x + y发生了溢出，x + y = t + 2^64
      while(t >= m) // 一直减m,直到发生溢出
        t -= m;     // 利用溢出和模的性质，处理溢出问题

      t -= m;
    }

    result = mymod(t, m);
  }

  return result;
}
