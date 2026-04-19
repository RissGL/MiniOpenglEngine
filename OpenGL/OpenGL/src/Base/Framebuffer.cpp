#include "Framebuffer.h"
#include "Debug/Debug.h"

Framebuffer::Framebuffer(unsigned int width, unsigned int height)
    : m_Width(width), m_Height(height)
{
    Invalidate();
}

Framebuffer::~Framebuffer()
{
    GLCALL(glDeleteFramebuffers(1, &m_RendererID));
    GLCALL(glDeleteTextures(1, &m_ColorAttachment));
    GLCALL(glDeleteRenderbuffers(1, &m_DepthAttachment));
}

void Framebuffer::Invalidate()
{
    // 如果已经存在，先删掉旧的（在 Resize 时会用到）
    if (m_RendererID)
    {
        GLCALL(glDeleteFramebuffers(1, &m_RendererID));
        GLCALL(glDeleteTextures(1, &m_ColorAttachment));
        GLCALL(glDeleteRenderbuffers(1, &m_DepthAttachment));
    }

    GLCALL(glGenFramebuffers(1, &m_RendererID));
    GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

    GLCALL(glGenTextures(1, &m_ColorAttachment));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    // 将纹理挂载到 FBO 的颜色槽 0 上
    GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));


    GLCALL(glGenRenderbuffers(1, &m_DepthAttachment));
    GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment));
    GLCALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height));
    GLCALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment));

    // 检查 FBO 是否完整无缺
    ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    // 解绑，恢复原状
    GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::Bind() const
{
    GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
    GLCALL(glViewport(0, 0, m_Width, m_Height));
}

void Framebuffer::UnBind() const
{
    GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::Resize(unsigned int width, unsigned int height)
{
    if (width == 0 || height == 0 || (m_Width == width && m_Height == height))
        return;

    m_Width = width;
    m_Height = height;
    Invalidate(); // 重新生成 FBO
}

int Framebuffer::ReadPixel(int x, int y) const
{
    GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
    GLCALL(glReadBuffer(GL_COLOR_ATTACHMENT0));

    unsigned char pixelData[4]; // RGBA
    // 读取指定坐标的 1x1 像素区块
    GLCALL(glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixelData));

    return (int)pixelData[0];
}