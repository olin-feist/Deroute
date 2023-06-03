# setup.py
from distutils.core import setup
import py2exe

import sys
sys.path.append("./src")

depen  = [
('dlls',['./bin/libstdc++-6.dll', './bin/libgcc_s_seh-1.dll', './bin/libwinpthread-1.dll','./bin/mkl_rt.2.dll','./bin/libgomp-1.dll','./bin/libfaiss.dll','./bin/libfasttext.dll','./bin/libderoute.dll']),
('data',["./data/model.deroute.bin"]),
('stoplists',["./stoplists/English.txt"])
]

setup(
    console=["./src/controller.py"],
    options={
    'py2exe': {"includes": ["lxml._elementpath","justext","parse_website"]}
    },
    data_files=depen,
    py_modules=[]
    
)
