#pragma once
#include  "Base/Renderer.h"

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot)const;
	void UnBind()const;

	inline int GetWidth() { return m_Width; }
	inline int GetHeight(){ return m_Height; }

private:
	unsigned int  m_RenderID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BBP;
};
