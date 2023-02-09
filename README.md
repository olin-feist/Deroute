# Deroute
This branch contains the search tools used by the Deroute extension.
### Search:
The search tool will find the nearest neighbors of given database of dense vectors.
``` shell
search database_path label_path *query_path
```
- `database_path` path to binary file of dense vectors to be searched <br>
- `index_path` path to binary file of urls <br>
- `query_path` optional path to binary file of query vectors, if left unset will read binary data from stdin<br>

### Embed:
The embed tool will take a single line of plain text from stdin and return a dense vector representation of it in the form of a binary file.
``` shell
embed model_path *output_path
```
- `model_path` path to the word embedding model<br>
- `output_path` optional parameter to specifiy output of embedding, if left unset will output vector as binary to stdout<br>

### Locally Sensitive Hashing:
Given a binary file of vectors will compute and print the corresponding 128bit hash value.
``` shell
lsh vectors_path
```
- `vectors_path` path to binary file of dense vectors to be hashed <br>

### Website Parser:
The website parser will take a html file of a website and parse it down to its relevant content
``` shell
python src/parse_website.py output_path url
```
- `url` url of website to be parsed<br>
- `output_path` path to where binary file of urls will be written <br>
# Binary Data format
All binary data sent and recieved from Deroute tools will be in the following format <br>
```
[D][N]
v0
v1
.
.
.
vN
```
- <b>D</b> : 4 byte integer corresponding to dimensions of vectors
- <b>N</b> : 4 byte integer corresponding to the number of vectors
- <b>V</b><sub>i</sub> : Vector of 4 byte floating point values with number of elements equal to <b>[D]</b> 
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
Builds a dataset of websites taken from a text file of urls passed via command line (example URLS in test/)
### query.sh
Querys dataset built by previous script using query built from `tests/query_plain.txt`
### query.js
javascript file that will utilize the Deroute tools to query a dataset built from `build_dataset.sh`
