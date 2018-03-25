#include "XFFmpeg.h"

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "swscale.lib")

static double r2d(AVRational r)
{
	return r.num == 0 || r.den == 0 ? 0. : (double)r.num / (double)r.den;
}

XFFmpeg::XFFmpeg()
{
	errorBuf[0] = '\0';
	av_register_all();
}


XFFmpeg::~XFFmpeg()
{
}

int XFFmpeg::Open(const char * path)
{
	Close();
	mutex.lock();
	int re = avformat_open_input(&ic, path, nullptr, nullptr);
	if (re != 0)
	{
		mutex.unlock();
		av_strerror(re, errorBuf, sizeof(errorBuf));
		printf("open %s failed:%s\n", path, errorBuf);		
		return 0;
	}
	totalMs = (ic->duration / AV_TIME_BASE * 1000);
	
	for (int i = 0; i < ic->nb_streams; i++)
	{
		AVCodecContext *enc = ic->streams[i]->codec;
		if (enc->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			fps = r2d(ic->streams[i]->avg_frame_rate);
			AVCodec *codec = avcodec_find_decoder(enc->codec_id);
			if (!codec)
			{
				mutex.unlock();
				printf("video code not find!\n");
				return 0;
			}
			int err = avcodec_open2(enc, codec, nullptr);
			if (err != 0)
			{
				mutex.unlock();
				char buf[1024] = { 0 };
				av_strerror(err, buf, sizeof(buf));
				printf(buf);
				return 0;
			}
			printf("open codec success\n");
		}
	}

	mutex.unlock();
	return totalMs;
}

void XFFmpeg::Close()
{
	mutex.lock();
	if (ic)
	{
		avformat_close_input(&ic);
	}

	if (yuv)
	{
		av_frame_free(&yuv);
	}

	if (cCtx)
	{
		sws_freeContext(cCtx);
		cCtx = nullptr;
	}
	mutex.unlock();
}

AVPacket XFFmpeg::Read()
{
	AVPacket pkt;
	memset(&pkt, 0, sizeof(AVPacket));
	mutex.lock();
	if (!ic)
	{
		mutex.unlock();
		return pkt;
	}
	int err = av_read_frame(ic, &pkt);
	if (err != 0)
	{
		av_strerror(err, errorBuf, sizeof(errorBuf));
	}

	mutex.unlock();
	return pkt;
}

AVFrame * XFFmpeg::Decode(const AVPacket * pkt)
{
	mutex.lock();
	if (!ic)
	{
		mutex.unlock();
		return nullptr;
	}

	if (yuv == nullptr)
	{
		yuv = av_frame_alloc();
	}

	int re = avcodec_send_packet(ic->streams[pkt->stream_index]->codec, pkt);
	if (re != 0)
	{
		mutex.unlock();
		return nullptr;
	}
	re = avcodec_receive_frame(ic->streams[pkt->stream_index]->codec, yuv);
	if (re != 0)
	{
		mutex.unlock();
		return nullptr;
	}
	mutex.unlock();
	pts = yuv->pts * r2d(ic->streams[pkt->stream_index]->time_base) * 1000;
	return yuv;
}

bool XFFmpeg::ToRGB(char * out, int outWidth, int outHeight)
{
	mutex.lock();
	if (!ic || !yuv)
	{
		mutex.unlock();
		return false;
	}

	AVCodecContext *videoCtx = ic->streams[this->videoStream]->codec;
	cCtx = sws_getCachedContext(cCtx,
		videoCtx->width,
		videoCtx->height,
		videoCtx->pix_fmt,// 可能失败
		outWidth,
		outHeight,
		AV_PIX_FMT_BGRA,
		SWS_BICUBIC,
		nullptr,
		nullptr,
		nullptr
	);
	if (!cCtx)
	{
		mutex.unlock();
		printf("sws_getCachedContext failed!\n");
		return false;
	}

	uint8_t *data[AV_NUM_DATA_POINTERS];
	data[0] = (uint8_t*)out;
	int lineSize[AV_NUM_DATA_POINTERS] = { 0 };
	lineSize[0] = outWidth * 4;
	int h = sws_scale(cCtx, yuv->data, yuv->linesize, 0, videoCtx->height,
		data,
		lineSize); // 返回转码后高度
	if (h > 0)
	{
		printf("(%d)", h);
	}

	mutex.unlock();
	return false;
}

bool XFFmpeg::Seek(float pos)
{
	mutex.lock();
	if (!ic)
	{
		mutex.unlock();
		return false;
	}

	int64_t stamp = 0;
	stamp = pos * ic->streams[videoStream]->duration;
	int re = av_seek_frame(ic, videoStream, stamp,
		AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);
	avcodec_flush_buffers(ic->streams[videoStream]->codec);
	pts = stamp * r2d(ic->streams[videoStream]->time_base) * 1000;
	
	mutex.unlock();
	if (re >= 0)
	{
		return true;
	}

	return false;
}

std::string XFFmpeg::GetError()
{
	mutex.lock();
	std::string re = this->errorBuf;
	mutex.unlock();
	return re;
}
