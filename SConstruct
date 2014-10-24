vars = Variables()

# 3 build modes
vars.Add(EnumVariable('mode', 'Building mode', 'debug', allowed_values=('debug', 'profile', 'release')))

env = Environment(
    CC = 'clang',
    CXX = 'clang++',
    variables = vars
)
Export('env')
Help(vars.GenerateHelpText(env))

# basic flags according to build mode
if env['mode'] == 'debug':
    env.Append(CCFLAGS = ['-Wall', '-g', '-O0', '-DDEBUG'])
elif env['mode'] == 'release':
    env.Append(CCFLAGS = ['-Wall', '-O3', '-DNDEBUG'])
    # env.Append(LINKFLAGS = ['-s'])
elif env['mode'] == 'profile':
    env.Append(CCFLAGS = ['-Wall', '-pg', '-O0', '-DNDEBUG'])

# env.Append(CCFLAGS = ['-DLINUX'])
env.Append(CXXFLAGS = ['-std=c++11'])

# Frameworks
env.Append(FRAMEWORKS = [
    'Cocoa',
    'OpenGL',
    'IOKit',
    'CoreVideo'
])

# Libs
env.Append(CPPPATH = ['/usr/local/include'])
env.Append(LIBPATH = ['/usr/local/lib'])
env.Append(LIBS = ['glfw3'])

# Build
# - Packages
sources = Split("""
    src/berry/AppViewController.cc
    src/berry/Canvas.cc
    src/berry/Core.cc
    src/berry/JavaScriptView.cc
    src/berry/LocalStorage.cc
    src/common/FileUtil.cc
    src/common/StringUtil.cc
    src/common/Object.cc
    src/duktape/duktape.c
    src/main.cc
""")
env.Program('#bin/%s/berry' % env['mode'], sources)
