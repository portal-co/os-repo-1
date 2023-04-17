#include "jectglobals.h"
#include "ject.h"
#include <cstdint>
struct task{
    uint64_t puppet;
    bool sleeping;
    struct ref{
        task &t; uint64_t addr;
        inline operator uint8_t(){
            return call_ipc({{READ_MEM,addr}}, t.puppet)[0];
        }
        inline ref& operator=(uint8_t x){
            uint8_t y[65536];
            y[9] = x;
            *(uint64_t*)y = addr;
            call_ipc({{WRITE_MEM,create_heap(y)}}, t.puppet);
            return *this;
        }
    };
    inline ref operator[](uint64_t x){
        return {*this,x};
    }
    template<typename F> void sleep(F f){
        if(sleeping){
            sleeping = false;
            call_ipc({{STOP_REWIND,0}, puppet);
            return;
        }
        sleeping = true;
        auto d = call_ipc({{MALLOC,8}}, puppet)[0];
        auto m = call_ipc({{MALLOC,0xffffffff}}, puppet)[0];
        sput(*this, d, m);
        sput(*this, d + 4, 0xffffffff);
        f([&](){
            call_ipc({{START_REWIND,d},{FREE,m},{ENTRY,0},{STOP_UNWIND,0}},puppet);
        });
    }
};