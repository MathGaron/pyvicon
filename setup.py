from distutils.core import setup, Extension

module = Extension('pyvicon_module',
                    sources=['pyvicon/pyvicon.cpp'],
                    libraries=['ViconDataStreamSDK_CPP'])

setup(name='pyvicon',
      version='0.1',
      description='Python wrapper over Vicon DataStream SDK',
      license='GPL-3.0',
      author='Mathieu Garon',
      author_email='mathieugaron91@gmail.com',
      packages=['pyvicon'],
      ext_modules=[module])