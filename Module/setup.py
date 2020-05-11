from distutils.core import setup, Extension

module = Extension('Complexity',
                    sources = ['main.c'])

setup (name = 'Complexity',
       version = '1.0',
       description = 'Idk',
       ext_modules = [module])
