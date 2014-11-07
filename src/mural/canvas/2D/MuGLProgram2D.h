#pragma once

#include "../gl.h"
#include "../../../common/StringUtil.h"

namespace mural
{

enum {
    kMuGLProgram2DAttributePos,
    kMuGLProgram2DAttributeUV,
    kMuGLProgram2DAttributeColor
};

class MuGLProgram2D
{
    GLuint program;
    GLuint screen;

    void bindAttributeLocations();
    void getUniforms();

    static GLint compileShaderFile(const String& file, GLenum type);
    static GLint compileShaderSource(const String& source, GLenum type);
    static void linkProgram(GLuint program);

public:
    MuGLProgram2D();
    ~MuGLProgram2D();

    bool initWithShaders(const String& vertexShaderFile, const String& fragmentShaderFile);

    GLuint getProgram() const { return program; }

    GLuint getScreen() const { return screen; }
    //TODO: const GLuint getTranslate() { return translate; }
};

}
