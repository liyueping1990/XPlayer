#include "XPlayer.h"
#include <QtWidgets/QApplication>

extern "C" {
#include "libavformat/avformat.h"
}

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")

int main(int argc, char *argv[])
{
	av_register_all();
	char *path = "video.mp4";
	AVFormatContext *ic = nullptr;
	int re = avformat_open_input(&ic, path, nullptr, nullptr);
	if (re != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf));
		printf("open %s failed:%s\n", path, buf);
		getchar();
		return -1;
	}
	
	int totalSec = ic->duration / AV_TIME_BASE;
	printf("file totalSec is %d-%d\n", totalSec / 60, totalSec % 60);
	for (;;)
	{
		AVPacket pkt;
		re = av_read_frame(ic, &pkt);
		if (re != 0)
		{
			break;
		}
		printf("pts=%lld\n", pkt.pts);
		av_packet_unref(&pkt);
	}


	avformat_close_input(&ic);
	

	QApplication a(argc, argv);
	XPlayer w;
	w.show();
	return a.exec();
}
