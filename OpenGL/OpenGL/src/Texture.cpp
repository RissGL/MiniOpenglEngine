#include "Texture.h"

#include "stb_image/stb_image.h"

#include "Debug/Debug.h"


Texture::Texture(const std::string& path)
	:m_RenderID(0),m_FilePath(path),m_LocalBuffer(nullptr),m_Width(0),m_Height(0),m_BBP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(),&m_Width,&m_Height,&m_BBP,4);

	GLCALL(glGenTextures(1, &m_RenderID));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_RenderID));

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE,m_LocalBuffer));
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GLCALL(glDeleteTextures(1, &m_RenderID));
}

void Texture::UnBind()const
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Bind(unsigned int slot)const
{
	GLCALL(glActiveTexture(GL_TEXTURE0+slot));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_RenderID));
}