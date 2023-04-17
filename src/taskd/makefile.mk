$(O)/taskd/taskd.wasm: $(R)/src/taskd/*.cc $(O)/3rdparty-build-wasm/binaryen/lib/libbinaryen.a $(R)/src/taskd/*.h $(O)/ject/ject.wasm.h
	mkdir -p $(O)/taskd
	sh $(R)/tools/k++ -std=c++2a $(O)/ject/ject.wasm $(BY_CFLAGS) -I$(O)/ject -I$(R)/src/jectLib -I$(R)/src/taskd -o $@ $<