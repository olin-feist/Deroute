# Deroute
This branch contains the search tools used by the Deroute extension.
### Search:
The search tool will find the nearest neighbors of given database of dense vectors.
``` shell
search database_path query_path k_neighbors
```
- `database_path` path to binary file of dense vectors to be searched <br>
- `query_path` path to binary file of query vectors<br>
- `k_neighbors` number of neighbors to be returned<br>
### Embed:
The embed tool will take a single line of plain text and return a dense vector representation of it in the form of a binary file.
``` shell
embed model_path input_path output_path write_type
```
- `model_path` path to the word embedding model<br>
- `input_path` path to plain text<br>
- `output_path` path to output vector<br>
- `write_type` either "append" to add a vector to existing file or "new" to create a new file<br>

### Locally Sensitive Hashing:
Given a binary file of vectors will compute and print the corresponding 128bit hash value.
``` shell
lsh vectors_path
```
- `vectors_path` path to binary file of dense vectors to be hashed <br>

### Website Parser:
The website parser will take a html file of a website and parse it down to its relevant content
``` shell
python src/parse_website.py url
```
- `url` url of website to be parsed<br>

# Building similarity search tools from source
### Requirments
* CMake
* Mingw64 compiler
* Intel MKL
  * mkl_rt.2.dll
  * mkl_intel_thread.2.dll
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
* lsh

# Test Scripts
### build_dataset.sh
Builds a dataset of websites taken from `data/urls.txt`
### query.sh
Querys dataset built by previous script using query build from `data/query_plain.txt`
