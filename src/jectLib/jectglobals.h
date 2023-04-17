#include <array>
#include <cstdint>
#include <cstdlib>
#include <map>
#include <vector>
extern std::map<uint64_t, uint8_t> glob;
extern std::map<uint32_t, std::array<uint64_t, 4>> wasi;
extern "C" {
void sleep(uint64_t x)
    __attribute__((__import_module__("portal"), __import_name__("sleep")));
uint64_t start(void *ptr, uint64_t len)
    __attribute__((__import_module__("portal"), __import_name__("start")));
uint64_t create_heap(uint8_t vals[65536])
    __attribute__((__import_module__("portal"),
                   __import_name__("create_heap")));
std::array<uint8_t, 65536> *read_heap(uint64_t addr, bool keep)
    __attribute__((__import_module__("portal"), __import_name__("read_heap")));
uint64_t ipc(uint64_t m, uint64_t val)
    __attribute__((__import_module__("portal"), __import_name__("ipc")));

void start_unwind(void*) __attribute__((__import_module__("asyncify"), __import_name__("start_unwind")));
void start_rewind(void*) __attribute__((__import_module__("asyncify"), __import_name__("start_rewind")));
void stop_unwind() __attribute__((__import_module__("asyncify"), __import_name__("stop_unwind")));
void stop_rewind() __attribute__((__import_module__("asyncify"), __import_name__("stop_rewind")));
}
struct msg {
  uint16_t ty;
  uint64_t msg;
};
inline std::vector<uint64_t> call_ipc(std::vector<msg> m, uint64_t target) {
  uint16_t s = m.size();
  uint8_t send[65536];
  *(uint16_t *)send = s;
  uint8_t *t = send;
  t += 2;
  for (auto n : m) {
    *(msg *)t = n;
    t += sizeof(msg);
  }
  auto r_ = read_heap(ipc(target, create_heap(send)), false);
  auto r = *r_;
  free(r_);
  uint64_t *ra = (uint64_t *)&r[0];
  std::vector<uint64_t> ret;
  for (auto n : m) {
    ret.push_back(*ra);
    ra++;
  };
  return ret;
}
enum msgTy : uint16_t {
  READ_MEM,
  WRITE_MEM,
  MALLOC,
  FREE,
  START_UNWIND,
  START_REWIND,
  STOP_UNWIND,
  STOP_REWIND,
  ENTRY,
};