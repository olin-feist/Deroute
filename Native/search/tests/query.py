import ctypes

def embed(file_path,text):
    float_array_pointer=embed_dll.get_vector(text, file_path)
    
    return float_array_pointer

def search_local(query):
    search_results=search_dll.search(query)
    return search_results

vectors_path="../data/vectors.bin"
urls_path="../data/urls.bin"

#search result struct
class search_ret(ctypes.Structure):
    _fields_ = [
        ("k", ctypes.c_int),
        ("distances", ctypes.POINTER(ctypes.c_float)),
        ("urls",  ctypes.POINTER(ctypes.c_char_p)),
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
embed_dll.get_vector.restype = ctypes.POINTER(ctypes.c_float)
search_dll.search.restype = ctypes.POINTER(search_ret)
embed_dll.load_model(b"../bin/model.deroute.bin")

search_dll.load_data(vectors_path.encode("utf-8"),urls_path.encode("utf-8"))
#search_dll.update_index(vectors_path.encode("utf-8"))
try:
    #search request from stdin
    while(True):
        query = input("Enter Query: ")
        print()
        print("Results:")
        vector=embed(b"",query.encode("utf-8"))

        results = search_local(vector)
        embed_dll.free_ptr(results)
        print(results)
        k=results.contents.k
        for i in range(k):
            print(results.contents.urls[i].decode("utf-8").replace("\0", ""))
            print(results.contents.distances[i])
        print()

except KeyboardInterrupt:
    print()
    print("Done Searching")
