#include "OGLProgram.h"
#include "Common.h"
#include <glad/glad.h>
#include <fstream>
#include <vector>

static std::string ReadFile(const std::string& InPath) 
{
    std::ifstream InputStream(InPath, std::ios::ate);
    DIE_ON_ERROR(InputStream.is_open(), "Impossible to open file");

    size_t Size = InputStream.tellg();

    std::string Text;
    Text.resize(Size);

    InputStream.seekg(0, std::ios::beg);
    InputStream.read(&Text[0], Size);

    InputStream.close();
    return Text;
}


static GLuint CreateShader(const std::string& InPath, GLuint InShaderType)
{
    //Read file
    std::string Text = ReadFile(InPath);
    const char* ShaderSource = Text.c_str();

    GLuint ShaderId = glCreateShader(InShaderType);
    glShaderSource(ShaderId, 1, &ShaderSource, NULL);
    glCompileShader(ShaderId);

    GLint Success;
    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Success);
    if (!Success) 
    {
        GLint MaxLogLength;
        glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &MaxLogLength);

        std::vector<GLchar> InfoLog(MaxLogLength);
        glGetShaderInfoLog(ShaderId, MaxLogLength, NULL, InfoLog.data());

        std::string LogStr(InfoLog.begin(), InfoLog.end());
        DIE(LogStr);
    }

    //Load and Compile on GPU
    return ShaderId;
}

static GLuint CreateProgram(GLuint VertexId, GLuint FragmentId)
{
    GLuint ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexId);
    glAttachShader(ProgramId, FragmentId);
    glLinkProgram(ProgramId);

    GLint Success;
    glGetProgramiv(ProgramId, GL_LINK_STATUS, &Success);
    if (!Success) 
    {
        GLint MaxLogLength;
        glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &MaxLogLength);

        std::vector<GLchar> InfoLog(MaxLogLength);
        glGetProgramInfoLog(ProgramId, MaxLogLength, NULL, InfoLog.data());

        std::string LogStr(InfoLog.begin(), InfoLog.end());
        DIE(LogStr);
    }

    glDeleteShader(VertexId);
    glDeleteShader(FragmentId);
    return ProgramId;
}



OGLProgram::OGLProgram(const std::string& InVertShaderPath, const std::string& InFragShaderPath)
{
    GLuint VertexShaderId = CreateShader(InVertShaderPath, GL_VERTEX_SHADER);
    GLuint FragmeShaderId = CreateShader(InFragShaderPath, GL_FRAGMENT_SHADER);
    ProgramId = CreateProgram(VertexShaderId, FragmeShaderId);
}

OGLProgram::~OGLProgram()
{
    glDeleteProgram(ProgramId);
}

void OGLProgram::Bind()
{
    glUseProgram(ProgramId);
}

GLuint OGLProgram::ID() 
{
    return ProgramId;
}

void OGLProgram::SetUniform(const std::string& name, const Color& color) {
       glUniform4fv(glGetUniformLocation(ID(), name.c_str()), 1, (GLfloat*)&color);
}

void OGLProgram::SetUniform(const std::string& name, float value) {
       glUniform1f(glGetUniformLocation(ID(), name.c_str()), value);
}

void OGLProgram::SetUniform(const std::string& name, glm::mat4 matrix) 
{
    glUniformMatrix4fv(glGetUniformLocation(ID(), name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}


void OGLProgram::SetUniform(const std::string& name, glm::vec3 vect) 
{
    glUniform3fv(glGetUniformLocation(ID(), name.c_str()), 1, &vect[0]);
}