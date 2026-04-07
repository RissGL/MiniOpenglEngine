#include "GLVideoRecorder.h"
#include "imgui/imgui.h"
#include <GL/glew.h>
#include <iostream>

GLVideoRecorder::GLVideoRecorder()
    : m_FFmpegPipe(nullptr), m_IsRecording(false), m_FrameCount(0), m_TargetFrames(0),
    m_EnableAudio(false), m_DurationSeconds(5), m_FPS(60)
{
    strcpy_s(m_FFmpegPath, "D:\\桌面\\TOOL\\ffmpeg-8.0.1-full_build\\bin\\ffmpeg.exe");
    strcpy_s(m_OutputVideo, "opengl_render.mkv"); // 推荐 MKV 封装 FLAC
    strcpy_s(m_InputAudio, "bgm.flac");
}

GLVideoRecorder::~GLVideoRecorder()
{
    StopRecording();
}

void GLVideoRecorder::StartRecording(int width, int height)
{
    if (m_IsRecording) return;

    m_FrameCount = 0;
    m_TargetFrames = m_DurationSeconds * m_FPS;

    // 预分配显存抓取所需的内存
    m_PixelBuffer.resize(width * height * 3);

    // 构建 FFmpeg 管道命令
    std::string cmd = std::string(m_FFmpegPath);

    // 告诉 FFmpeg 我们会通过标准输入 (stdin) 喂给它纯 RGB 数据
    cmd += " -y -f rawvideo -vcodec rawvideo";
    cmd += " -s " + std::to_string(width) + "x" + std::to_string(height);
    cmd += " -pix_fmt rgb24 -r " + std::to_string(m_FPS) + " -i - ";

    // 加入音频
    if (m_EnableAudio) {
        cmd += "-i \"" + std::string(m_InputAudio) + "\" ";
    }

    // OpenGL 的原点在左下角，而视频标准在左上角，所以需要用 -vf vflip 把画面上下翻转过来
    cmd += "-vf vflip ";

    // 视频编码设置 (高质量，快预设)
    cmd += "-c:v libx264 -preset fast -crf 18 -pix_fmt yuv420p ";

    // 音频编码设置
    if (m_EnableAudio) {
        cmd += "-c:a flac ";
    }

    // 限制录制时长
    cmd += "-t " + std::to_string(m_DurationSeconds) + " ";
    cmd += "\"" + std::string(m_OutputVideo) + "\"";

    std::cout << "Starting FFmpeg pipe:\n" << cmd << std::endl;

    // 使用 _popen 打开一个向 FFmpeg 写入二进制数据的进程管道 (Windows 专属写法)
    m_FFmpegPipe = _popen(cmd.c_str(), "wb");

    if (m_FFmpegPipe) {
        m_IsRecording = true;
    }
    else {
        std::cerr << "Failed to start FFmpeg!" << std::endl;
    }
}

void GLVideoRecorder::StopRecording()
{
    if (m_FFmpegPipe) {
        _pclose(m_FFmpegPipe); // 关闭管道，FFmpeg 会自动完成视频封装并保存
        m_FFmpegPipe = nullptr;
    }
    m_IsRecording = false;
    std::cout << "Recording Finished! Saved to " << m_OutputVideo << std::endl;
}

void GLVideoRecorder::CaptureFrame(int width, int height)
{
    // 如果没有在录制，直接跳过，完全不影响渲染性能
    if (!m_IsRecording || !m_FFmpegPipe) return;

    // 如果窗口大小变化，重新分配内存
    if (m_PixelBuffer.size() != width * height * 3) {
        m_PixelBuffer.resize(width * height * 3);
    }

    // 1. 设置 OpenGL 像素对齐
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    // 2. 从当前 Framebuffer 读取当前帧所有的 RGB 像素
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, m_PixelBuffer.data());

    // 3. 把读取到的像素数据通过管道“塞”进 FFmpeg 里
    fwrite(m_PixelBuffer.data(), 1, m_PixelBuffer.size(), m_FFmpegPipe);

    m_FrameCount++;

    // 到达设定时长，自动停止
    if (m_FrameCount >= m_TargetFrames) {
        StopRecording();
    }
}

void GLVideoRecorder::OnImGuiRender(int currentWidth, int currentHeight)
{
    ImGui::Begin("OpenGL Video Recorder");

    ImGui::InputText("FFmpeg Path", m_FFmpegPath, sizeof(m_FFmpegPath));

    ImGui::Separator();
    ImGui::Checkbox("Add Background Music", &m_EnableAudio);
    if (m_EnableAudio) {
        ImGui::InputText("Audio File", m_InputAudio, sizeof(m_InputAudio));
    }

    ImGui::Separator();
    ImGui::SliderInt("Duration (Sec)", &m_DurationSeconds, 1, 600);
    ImGui::SliderInt("FPS", &m_FPS, 24, 144);

    ImGui::InputText("Output File", m_OutputVideo, sizeof(m_OutputVideo));

    ImGui::Spacing();

    if (!m_IsRecording) {
        // 注意：我们这里硬编码传了 1000x1000，因为你 Application 里创建的窗口就是这个大小
        // 如果窗口大小能调整，你需要传动态的宽高进来
        if (ImGui::Button("START RECORDING", ImVec2(200, 50))) {
            StartRecording(currentWidth, currentHeight);
        }
    }
    else {
        // 显示录制进度
        float progress = (float)m_FrameCount / (float)m_TargetFrames;
        ImGui::ProgressBar(progress, ImVec2(200, 50));
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "RECORDING... DO NOT RESIZE WINDOW");

        if (ImGui::Button("FORCE STOP", ImVec2(200, 30))) {
            StopRecording();
        }
    }

    ImGui::End();
}