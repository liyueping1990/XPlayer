#include "XVideoThread.h"
#include "XFFmpeg.h"
#include "XAudioPlay.h"
#include "list"

bool isExit = false;

static std::list<AVPacket> videos;
static int apts = -1;

void XVideoThread::run()
{
	char out[10000] = { 0 };
	while (!isExit)
	{
		if (!XFFmpeg::Get()->isPlay)
		{
			msleep(10);
			continue;
		}

		while (videos.size() > 0)
		{
			AVPacket pack = videos.front();
			int pts = XFFmpeg::Get()->GetPts(&pack);
			if (pts > apts)
			{
				break;
			}
			XFFmpeg::Get()->Decode(&pack);
			av_packet_unref(&pack);
			videos.pop_front();
		}

		int free = XAudioPlay::Get()->GetFree();
		if (free < 10000)
		{
			msleep(1);
			continue;
		}

		// ��ȡ����Ƶ
		AVPacket pkt = XFFmpeg::Get()->Read();
		if (pkt.size <= 0)
		{
			msleep(10);
		}

		if (pkt.stream_index == XFFmpeg::Get()->audioStream)
		{
			apts = XFFmpeg::Get()->Decode(&pkt);
			av_packet_unref(&pkt);
			int len = XFFmpeg::Get()->ToPCM(out);
			XAudioPlay::Get()->Write(out, len);

			continue;
		}

		if (pkt.stream_index != XFFmpeg::Get()->videoStream)
		{
			av_packet_unref(&pkt);
			continue;
		}

		//XFFmpeg::Get()->Decode(&pkt);
		//av_packet_unref(&pkt);
		videos.push_back(pkt);

		//if (XFFmpeg::Get()->fps >0)
		//{
		//	msleep(1000 / XFFmpeg::Get()->fps);
		//}
	}
}

XVideoThread::XVideoThread()
{
}


XVideoThread::~XVideoThread()
{
}
