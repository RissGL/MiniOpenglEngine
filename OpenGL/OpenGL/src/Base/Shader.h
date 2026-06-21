#pragma once
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include "Material/LightAndMaterial.h"

class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void Bind()const;
	void UnBind()const;
	void Reload();

	//Set Uniform
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniformMat4f(const std::string& name,const glm::mat4& proj);

	void SetUniformMaterial(const std::string& name, const Material& material);

private:
	unsigned int m_RendererID;
	std::string m_VertexPath;
	std::string m_FragmentPath;
	mutable std::unordered_map<std::string, int> m_UniformLocationCaChe;

	int GetUniformLocation(const std::string& name)const;

	std::string ReadFile(const std::string& filepath);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	unsigned int CreateShader(const std::string& vertexShaer, const std::string& fragmentShader);

};
