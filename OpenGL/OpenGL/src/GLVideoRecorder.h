#pragma once
#include <string>
#include <vector>
#include <stdio.h>

class GLVideoRecorder
{
public:
    GLVideoRecorder();
    ~GLVideoRecorder();

    void OnImGuiRender();

    void CaptureFrame(int windowWidth, int windowHeight);
    void OnImGuiRender(int currentWidth, int currentHeight);

private:
    void StartRecording(int width, int height);
    void StopRecording();

private:
    FILE* m_FFmpegPipe;      // 指向 FFmpeg 进程的管道流
    bool m_IsRecording;
    int m_FrameCount;        // 当前录制的帧数
    int m_TargetFrames;      // 目标需要录制的总帧数

    std::vector<unsigned char> m_PixelBuffer; // 像素缓冲区

    // 录制参数
    char m_FFmpegPath[256];
    char m_OutputVideo[256];
    char m_InputAudio[256];

    bool m_EnableAudio;
    int m_DurationSeconds;
    int m_FPS;               // 录制帧率
};