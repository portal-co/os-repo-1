#include "ir/module-utils.h"
#include "pass.h"
#include "wasm.h"
#include "wasm-binary.h"
#include "map.h"
void moveOut(wasm::Module *);
std::vector<uint8_t> coreData();
inline void process(wasm::Module *x) {
  moveOut(x);
  wasm::Module n;
  wasm::WasmBinaryBuilder b(n, x->features, map<char>(coreData(), [](auto i,auto x) -> char { return (char)x; }));
  b.read();
  wasm::ModuleUtils::copyModule(n, *x);
  wasm::PassRunner p(x);
  p.add("asyncify");
  p.run();
}