#!/usr/bin/env python3
import os

if os.name == 'nt':
    env = Environment(tools=['msvc', 'default'])
    BOOST_LIB = 'D:/dev/boost_1_68_0'
else:
    env = Environment(tools=['clangxx', 'g++', 'default'])
    BOOST_LIB = ''

sources = ['main.cc', 'callback.cc', 'color.cc', 
    'shadersproc.cc', 'window.cc', 'objparser.cc']
flags = []
libs = []

if os.name == 'nt':
    # cl.exe
    libs.extend(['opengl32', 'glew32', 'glu32', 'gdi32','glfw3dll'])
    flags.extend(['NO_PNG', 'GLFW_DLL', 'BOOST_NO_EXCEPTIONS'])

    env.Append(TARGET_ARCH='x86_64')
    env.Append(CXXFLAGS=['/std:c++14', '/Za'])

    for flag in flags:
        env['CXXFLAGS'].append('/D{0}'.format(flag))

    env.Append(CPPPATH=['./msvc/include'])
    env.Append(LIBPATH=['./msvc/libs'])

    # boost library here
    env['CPPPATH'].append(BOOST_LIB)
    env['LIBPATH'].append(BOOST_LIB + '/stage/lib')
else:
    sources.append('glpng.cc')
    libs.extend(['glfw', 'GL', 'GLU', 'boost_system', 'GLEW', 'png'])

    env.Append(CXXFLAGS=['-std=c++14', '-g', '-Wall'])

    for flag in flags:
        env['CXXFLAGS'].append('-D{0}'.format(flag))



env.Append(LIBS=libs)
env.Program(target='testopengl', source=sources)
