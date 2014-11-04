#include "MuGLProgram2D.h"
#include "../../../common/FileUtil.h"
#include <cstdlib>

namespace mural
{

MuGLProgram2D::MuGLProgram2D():
    program(0),
    screen(0)
{}

MuGLProgram2D::~MuGLProgram2D()
{
    if (this->program) {
        glDeleteProgram(this->program);
    }
}

bool MuGLProgram2D::initWithShaders(const String& vsFile, const String& fsFile)
{
    this->program = glCreateProgram();
    GLuint vs = this->compileShaderFile(vsFile, GL_VERTEX_SHADER);
    GLuint fs = this->compileShaderFile(fsFile, GL_FRAGMENT_SHADER);

    glAttachShader(this->program, vs);
    glAttachShader(this->program, fs);

    this->bindAttributeLocations();
    this->linkProgram(this->program);

    this->getUniforms();

    glDeleteProgram(vs);
    glDeleteProgram(fs);

    return true;
}

void MuGLProgram2D::bindAttributeLocations()
{
    glBindAttribLocation(this->program, kMuGLProgram2DAttributePos, "pos");
    glBindAttribLocation(this->program, kMuGLProgram2DAttributeUV, "uv");
    glBindAttribLocation(this->program, kMuGLProgram2DAttributeColor, "color");
}
void MuGLProgram2D::getUniforms()
{
    this->screen = glGetUniformLocation(this->program, "screen");
}

GLint MuGLProgram2D::compileShaderFile(const String& file, GLenum type)
{
    return compileShaderSource(readWholeFile(file), type);
}
GLint MuGLProgram2D::compileShaderSource(const String& source, GLenum type)
{
    const GLchar *glSource = (GLchar *)source.c_str();

    GLint shader = glCreateShader(type);
    glShaderSource(shader, 1, &glSource, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetShaderInfoLog(shader, logLength, &logLength, log);
            printf("Shader compile log:\n%s", log);
            free(log);
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}
void MuGLProgram2D::linkProgram(GLuint program)
{
    GLint status;
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == 0) {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetProgramInfoLog(program, logLength, &logLength, log);
            printf("Program link log:\n%s", log);
            free(log);
        }
    }
}

}
