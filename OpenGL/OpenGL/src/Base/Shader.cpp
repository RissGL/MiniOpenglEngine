#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include "Debug/Debug.h"


Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	:m_RendererID(0), m_VertexPath(vertexPath), m_FragmentPath(fragmentPath)
{
    std::string vertexSource = ReadFile(vertexPath);
    std::string fragmentSource = ReadFile(fragmentPath);

    m_RendererID = CreateShader(vertexSource, fragmentSource);
}

void Shader::Reload()
{
    std::string vertexSource = ReadFile(m_VertexPath);
    std::string fragmentSource = ReadFile(m_FragmentPath);

    unsigned int newProgram = CreateShader(vertexSource, fragmentSource);
    if (newProgram == 0) {
        std::cout << "Shader重载失败，保留旧程序" << std::endl;
        return;
    }

    GLCALL(glDeleteProgram(m_RendererID));
    m_RendererID = newProgram;
    m_UniformLocationCaChe.clear();
    std::cout << "Shader重载成功: " << m_VertexPath << std::endl;
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

std::string Shader::ReadFile(const std::string& filepath)
{
    std::ifstream stream(filepath);
    if (!stream.is_open()) {
        std::cout << "错误：找不到 Shader 文件: " << filepath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << stream.rdbuf(); // 一次性把文件全部读进流里
    return buffer.str();      // 转成 string 返回
}

unsigned int Shader::CreateShader(const std::string& vertexShaer, const std::string& fragmentShader)
{
    unsigned int program;
    GLCALL(program = glCreateProgram());
    unsigned int vs = CompileShader(vertexShaer, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    if (vs == 0 || fs == 0) {
        if (vs) GLCALL(glDeleteShader(vs));
        if (fs) GLCALL(glDeleteShader(fs));
        GLCALL(glDeleteProgram(program));
        return 0;
    }

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


