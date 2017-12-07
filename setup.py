from distutils.core import setup, Extension

module = Extension('pyvicon_module',
                    sources=['pyvicon/pyvicon.cpp'],
                    libraries=['pyvicon/ViconDataStreamSDK_CPP'])

setup(name='pyvicon',
      version='0.1',
      description='Python wrapper over Vicon DataStream SDK',
      ext_modules=[module])