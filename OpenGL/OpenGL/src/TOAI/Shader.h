#pragma once
#include <string>
#include <unordered_map>
#include <tuple>

#include "glm/glm.hpp"

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind()const;
	void UnBind()const;

	//Set Uniform
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform1i(const std::string& name, int value);
	void SetUniformMat4f(const std::string& name,const glm::mat4& proj);

private:
	unsigned int m_RendererID;
	std::string filepath;
	mutable std::unordered_map<std::string, int> m_UniformLocationCaChe;

	int GetUniformLocation(const std::string& name)const;
	std::tuple<std::string,std::string> ParseShader(const std::string& filepath);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	unsigned int CreateShader(const std::string& vertexShaer, const std::string& fragmentShader);

};
