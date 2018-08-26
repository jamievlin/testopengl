#!/usr/bin/env python3
import os

env = Environment()

BOOST_LIB='D:/dev/boost_1_68_0'

sources = ['main.cc', 'callback.cc', 'color.cc', 
    'shadersproc.cc', 'window.cc', 'objparser.cc']

if os.name != 'nt':
    sources.append('glpng.cc')

if os.name == 'nt':
    # cl.exe
    env.Append(TARGET_ARCH='x86_64')
    env.Append(CXXFLAGS=['/std:c++14', '/Za', '/DNO_PNG',
                         '/DGLFW_DLL', '/DBOOST_NO_EXCEPTIONS'])
    env.Append(CPPPATH=['./msvc/include'])

    env.Append(LIBPATH=['./msvc/libs'])

    env.Append(LIBS=['opengl32', 'glew32', 'glu32', 'gdi32',
                     'glfw3dll'])

    # boost library here
    env['CPPPATH'].append(BOOST_LIB)
    env['LIBPATH'].append(BOOST_LIB + '/stage/lib')



env.Program(target='testogl', source=sources)
