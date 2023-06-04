import ctypes
import os
import sys
import time;
sys.path.append("../src")
from parse_website import parse_website

def embed(file_path,text,url):
    float_array_pointer=deroute_dll.get_vector(text, url, file_path)
    
    return float_array_pointer

database_path="../data/vectors.bin"

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
#mkl_intel_thread = ctypes.WinDLL('../bin/mkl_intel_thread.2.dll')
libgomp = ctypes.WinDLL('../bin/libgomp-1.dll')
libfaiss = ctypes.WinDLL('../bin/libfaiss.dll')
libfasttext = ctypes.WinDLL('../bin/libfasttext.dll')

# Load the main DLL's
deroute_dll = ctypes.WinDLL('../bin/libderoute.dll')
deroute_dll.get_vector.restype = ctypes.POINTER(ctypes.c_float)
deroute_dll.search.restype = ctypes.POINTER(search_ret)
deroute_dll.load_model(b"../bin/model.deroute.bin")

if os.path.exists(database_path):
    os.remove(database_path)
    print("Deleted vectors.bin")


start = time.perf_counter()
count = 0
avg = 0
with open('urls.txt') as urls:
    for url in urls:
        count+=1
        start_1 = time.perf_counter()
        web_content=parse_website(url,False)
 
        embed(database_path.encode("utf-8"),web_content.encode("utf-8"),url.encode("utf-8"))
        
        end_1 = time.perf_counter()
        avg+=end_1-start_1
        print("Embedded URL: {}".format(url.strip())),

end = time.perf_counter()
avg/=count

print()
print("Embedded {} URLS".format(count))
print("Duration: {:.4f}s Average: {:.4f}s".format((end-start),avg))
print("Labels stored in urls.bin")
print("Vectors stored in vectors.bin")