#include <vector>
template<typename T, typename C>
std::vector<T> map(const std::vector<C> &array, auto iteratee) {
  int index = -1;
  int length = array.size();
  std::vector<T> v(length);
  while(++index < length) {
    v[index] = iteratee(array[index], index);
  }
  return v;
}