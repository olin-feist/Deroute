# setup.py
from distutils.core import setup
import py2exe

depen  = [
('dlls',['../bin/libstdc++-6.dll', '../bin/libgcc_s_seh-1.dll', '../bin/libwinpthread-1.dll','../bin/mkl_rt.2.dll','../bin/mkl_intel_thread.2.dll','../bin/libgomp-1.dll','../bin/libfaiss.dll','../bin/libfasttext.dll','../bin/libembed.dll','../bin/libsearch.dll']),
('data',["../bin/model.q.ftz"])  
]

setup(
    console=["controller.py"],
    options={
    'py2exe': {"includes": ["lxml._elementpath"]}
    },
    data_files=depen
)
    