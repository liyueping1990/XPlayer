#include "XPlayer.h"
#include <QtWidgets/QApplication>
//#include "XFFmpeg.h"

int main(int argc, char *argv[])
{
	//XFFmpeg *xFFmpeg = XFFmpeg::Get();
	//bool isOpen = xFFmpeg->Open("video_mv.mp4");
	//if (isOpen)
	//{
	//	printf("open success!\n");
	//}
	//else
	//{
	//	printf("open failed!--%s", xFFmpeg->GetError().c_str());
	//	return -1;
	//}

	//char* rgb = new char[800 * 600 * 4];
	//for (;;)
	//{
	//	AVPacket pkt = xFFmpeg->Read();

	//	if (pkt.size == 0)
	//	{
	//		break;
	//	}
	//	printf("pts=%lld\n", pkt.pts);
	//	if (pkt.stream_index != xFFmpeg->videoStream)
	//	{
	//		av_packet_unref(&pkt);
	//		continue;
	//	}
	//	AVFrame* yuv = xFFmpeg->Decode(&pkt);
	//	if (yuv)
	//	{
	//		printf("[D]");
	//		xFFmpeg->ToRGB(yuv, rgb, 800, 600);
	//	}
	//	

	//	av_packet_unref(&pkt);
	//}

	//av_register_all();
	//char *path = "video_mv.mp4";
	//AVFormatContext *ic = nullptr;
	//// 打开视频
	//int re = avformat_open_input(&ic, path, nullptr, nullptr);
	//if (re != 0)
	//{
	//	char buf[1024] = { 0 };
	//	av_strerror(re, buf, sizeof(buf));
	//	printf("open %s failed:%s\n", path, buf);
	//	getchar();
	//	return -1;
	//}
	//
	//// 获取视频总时长
	//int totalSec = ic->duration / AV_TIME_BASE;
	//printf("file totalSec is %d-%d\n", totalSec / 60, totalSec % 60);
	//// 查找码流解码器 
	//int videoStream = 0;
	//AVCodecContext *videoCtx = nullptr;
	//for (int i = 0; i < ic->nb_streams; i++)
	//{
	//	AVCodecContext *enc = ic->streams[i]->codec;
	//	if (enc->codec_type == AVMEDIA_TYPE_VIDEO)
	//	{
	//		videoCtx = enc;
	//		videoStream = i;
	//		AVCodec *codec = avcodec_find_decoder(enc->codec_id);
	//		if (!codec)
	//		{
	//			printf("video code not find!\n");
	//			return -1;
	//		}
	//		int err = avcodec_open2(enc, codec, nullptr);
	//		if (err != 0)
	//		{
	//			char buf[1024] = { 0 };
	//			av_strerror(err, buf, sizeof(buf));
	//			printf(buf);
	//			return -2;
	//		}
	//		printf("open codec success\n");
	//	}
	//}

	//// 视频解码
	//AVFrame* yuv = av_frame_alloc();
	//SwsContext *cCtx = nullptr;
	//int outWidth = 800;
	//int outHeight = 600;
	//char* rgb = new char[outWidth * outHeight * 4];
	//for (;;)
	//{
	//	AVPacket pkt;
	//	re = av_read_frame(ic, &pkt);
	//	if (re != 0)
	//	{
	//		break;
	//	}

	//	if (pkt.stream_index != videoStream)
	//	{
	//		av_packet_unref(&pkt);
	//		continue;
	//	}

	//	int pts = pkt.pts * r2d(ic->streams[pkt.stream_index]->time_base) * 1000;
	//	
	//	// 打开视频帧
	//	int re = avcodec_send_packet(videoCtx, &pkt);
	//	if (re !=0)
	//	{
	//		av_packet_unref(&pkt);
	//		continue;
	//	}
	//	re = avcodec_receive_frame(videoCtx, yuv);
	//	if (re != 0)
	//	{
	//		av_packet_unref(&pkt);
	//		continue;
	//	}
	//	printf("[%D]", re);

	//	cCtx = sws_getCachedContext(cCtx,
	//		videoCtx->width,
	//		videoCtx->height,
	//		videoCtx->pix_fmt,
	//		outWidth,
	//		outHeight,
	//		AV_PIX_FMT_BGRA,
	//		SWS_BICUBIC,
	//		nullptr,
	//		nullptr,
	//		nullptr
	//	);
	//	if (!cCtx)
	//	{
	//		printf("sws_getCachedContext failed!\n");
	//	}

	//	uint8_t *data[AV_NUM_DATA_POINTERS];
	//	data[0] = (uint8_t*)rgb;
	//	int lineSize[AV_NUM_DATA_POINTERS] = { 0 };
	//	lineSize[0] = outWidth * 4;
	//	int h = sws_scale(cCtx, yuv->data, yuv->linesize, 0, videoCtx->height,
	//		data,
	//		lineSize); // 返回转码后高度
	//	if (h > 0)
	//	{
	//		printf("(%d)", h);
	//	}
	//	
	//	//int got_picture = 0;
	//	//int re = avcodec_decode_video2(videoCtx, yuv,&got_picture, &pkt);
	//	//if (got_picture)
	//	//{
	//	//	printf("[%d]", re);
	//	//}

	//	printf("pts=%lld\n", pts);
	//	av_packet_unref(&pkt);
	//}
	//if (cCtx)
	//{
	//	sws_freeContext(cCtx);
	//	cCtx = nullptr;
	//}

	//avformat_close_input(&ic);

	QApplication a(argc, argv);
	XPlayer w;
	w.show();
	return a.exec();
}
