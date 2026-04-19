#pragma once
#include <GL/glew.h>
#include <iostream>

class Framebuffer
{
public:
    inline unsigned int GetWidth() const { return m_Width; }
    inline unsigned int GetHeight() const { return m_Height; }

    Framebuffer(unsigned int width, unsigned int height);
    ~Framebuffer();

    void Bind() const;

    void UnBind() const;

    void Resize(unsigned int width, unsigned int height);

    unsigned int GetColorAttachment() const { return m_ColorAttachment; }

    int ReadPixel(int x, int y) const;

private:
    unsigned int m_RendererID = 0;
    unsigned int m_ColorAttachment = 0;
    unsigned int m_DepthAttachment = 0;
    unsigned int m_Width, m_Height;

    void Invalidate();
};