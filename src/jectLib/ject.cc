#include "ject.h"
#include "jectglobals.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <stdlib.h>
// typedef struct wasi_iovec_t {
//   /**
//    * The address of the buffer to be filled.
//    */
//   uint64_t buf;

//   /**
//    * The length of the buffer to be filled.
//    */
//   size_t buf_len;

// } wasi_iovec_t;

// std::map<uint64_t, uint8_t> glob;
// std::map<uint32_t, std::array<uint64_t, 4>> wasi;
// inline uint32_t popenat(std::array<uint64_t, 4> a, uint8_t *x, size_t d,
//                         std::array<uint64_t, 4> &r) {
//   auto y = readWrite(a, (uint8_t *)"o", 1, true);
//   if (y) {
//     return y;
//   }
//   y = readWrite(a, &x[0], d, true);
//   if (y) {
//     return y;
//   }
//   y = readWrite(a, (uint8_t *)"\0", 1, true);
//   if (y) {
//     return y;
//   }
//   uint8_t z[32];
//   y = readWrite(a, &z[0], 32, false);
//   if (y) {
//     return y;
//   }
//   r = dearrize<std::array<uint64_t, 4>>(std::to_array(z));
// }
extern "C" {
uint64_t my;
uint64_t mo$usr_msg(uint16_t, uint64_t);
void mo$EntryPoint();
}
__attribute__((always_inline)) uint64_t handle(msg m) {
  switch (m.ty) {
  case READ_MEM:
    return *(uint8_t *)(m.msg);
  case WRITE_MEM: {
    auto x = read_heap(m.msg, false);
    auto y = *x;
    free(x);
    uint64_t z = *(uint64_t *)&y[0];
    *(uint8_t *)z = y[8];
    return 0;
  }
  case MALLOC:
    return (uint64_t)malloc(m.msg);
  case FREE:
    free((void *)m.msg);
    return 0;
  case START_REWIND:
    start_rewind((void *)m.msg);
    return 0;
  case START_UNWIND:
    start_unwind((void *)m.msg);
    return 0;
  case STOP_REWIND:
    stop_rewind();
    return 0;
  case STOP_UNWIND:
    stop_unwind();
    return 0;
  case ENTRY:
  mo$EntryPoint();
  return 0;
  default:
    return mo$usr_msg(m.ty, m.msg);
  }
}
extern "C" {
uint64_t ipc_handler(uint64_t x) {
  auto h = read_heap(x, false);
  auto i = &h[0];
  uint16_t l = *(uint16_t *)i;
  i += sizeof(uint16_t);
  auto p = (msg *)i;
  uint8_t r[65536];
  uint64_t *s = (uint64_t *)r;
  while (l) {
    l--;
    msg m = *p;
    p++;
    *s = handle(m);
    s++;
  }
  free(h);
  return create_heap(r);
}
uint32_t mo_load_i32(uint64_t a) { return sget<uint32_t>(glob, a); }
uint64_t mo_load_i64(uint64_t a) { return sget<uint64_t>(glob, a); }
float mo_load_f32(uint64_t a) { return sget<float>(glob, a); }
double mo_load_f64(uint64_t a) { return sget<double>(glob, a); }

void mo_store_i32(uint64_t a, uint32_t b) { sput(glob, a, b); }
void mo_store_i64(uint64_t a, uint64_t b) { sput(glob, a, b); }
void mo_store_f32(uint64_t a, float b) { sput(glob, a, b); }
void mo_store_f64(uint64_t a, double b) { sput(glob, a, b); }

// void mo$wasi_snapshot_preview1$fd_read(uint32_t a, uint64_t p, size_t l,
//                                        uint64_t t) {
//   for (size_t i = 0; i < l; i++) {
//     auto r = sget<wasi_iovec_t>(glob, p + (i * sizeof(struct wasi_iovec_t)));
//     uint8_t x[r.buf_len];
//     auto y = readWrite(wasi[a], &x[0], r.buf_len, false);
//     if (y) {
//       mo_store_i32(t, y);
//       return;
//     }
//     for (size_t b = 0; b < r.buf_len; b++)
//       glob[r.buf + b] = x[b];
//   }
//   mo_store_i32(t, 0);
// }
// void mo$wasi_snapshot_preview1$sock_recv(uint32_t a, uint64_t p, size_t l,
//                                          uint32_t _b, uint64_t t,
//                                          uint32_t _tt) {
//   for (size_t i = 0; i < l; i++) {
//     auto r = sget<wasi_iovec_t>(glob, p + (i * sizeof(struct wasi_iovec_t)));
//     uint8_t x[r.buf_len];
//     auto y = readWrite(wasi[a], &x[0], r.buf_len, false);
//     if (y) {
//       mo_store_i32(t, y);
//       return;
//     }
//     for (size_t b = 0; b < r.buf_len; b++)
//       glob[r.buf + b] = x[b];
//   }
//   mo_store_i32(t, 0);
// }
// void mo$wasi_snapshot_preview1$fd_pread(uint32_t a, uint64_t p, size_t l,
//                                         uint64_t t) {
//   for (size_t i = 0; i < l; i++) {
//     auto r = sget<wasi_iovec_t>(glob, p + (i * sizeof(struct wasi_iovec_t)));
//     uint8_t x[r.buf_len];
//     auto y = readWrite(wasi[a], &x[0], r.buf_len, false);
//     if (y) {
//       mo_store_i32(t, y);
//       return;
//     }
//     for (size_t b = 0; b < r.buf_len; b++)
//       glob[r.buf + b] = x[b];
//   }
//   mo_store_i32(t, 0);
// }
// void mo$wasi_snapshot_preview1$fd_write(uint32_t a, uint64_t p, size_t l,
//                                         uint64_t t) {
//   for (size_t i = 0; i < l; i++) {
//     auto r = sget<wasi_iovec_t>(glob, p + (i * sizeof(struct wasi_iovec_t)));
//     uint8_t x[r.buf_len];
//     for (size_t b = 0; b < r.buf_len; b++)
//       x[b] = glob[r.buf + b];
//     auto y = readWrite(wasi[a], &x[0], r.buf_len, true);
//     if (y) {
//       mo_store_i32(t, y);
//       return;
//     }
//   }
//   mo_store_i32(t, 0);
// }
// void mo$wasi_snapshot_preview1$sock_send(uint32_t a, uint64_t p, size_t l,
//                                          uint64_t t) {
//   for (size_t i = 0; i < l; i++) {
//     auto r = sget<wasi_iovec_t>(glob, p + (i * sizeof(struct wasi_iovec_t)));
//     uint8_t x[r.buf_len];
//     for (size_t b = 0; b < r.buf_len; b++)
//       x[b] = glob[r.buf + b];
//     auto y = readWrite(wasi[a], &x[0], r.buf_len, true);
//     if (y) {
//       mo_store_i32(t, y);
//       return;
//     }
//   }
//   mo_store_i32(t, 0);
// }

// uint32_t mo$wasi_snapshot_preview1$path_open(uint32_t a, uint32_t b_,
//                                              uint64_t c, size_t d, int32_t
//                                              arg4, int64_t arg5, int64_t
//                                              arg6, int32_t arg7, uint64_t t)
//                                              {
//   uint8_t x[d];
//   for (size_t e = 0; e < d; e++)
//     x[e] = glob[c + e];
//   uint32_t j;
//   auto z = popenat(wasi[a], x, d,
//                    alloc<std::array<uint64_t, 4>, uint32_t>(wasi, &j));
//   if (z) {
//     return z;
//   }

//   mo_store_i32(t, j);
// }
// uint32_t mo$wasi_snapshot_preview1$fd_close(uint32_t x) {
//   close(wasi[x]);
//   wasi.erase(x);
// }
// void mo$portal$pipe(uint64_t a, uint64_t b, bool tw) {
//   std::array<uint64_t, 4> c;
//   std::array<uint64_t, 4> d;
//   pipe(c, d, tw);
//   uint32_t j, k;
//   alloc<std::array<uint64_t, 4>, uint32_t>(wasi, &j) = c;
//   alloc<std::array<uint64_t, 4>, uint32_t>(wasi, &k) = d;
//   mo_store_i32(a, j);
//   mo_store_i32(b, k);
// }
// std::array<uint64_t, 4> mo$portal$l2g(uint32_t x) { return wasi[x]; }
// uint32_t mo$portal$g2l(std::array<uint64_t, 4> x) {
//   uint32_t a;
//   alloc<std::array<uint64_t, 4>>(wasi, &a) = x;
//   return a;
// }
// uint32_t mo$portal$connect(uint32_t a, std::array<uint8_t, 6> u, uint64_t t)
// {
//   uint32_t j;
//   std::array<uint8_t, 7> s;
//   for (size_t i = 0; i < 6; i++)
//     s[i + 1] = u[i];
//   s[0] = 'c';
//   auto z = popenat(wasi[a], &s[0], sizeof(s),
//                    alloc<std::array<uint64_t, 4>, uint32_t>(wasi, &j));
//   if (z) {
//     return z;
//   }

//   mo_store_i32(t, j);
// }
}