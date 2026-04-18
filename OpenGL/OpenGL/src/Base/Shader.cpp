#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include "Debug/Debug.h"


Shader::Shader(const std::string& filepath)
	:m_RendererID(0),filepath(filepath)
{
    auto [vertexShader,fragmentShader] = ParseShader(filepath);

    m_RendererID = CreateShader(vertexShader, fragmentShader);
}

Shader::~Shader() 
{
    GLCALL(glDeleteProgram(m_RendererID));

}

void Shader::Bind() const
{
    GLCALL(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
    GLCALL(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCALL(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCALL(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniformMat4f(const std::string& name,const glm::mat4& matrix)
{
    GLCALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE,&matrix[0][0]));
}

void Shader::SetUniformMaterial(const std::string& name, const Material& material)
{
    SetUniform1i(name + ".diffuse", 0);//漫反射槽
    SetUniform1i(name + ".specular", 1);//镜面反射槽
    SetUniform1i(name + ".emission", 2);
    SetUniform1f(name + ".shininess", material.shininess);
}

void Shader::SetUniformLight(const std::string& name, const Light& light)
{
    SetUniform3f(name + ".lightPos", light.lightPos.x, light.lightPos.y, light.lightPos.z);
    SetUniform3f(name + ".ambient", light.ambient.x, light.ambient.y, light.ambient.z);
    SetUniform3f(name + ".diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
    SetUniform3f(name + ".specular", light.specular.x, light.specular.y, light.specular.z);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCALL(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

int Shader::GetUniformLocation(const std::string& name) const
{
    if (m_UniformLocationCaChe.find(name) != m_UniformLocationCaChe.end())
        return m_UniformLocationCaChe[name];
    
    GLCALL(int location= glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "警告 uniform" << name << "不存在" << std::endl;

    m_UniformLocationCaChe[name] = location;
    return location;
}

unsigned int Shader::CreateShader(const std::string& vertexShaer, const std::string& fragmentShader)
{
    unsigned int program;
    GLCALL(program = glCreateProgram());
    unsigned int vs = CompileShader(vertexShaer, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    GLCALL(glAttachShader(program, vs));
    GLCALL(glAttachShader(program, fs));

    GLCALL(glLinkProgram(program));
    GLCALL(glValidateProgram(program));

    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));

    return program;
}

unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
    // 这里所有的 gl 调用都应该被 GLCALL 包裹
    unsigned int id;
    GLCALL(id = glCreateShader(type));
    const char* src = source.c_str();
    GLCALL(glShaderSource(id, 1, &src, nullptr));
    GLCALL(glCompileShader(id));

    int result;
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(sizeof(char) * length);
        GLCALL(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "编译" << (type == GL_VERTEX_SHADER ? "顶点着色器" : "片段着色器") << "失败" << std::endl;
        std::cout << message << std::endl;
        GLCALL(glDeleteShader(id));
        return 0;
    }

    return id;
}


std::tuple<std::string, std::string> Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        None = -1, VertexShader = 0, FragementShader = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType shaderType = ShaderType::None;

    if (!stream.is_open()) {
        std::cout << "失败！没找到文件路径: " << filepath << std::endl;
        return { "", "" };
    }

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                shaderType = ShaderType::VertexShader;
            else if (line.find("fragment") != std::string::npos)
                shaderType = ShaderType::FragementShader;
        }
        else
        {
            if (shaderType != ShaderType::None)
                ss[(int)shaderType] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}