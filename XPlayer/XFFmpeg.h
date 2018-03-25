#pragma once
#include <iostream>
#include <string>
#include <qmutex.h>

extern "C" {
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
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

	AVFrame* Decode(const AVPacket *pkt);

	bool ToRGB(char* out, int outWidth, int outHeight);

	// 0~1之间
	bool Seek(float pos);

	std::string GetError();
	virtual ~XFFmpeg();
	int totalMs = 0;
	int fps = 0;
	int pts = 0;
	bool isPlay = false;
	int videoStream = 0;
protected:

	char errorBuf[1024];
	AVFormatContext *ic = nullptr;
	AVFrame* yuv = nullptr;
	SwsContext *cCtx = nullptr;
	QMutex mutex;
	XFFmpeg();
};

