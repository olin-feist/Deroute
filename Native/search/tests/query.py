import ctypes
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

try:
    #search request from stdin
    while(True):
        query = input("Enter Query: ")
        print()
        print("Results:")
        vector=embed(b"",query.encode("utf-8"))

        results = search_local(vectors_path.encode("utf-8"),urls_path.encode("utf-8"),vector)

        k=results.contents.k
        for i in range(k):
            print(results.contents.urls[i*300:(i+1)*300].decode("utf-8").replace("\0", ""))
            print(results.contents.distances[i])
        print()

except KeyboardInterrupt:
    print()
    print("Done Searching")
