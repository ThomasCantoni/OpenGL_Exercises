#pragma once

#include <glad/glad.h>
#include <string>

class OGLTexture 
{
    public:
        OGLTexture(const std::string& Path);
        ~OGLTexture();
        void Bind(GLenum TextureUnit);
    private:
       GLuint Id; 
};