#pragma once
#include <iostream>
#include <string>
#include <qmutex.h>

extern "C" {
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}

class XFFmpeg
{
public:
	static XFFmpeg *Get()
	{
		static XFFmpeg ff;
		return &ff;
	}

	int Open(const char* path);
	void Close();
	AVPacket Read(); // 返回值需要用户清理

	int GetPts(const AVPacket *pkt);

	int Decode(const AVPacket *pkt);

	bool ToRGB(char* out, int outWidth, int outHeight);
	int ToPCM(char* out);

	// 0~1之间
	bool Seek(float pos);

	std::string GetError();
	virtual ~XFFmpeg();
	int totalMs = 0;
	int fps = 0;
	int pts = 0;
	bool isPlay = false;
	int videoStream = 0;
	int audioStream = 1;

	int sampleRate = 48000;
	int sampleSize = 16;
	int channel = 2;
protected:
	char errorBuf[1024];
	AVFormatContext *ic = nullptr;
	AVFrame* yuv = nullptr;
	AVFrame* pcm = nullptr;
	SwsContext *cCtx = nullptr;
	SwrContext *aCtx = nullptr;
	QMutex mutex;
	XFFmpeg();
};

