$(O)/tools/embedfile: $(R)/tools/embedfile.c
	mkdir -p $(O)/tools
	cc $(R)/tools/embedfile.c -o $(O)/tools/embedfile

$(O)/3rdparty/binaryen:
	git clone https://github.com/WebAssembly/binaryen.git $(O)/3rdparty/binaryen

$(O)/3rdparty-build-wasm/binaryen/lib/libbinaryen.a: $(O)/3rdparty/binaryen
	mkdir -p $(O)/3rdparty-build-wasm/binaryen
	sh $(R)/tools/cmake.sh cmake -DBUILD_TESTS=OFF -DENABLE_WERROR=OFF -DBUILD_TOOLS=OFF -DBUILD_LLVM_DWARF=OFF -DBUILD_STATIC_LIB=ON -S$(O)/3rdparty/binaryen -B$(O)/3rdparty-build-wasm/binaryen
	$(MAKE) -C $(O)/3rdparty-build-wasm/binaryen binaryen

BY_CFLAGS=-I$(O)/3rdparty/binaryen/src

BY_LIBS=$(O)/3rdparty-build-wasm/binaryen/lib/libbinaryen.a

include $(R)/src/portal-uefi/makefile.mk

include $(R)/src/jectLib/makefile.mk

include $(R)/src/taskd/makefile.mk