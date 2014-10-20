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

# Build
# - Packages
sources = Split("""
    src/berry/core.cc
    src/common/object.cc
    src/duktape/duktape.c
    src/main.cc
""")
env.Program('#bin/%s/berry' % env['mode'], sources)
