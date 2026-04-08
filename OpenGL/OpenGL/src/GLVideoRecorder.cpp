#include "GLVideoRecorder.h"
#include "imgui/imgui.h"
#include <GL/glew.h>
#include <iostream>

GLVideoRecorder::GLVideoRecorder()
    : m_FFmpegPipe(nullptr), m_IsRecording(false), m_FrameCount(0), m_TargetFrames(0),
    m_EnableAudio(false), m_DurationSeconds(5), m_FPS(60)
{
    strcpy_s(m_FFmpegPath, "D:\\×ÀÃæ\\TOOL\\ffmpeg-8.0.1-full_build\\bin\\ffmpeg.exe");
    strcpy_s(m_OutputVideo, "opengl_render.mkv"); 
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

    m_PixelBuffer.resize(width * height * 3);

    std::string cmd = std::string(m_FFmpegPath);

    cmd += " -y -f rawvideo -vcodec rawvideo";
    cmd += " -s " + std::to_string(width) + "x" + std::to_string(height);
    cmd += " -pix_fmt rgb24 -r " + std::to_string(m_FPS) + " -i - ";

    if (m_EnableAudio) {
        cmd += "-i \"" + std::string(m_InputAudio) + "\" ";
    }

    cmd += "-vf vflip ";

    cmd += "-c:v libx264 -preset fast -crf 18 -pix_fmt yuv420p ";

    if (m_EnableAudio) {
        cmd += "-c:a flac ";
    }

    cmd += "-t " + std::to_string(m_DurationSeconds) + " ";
    cmd += "\"" + std::string(m_OutputVideo) + "\"";

    std::cout << "Starting FFmpeg pipe:\n" << cmd << std::endl;

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
        _pclose(m_FFmpegPipe); 
        m_FFmpegPipe = nullptr;
    }
    m_IsRecording = false;
    std::cout << "Recording Finished! Saved to " << m_OutputVideo << std::endl;
}

void GLVideoRecorder::CaptureFrame(int width, int height)
{
    if (!m_IsRecording || !m_FFmpegPipe) return;

    if (m_PixelBuffer.size() != width * height * 3) {
        m_PixelBuffer.resize(width * height * 3);
    }

    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, m_PixelBuffer.data());

    fwrite(m_PixelBuffer.data(), 1, m_PixelBuffer.size(), m_FFmpegPipe);

    m_FrameCount++;

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
        if (ImGui::Button("START RECORDING", ImVec2(200, 50))) {
            StartRecording(currentWidth, currentHeight);
        }
    }
    else {
        float progress = (float)m_FrameCount / (float)m_TargetFrames;
        ImGui::ProgressBar(progress, ImVec2(200, 50));
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "RECORDING... DO NOT RESIZE WINDOW");

        if (ImGui::Button("FORCE STOP", ImVec2(200, 30))) {
            StopRecording();
        }
    }

    ImGui::End();
}