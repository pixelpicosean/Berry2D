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
env.Append(CXXFLAGS = ['-std=c++11', '-stdlib=libc++'])

# Frameworks
env.Append(FRAMEWORKS = [
    'Cocoa',
    'OpenGL',
    'IOKit',
    'CoreVideo',
    'libpng'
])

# Libs
env.Append(CPPPATH = ['/usr/local/include'])
env.Append(LIBPATH = ['/usr/local/lib'])
env.Append(LIBS = ['glfw3'])

# Build
# - Packages
sources = Split("""
    src/mural/AppViewController.cc
    src/mural/JavaScriptView.cc
    src/mural/LocalStorage.cc
    src/mural/canvas/Canvas.cc
    src/mural/canvas/CanvasContext.cc
    src/mural/canvas/Image.cc
    src/mural/canvas/SharedTextureCache.cc
    src/mural/canvas/Texture.cc
    src/mural/canvas/TextureStorage.cc
    src/common/BindUtil.cc
    src/common/FileUtil.cc
    src/common/StringUtil.cc
    src/common/MuOperationQueue.cc
    src/duktape/duktape.c
    src/main.cc
""")
env.Program('#bin/%s/mural' % env['mode'], sources)
