import ctypes
import os
from parse_website import parse_website

def embed(file_path,text):
    float_array_pointer=embed_dll.getVector(file_path, text)
    
    return float_array_pointer

def search_local(database,labels,query):
    search_results=search_dll.search(database,labels,query)
    return search_results

vectors_path="../data/vectors.bin"
urls_path="../data/urls.bin"

#search result struct
class search_ret(ctypes.Structure):
    _fields_ = [
        ("k", ctypes.c_int),
        ("distances", ctypes.POINTER(ctypes.c_float)),
        ("urls", ctypes.POINTER(ctypes.c_char)),
    ]

# Load the required dependencies
libstdc = ctypes.WinDLL('../bin/libstdc++-6.dll')
libgcc_s_seh = ctypes.WinDLL('../bin/libgcc_s_seh-1.dll')
libwinpthread = ctypes.WinDLL('../bin/libwinpthread-1.dll')
mkl_rt = ctypes.WinDLL('../bin/mkl_rt.2.dll')
mkl_intel_thread = ctypes.WinDLL('../bin/mkl_intel_thread.2.dll')
libgomp = ctypes.WinDLL('../bin/libgomp-1.dll')
libfaiss = ctypes.WinDLL('../bin/libfaiss.dll')
libfasttext = ctypes.WinDLL('../bin/libfasttext.dll')

# Load the main DLL's
embed_dll = ctypes.WinDLL('../bin/libembed.dll')
search_dll = ctypes.WinDLL('../bin/libsearch.dll')
embed_dll.getVector.restype = ctypes.POINTER(ctypes.c_float)
search_dll.search.restype = ctypes.POINTER(search_ret)
embed_dll.load_model(b"../data/model.q.ftz")

if os.path.exists(vectors_path):
    os.remove(vectors_path)
    print("Deleted vectors.bin")

if os.path.exists(urls_path):
    os.remove(urls_path)
    print("Deleted urls.bin")


count = 0
with open('urls.txt') as urls:
    for url in urls:
        count+=1
        web_content=parse_website(url,urls_path,False)
        embed(vectors_path.encode("utf-8"),web_content.encode("utf-8"))
        print("Embedded URL: {}".format(url.strip())),



print()
print("Embedded {} URLS".format(count))
print("Labels stored in urls.bin")
print("Vectors stored in vectors.bin")