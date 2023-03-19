#pragma once

#include <string>
#include <vector>
#include <list>
#include <functional>

#include "utils/gl_utils.h"


#define MAX_2D_TEXTURES        (16)
#define INVALID_LOC            (-1)
#define MAX_LIGHTING_SOURCES   (100)


class Shader
{
public:
    Shader(const std::string& name);
    ~Shader();

    const char* GetName() const;
    GLuint GetProgramID() const;

    void Use() const;
    unsigned int Reload();

    void AddShader(const std::string& shaderFile, GLenum shaderType);
    void AddShaderCode(const std::string& shaderCode, GLenum shaderType);
    void ClearShaders();
    unsigned int CreateAndLink();

    void BindTexturesUnits();
    GLint GetUniformLocation(const char* uniformName) const;

    void OnLoad(std::function<void()> onLoad);

private:
    void GetUniforms();
    static unsigned int CreateShader(const std::string& shaderFile, GLenum shaderType);
    static unsigned int CompileShader(const std::string shaderCode, GLenum shaderType);
    static unsigned int CreateProgram(const std::vector<unsigned int>& shaderObjects);

public:
    GLuint program;

    // Lighting
    GLint ltype[MAX_LIGHTING_SOURCES];
    GLint lposition[MAX_LIGHTING_SOURCES];
    GLint lcolor[MAX_LIGHTING_SOURCES];
    GLint lintensity[MAX_LIGHTING_SOURCES];
    GLint ldirection[MAX_LIGHTING_SOURCES];
    GLint lcutoff[MAX_LIGHTING_SOURCES];

    // Textures
    GLint loc_textures[MAX_2D_TEXTURES];

    // MVP
    GLint loc_model_matrix;
    GLint loc_view_matrix;
    GLint loc_projection_matrix;

    // General
    GLint loc_resolution;


private:
    struct ShaderFile
    {
        std::string file;
        GLenum type;
    };

    std::string shaderName;
    std::vector<ShaderFile> shaderFiles;
    std::vector<ShaderFile> shaderCodes;
    std::list<std::function<void()>> loadObservers;
};