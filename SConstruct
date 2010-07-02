import os

env = Environment(ENV = os.environ)
env.Append(LIBS = ['GL', 'GLU', 'm'])

print(GetLaunchDir())

dependencies = []
def add_library(env, name, dir):
    global linker_flags, dependencies
    library = env.SConscript(dirs = dir)
    env.Append(**library.get('environment', {}))
    env.StaticLibrary('lib' + name, [(dir + '/' + file) for file in library['files']])
    env.Depends('lib' + name, [('lib' + dep) for dep in library.get('dependencies', {})])
    dependencies.append('lib' + name + '.a')
    env.Append(LIBS = [name])

if ARGUMENTS.get('debug', False):
    env.Append(CPPFLAGS = ['-g'])
else:
    env.Append(CPPFLAGS = ['-O3'])
    
# zlib
add_library(env, 'zlib', 'source/zlib')

# Corona (needs libjpeg, libpng, libungif, zlib)
add_library(env, 'jpeg', 'source/corona/jpeg-6b')
add_library(env, 'png', 'source/corona/libpng-1.2.1')
add_library(env, 'corona', 'source/corona')
add_library(env, 'ungif', 'source/corona/libungif-4.1.0')

# zziplib (needs zlib)
add_library(env, 'zzip', 'source/zzip')

# lua
add_library(env, 'lua', 'source/lua')

# plum (needs everything)
e = env.Clone()

e.ParseConfig('sdl-config --cflags --libs')
e.ParseConfig('audiere-config --cxxflags --libs')
e.Append(LIBPATH = ['.'])
e.Append(CPPDEFINES = ['ZZIP_HAVE_STDINT_H', 'HAVE_INTTYPES_H'])
e.Append(CPPPATH = [
    'source/zlib',
    'source/corona',
    'source/corona/jpeg-6b',
    'source/corona/libpng-1.2.1',
    'source/corona/libungif-4.1.0',
    'source/SDL-1.2.13/include',
    'source/lua',
    'source/audiere/src',
    'source',
])
PLUM_DIR = 'source/Plum'
plum = e.SConscript(dirs = PLUM_DIR)
e.Program('test/plum', [(PLUM_DIR + '/' + file) for file in plum])
e.Depends('test/plum', dependencies)

e.Command('test/maximumyak/plum', 'test/plum', 'cp test/plum test/maximumyak/plum')
