import ctypes
import struct
from flask import Flask, request, jsonify
from parse_website import parse_website

#generate dense vector
def embed(file_path,text):
    float_array_pointer=embed_dll.getVector(file_path, text)
    
    return float_array_pointer

#search local database
def search_local(database,labels,query):
    search_results=search_dll.search(database,labels,query)
    return search_results

#free struct returned by search
def free_search_results(struct_ptr):
    search_dll.free_mem(struct_ptr.contents.distances)
    search_dll.free_mem(struct_ptr.contents.urls)
    search_dll.delete_struct(struct_ptr)

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
embed_dll.get_vector_size.restype = ctypes.c_int


embed_dll.load_model(b"../bin/model.q.ftz") #load model
vectors_path="../data/vectors.bin"          #database path
urls_path="../data/urls.bin"                #labels path
urls_size=300                               #labels path url size
vector_size= embed_dll.get_vector_size()    #get vector size

app = Flask(__name__)

#embed url request
@app.route('/embedUrl', methods=['POST'])
def embed_url():
    data = request.get_json()
    url = data['url']
    web_content=parse_website(url,urls_path,False)
    if(web_content==-1):
        return jsonify({'response': 'Error'})
    
    embed(vectors_path.encode("utf-8"),web_content.encode("utf-8"))

    return jsonify({'response': 'Done'})

#embed search query
@app.route('/embedQuery', methods=['POST'])
def embed_query():
    data = request.get_json()
    query = data['query']
    print(query)
    dense_vector=embed("",query.encode("utf-8"))
    
    buf = bytearray()
    for i in range(vector_size):
        buf += struct.pack('f', dense_vector[i])


    return buf

#search vectors request
@app.route('/search', methods=['POST'])
def search():
    
    data = request.data #get buffer

    #convert to ctypes float pointer
    float_array_type = ctypes.c_float * vector_size
    float_array = float_array_type.from_buffer_copy(data)
    float_pointer = ctypes.cast(float_array, ctypes.POINTER(ctypes.c_float))

    #search local database
    results = search_local(vectors_path.encode("utf-8"),urls_path.encode("utf-8"),float_pointer)
    
    
    ret ={}
    k=results.contents.k
    for i in range(k):
        key_u = f"url{i+1}"
        key_d = f"dist{i+1}"
        ret[key_u]=results.contents.urls[i*urls_size:(i+1)*urls_size].decode("utf-8").replace("\0", "")
        ret[key_d]=results.contents.distances[i]


    free_search_results(results) #free struct

    return jsonify(ret)

if __name__ == '__main__':
    app.run(debug=True,port=5000)



