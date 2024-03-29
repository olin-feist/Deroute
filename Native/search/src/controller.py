import ctypes
import struct
from flask import Flask, request, jsonify
from parse_website import parse_website
from threading import Lock
import base64
import os;

#generate dense vector
def embed(file_path,text,url):
    float_array_pointer=deroute_dll.get_vector(text, url, file_path)
    return float_array_pointer

#search local database
def search_local(query):
    search_results=deroute_dll.search(query)
    return search_results

#free struct returned by search
def free_search_results(struct_ptr):
    deroute_dll.delete_struct(struct_ptr)

#search result struct
class search_ret(ctypes.Structure):
    _fields_ = [
        ("k", ctypes.c_int),
        ("distances", ctypes.POINTER(ctypes.c_float)),
        ("urls",  ctypes.POINTER(ctypes.c_char_p)),
    ]


mutex = Lock() #mutex lock for updating database

# Load the required dependencies
libstdc = ctypes.WinDLL('dlls/libstdc++-6.dll')
libgcc_s_seh = ctypes.WinDLL('dlls/libgcc_s_seh-1.dll')
libwinpthread = ctypes.WinDLL('dlls/libwinpthread-1.dll')
mkl_rt = ctypes.WinDLL('dlls/mkl_rt.2.dll')
#mkl_intel_thread = ctypes.WinDLL('dlls/mkl_intel_thread.2.dll')
libgomp = ctypes.WinDLL('dlls/libgomp-1.dll')
libfaiss = ctypes.WinDLL('dlls/libfaiss.dll')
libfasttext = ctypes.WinDLL('dlls/libfasttext.dll')

# Load the main DLL's
deroute_dll = ctypes.WinDLL('dlls/libderoute.dll')
deroute_dll.get_vector.restype = ctypes.POINTER(ctypes.c_float)
deroute_dll.search.restype = ctypes.POINTER(search_ret)
deroute_dll.get_vector_size.restype = ctypes.c_int


status=deroute_dll.load_model(b"data/model.deroute.bin")  #load model
if(status==1):
    print("Error: Failed to load Deroute Model")
else:
    print("Succesfully loaded Deroute Model")

database_path="data/vectors.bin"                   #database path
urls_size=300                                    #urls file url size
vector_size= deroute_dll.get_vector_size()         #get vector size
isDataLoaded=False

app = Flask(__name__)

#embed url request
@app.route('/embedUrl', methods=['POST'])
def embed_url():
    global isDataLoaded

    data = request.get_json()
    url = data['url']
    
    #prevent multiple edits to database
    mutex.acquire()

   
    web_content=parse_website(url,False) #parse website

    if(web_content==-1): #if web parsing failed
        mutex.release()
        return jsonify({'response': 'Error: Web parsing failed'})
    
    retrun_v=embed(database_path.encode("utf-8"),web_content.encode("utf-8"),url.encode("utf-8"))

    #check for nullptr return
    if(not retrun_v):
        mutex.release()
        return jsonify({'response': 'Error embed url failed'})
    
    deroute_dll.free_ptr(retrun_v) #free float*

    #if first url being saved
    if(not isDataLoaded):
        erno=deroute_dll.load_search_index(database_path.encode("utf-8")) #load database for searching 
        if(erno==1):
            print("Embedded ",url)
            print("Data successfully loaded")
            isDataLoaded=True
        else:
            print("Error: Intial data load failed")

    #update search index
    else:
        erno=deroute_dll.update_index()
        if(erno==0):
            print("Embedded ",url)
        else:
            print("Error: Failed to update search index with new URL")


    mutex.release()

    return jsonify({'response': 'Done'})


#embed search query
@app.route('/embedQuery', methods=['POST'])
def embed_query():  
    #get query from request
    data = request.get_json()
    query = data['query']
    query=query.replace("+"," ")
    print("Query: {}".format(query))

    #get dense vector
    dense_vector=embed(b"",query.encode("utf-8"),b"")
    
    if(not dense_vector):
        return jsonify({'response': 'Error embed query failed'})
    
    #convert float* to bytearray
    buf = bytearray()
    for i in range(vector_size):
        buf += struct.pack('f', dense_vector[i])

    print("Created Vector, First 5 elements:",dense_vector[:5])

    deroute_dll.free_ptr(dense_vector) #free float*
    
    return jsonify({'vector': base64.b64encode(buf).decode('utf-8')})

#search local database request
@app.route('/search', methods=['POST'])
def search():
    
    req = request.get_json()
    vector = req['vector']

    data=bytearray(base64.b64decode(vector)) #get byte array

    #convert to ctypes float pointer
    float_array_type = ctypes.c_float * vector_size
    float_array = float_array_type.from_buffer_copy(data)
    float_pointer = ctypes.cast(float_array, ctypes.POINTER(ctypes.c_float))

    print("Recieved Vector, First 5 elements:",float_pointer[:5])
    
    #search local database
    results = search_local(float_pointer)
    
    #check for null return
    try:
       k=results.contents.k
    except ValueError:
        ret =[]
        return jsonify(ret)
    
    ret =[]
    k=results.contents.k

    for i in range(k):
        single_res={}
        
        single_res["url"]=results.contents.urls[i].decode("utf-8").replace("\0", "")
        single_res["dist"]=float("{:.4f}".format(results.contents.distances[i]))
        ret.append(single_res)

    free_search_results(results) #free struct

    return jsonify(ret)

if __name__ == '__main__':
    if(not os.path.isfile(database_path)):
        print("Warning: Database file is not created yet")


    if(os.path.isfile(database_path)):
        isDataLoaded=True
        erno=deroute_dll.load_search_index(database_path.encode("utf-8"))       

        if(erno==1):
            print("Database successfully loaded")
        else:
            print("Error: Intial database loading failed")

    from waitress import serve
    serve(app, port=5000,backlog=2000,connection_limit=200,threads=8)



