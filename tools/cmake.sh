c=$1
shift
exec emcmake $c '-DCMAKE_EXECUTABLE_SUFFIX=.wasm' '-DCMAKE_C_FLAGS=-sMEMORY64 -mtail-call' '-DCMAKE_CXX_FLAGS=-sMEMORY64 -mtail-call' "$@"