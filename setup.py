#!/usr/bin/env python

import sys
from distutils.core import setup, Extension
import os
#os.environ["CC"] = "clang"
#os.environ["CXX"] = "clang++"

if sys.version_info[0]==3:
    boost_python_lib='boost_python3'
else:
    boost_python_lib='boost_python'


module1=Extension('pyef.native',
                      include_dirs=[],
                      libraries=[boost_python_lib],
                      library_dirs=[],
                      extra_compile_args=['-std=c++17','-O1'],
                      sources=['pyef.cpp', 'data_file.cpp'],
                      language='c++')
#module1.extra_compile_args+=['-DUSE_OMP_TRANSFORM','-fopenmp']
#module1.extra_link_args=['-fopenmp']



setup(name='native',
		  packages=['pyef'],
          version='1.0',
          ext_modules=[module1])
