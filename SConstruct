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
env.Append(CPPPATH = ['/usr/local/include', 'src/juce_core'])
env.Append(LIBPATH = ['/usr/local/lib'])
env.Append(LIBS = ['juce_core', 'glfw3'])
if env['mode'] == 'debug':
    env.Append(LIBPATH=['lib/juce_core/debug'])
else:
    env.Append(LIBPATH=['lib/juce_core/release'])

# Build
# - Packages
sources = Split("""
    src/mural/AppViewController.cc
    src/mural/JavaScriptView.cc
    src/mural/MuLocalStorage.cc
    src/mural/canvas/MuCanvas.cc
    src/mural/canvas/MuCanvasContext.cc
    src/mural/canvas/MuImage.cc
    src/mural/canvas/MuSharedTextureCache.cc
    src/mural/canvas/MuTexture.cc
    src/mural/canvas/MuTextureStorage.cc
    src/mural/canvas/2D/MuCanvasContext2D.cc
    src/mural/canvas/2D/MuCanvasContext2DScreen.cc
    src/mural/canvas/2D/MuCanvasContext2DTexture.cc
    src/mural/canvas/2D/MuGLProgram2D.cc
    src/mural/canvas/2D/MuPath.cc
    src/common/MuAffineTransform.cc
    src/common/BindUtil.cc
    src/common/FileUtil.cc
    src/common/StringUtil.cc
    src/common/MuOperationQueue.cc
    src/duktape/duktape.c
    src/main.cc
""")
env.Program('#bin/%s/mural' % env['mode'], sources)
