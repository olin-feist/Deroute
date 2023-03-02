import requests
import ctypes
import struct
import pika

def embed(file_path,text):
    float_array_pointer=embed_dll.getVector(file_path, text)
    return float_array_pointer

def search(database,labels,query):
    search_results=search_dll.search(database,labels,query)
    return search_results



#search result struct
class search_ret(ctypes.Structure):
    _fields_ = [
        ("k", ctypes.c_int),
        ("distances", ctypes.POINTER(ctypes.c_float)),
        ("urls", ctypes.POINTER(ctypes.c_char)),
    ]

# Load the required dependencies
libstdc = ctypes.WinDLL('./libstdc++-6.dll')
libgcc_s_seh = ctypes.WinDLL('./libgcc_s_seh-1.dll')
libwinpthread = ctypes.WinDLL('./libwinpthread-1.dll')
mkl_rt = ctypes.WinDLL('./mkl_rt.2.dll')
mkl_intel_thread = ctypes.WinDLL('./mkl_intel_thread.2.dll')
libgomp = ctypes.WinDLL('./libgomp-1.dll')
libfaiss = ctypes.WinDLL('./libfaiss.dll')
libfasttext = ctypes.WinDLL('./libfasttext.dll')

# Load the main DLL's
embed_dll = ctypes.WinDLL('./libembed.dll')
search_dll = ctypes.WinDLL('./libsearch.dll')
embed_dll.getVector.restype = ctypes.POINTER(ctypes.c_float)
search_dll.search.restype = ctypes.POINTER(search_ret)
embed_dll.load_model(b"../bin/model.q.ftz")


def on_message(channel, method_frame, header_frame, body):
    #print(header_frame.headers['request_type'])
    print(body)
    channel.basic_ack(delivery_tag=method_frame.delivery_tag)


connection = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel = connection.channel()
queue_name = 'deroute_controller'
channel.queue_declare(queue_name, durable=False)

channel.basic_consume('deroute_controller', on_message)

try:
    channel.start_consuming()
except KeyboardInterrupt:
    channel.stop_consuming()
connection.close()






