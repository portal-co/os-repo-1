#include "passes.h"
#include "wasm-builder.h"
#include "ject.wasm.h"
#include <vector>
using namespace wasm;
std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}
std::vector<uint8_t> coreData(){
    return {ject, ject + sizeof(ject)};
}
void moveOut(wasm::Module *m) {
  std::map<Name, Name> fnames;
  for (auto &f : m->functions) {
    fnames[f->name] = Name(std::string("mo$") + ReplaceString(std::string(f->name.str),".","$"));
  }
  ModuleUtils::renameFunctions(*m, fnames);
  std::vector<Expression **> loadStores;
  PointerFinder a;
  PointerFinder b;
  a.list = b.list = &loadStores;
  a.id = Expression::LoadId;
  b.id = Expression::StoreId;
  ModuleUtils::iterDefinedFunctions(*m, [&](auto x) {
    a.walk(x->body);
    b.walk(x->body);
  });
  Builder d(*m);
  for (auto ls : loadStores) {
    Expression *arg;
    Expression *v;
    Type t = (*ls)->type;
    bool il = true;
    if ((*ls)->_id == Expression::LoadId) {
      Load *l = (Load *)*ls;
      arg = d.makeBinary(BinaryOp::AddInt64, l->ptr, d.makeConst(l->offset));
    } else {
      Store *s = (Store *)*ls;
      v = s->value;
      arg = d.makeBinary(BinaryOp::AddInt64, s->ptr, d.makeConst(s->offset));
      t = s->valueType;
      il = false;
    }
    std::string name("mo_");
    name += il ? "load_" : "store_";
    name += t.toString();
    std::vector<Expression *> a{arg};
    if (v) {
      a.push_back(v);
    }
    *ls = d.makeCall(Name(name), a, (*ls)->type);
  }
}