#include <array>
#include <cstddef>
#include <cstdint>

template <typename X> inline std::array<uint8_t, sizeof(X)> arrize(X a) {
  return *reinterpret_cast<std::array<uint8_t, sizeof(X)> *>(&a);
}
template <typename X> inline X dearrize(std::array<uint8_t, sizeof(X)> a) {
  return *reinterpret_cast<X *>(&a);
}
template <size_t S, typename M>
inline std::array<uint8_t, S> get(M &m, uint64_t x) {
  std::array<uint8_t, S> b;
  for (size_t i = 0; i < S; i++)
    b[i] = m[x + i];
  return b;
}
template <typename X, typename M> inline X sget(M &m, uint64_t x) {
  return dearrize<X>(get<sizeof(X)>(m, x));
}
template <size_t S, typename M>
inline void put(M &m, uint64_t x, std::array<uint8_t, S> r) {
  for (size_t i = 0; i < S; i++)
    m[x + i] = r[i];
}
template <typename X, typename M> inline void sput(M &m, uint64_t x, X r) {
  put(m, x, arrize(r));
}
template <size_t S, typename M, typename N>
inline void copy(M *m, uint64_t ma, N &n, uint64_t na) {
  for (size_t i = 0; i < S; i++)
    m[ma + i] = n[na + i];
}
template <typename X, typename M, typename N>
inline void scopy(M *m, uint64_t ma, N &n, uint64_t na) {
  copy<sizeof(X)>(m, ma, n, na);
}
template <typename V, typename I, typename M> V &alloc(M &m, I *ip = 0) {
  I i;
  while (m.count(i))
    i++;
  if (ip)
    *ip = i;
  return m[i];
}