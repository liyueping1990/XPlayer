#include "XVideoThread.h"
#include "XFFmpeg.h"

bool isExit = false;

void XVideoThread::run()
{
	while (!isExit)
	{
		if (!XFFmpeg::Get()->isPlay)
		{
			msleep(10);
			continue;
		}

		AVPacket pkt = XFFmpeg::Get()->Read();
		if (pkt.size <= 0)
		{
			msleep(10);
		}

		if (pkt.stream_index != XFFmpeg::Get()->videoStream)
		{
			av_packet_unref(&pkt);
			continue;
		}

		XFFmpeg::Get()->Decode(&pkt);
		av_packet_unref(&pkt);

		if (XFFmpeg::Get()->fps >0)
		{
			msleep(1000 / XFFmpeg::Get()->fps);
		}
	}
}

XVideoThread::XVideoThread()
{
}


XVideoThread::~XVideoThread()
{
}
