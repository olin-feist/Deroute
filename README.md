# Deroute

# Building similarity search tools from source
### Requirments
* CMake
* Mingw64 compiler
* Intel MKL
  * mkl_rt.2.dll
* FAISS
  * include directories
  * libfaiss.dll.a
  * libfaiss.dll
* FastText
  * include directories
  * libfasttext.dll.a
  * libfasttext.dll
  
### Step 1: Invoke Cmake
``` shell
cmake -G "MinGW Makefiles" -B build .
```

### Step 2: Invoke Make
``` shell
mingw32-make -C build ${target}
```
Current targets include:
* search
* embed
* vectools
