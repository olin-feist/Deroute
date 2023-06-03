import ctypes
import timeit

def embed(file_path,text):
    float_array_pointer=deroute_dll.get_vector(text, file_path)
    
    return float_array_pointer

def search_local(query):
    search_results=deroute_dll.search(query)
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
#mkl_intel_thread = ctypes.WinDLL('../bin/mkl_intel_thread.2.dll')
libgomp = ctypes.WinDLL('../bin/libgomp-1.dll')
libfaiss = ctypes.WinDLL('../bin/libfaiss.dll')
libfasttext = ctypes.WinDLL('../bin/libfasttext.dll')

# Load the main DLL's
deroute_dll = ctypes.WinDLL('../bin/libderoute.dll')
deroute_dll.get_vector.restype = ctypes.POINTER(ctypes.c_float)
deroute_dll.search.restype = ctypes.POINTER(search_ret)
deroute_dll.load_model(b"../bin/model.deroute.bin")

deroute_dll.load_search_index(vectors_path.encode("utf-8"),urls_path.encode("utf-8"))
deroute_dll.update_index(vectors_path.encode("utf-8"))

try:
    #search request from stdin
    while(True):
        query = input("Enter Query: ")
        print()
        print("Results:")
        

        vector=embed(b"",query.encode("utf-8"))
        
        
        results = search_local(vector)
        
        
        end = timeit.default_timer()
        k=results.contents.k
        for i in range(k):
            print(results.contents.urls[i].decode("utf-8").replace("\0", ""))
            print("{:.4f}".format(results.contents.distances[i]))
        print()

        deroute_dll.free_ptr(vector) #free float*
        deroute_dll.delete_struct(results) #free return struct
        

except KeyboardInterrupt:
    print()
    print("Done Searching")
