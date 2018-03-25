#include "XFFmpeg.h"

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")

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
		else if (enc->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioStream = i;
			AVCodec *codec = avcodec_find_decoder(enc->codec_id);
			if (!codec)
			{
				mutex.unlock();
				printf("audio code not find!\n");
				return 0;
			}

			if (avcodec_open2(enc, codec, nullptr) < 0)
			{
				mutex.unlock();
				printf("audio can not open!\n");
				return 0;
			}

			this->sampleRate = enc->sample_rate;
			this->channel = enc->channels;
			switch (enc->sample_fmt)
			{
			case AV_SAMPLE_FMT_U8:
				break;
			case AV_SAMPLE_FMT_S16:
				this->sampleSize = 16;
				break;
			case AV_SAMPLE_FMT_S32:
				this->sampleSize = 32;
				break;
			case AV_SAMPLE_FMT_FLT:
				this->sampleSize = 32;
				break;
			case AV_SAMPLE_FMT_DBL:
				this->sampleSize = 32;
				break;
			default:
				break;
			}

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

	if (aCtx)
	{
		swr_free(&aCtx);

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

int XFFmpeg::GetPts(const AVPacket * pkt)
{
	mutex.lock();
	if (!ic)
	{
		mutex.unlock();
		return -1;
	}
	int p = pkt->pts * r2d(ic->streams[pkt->stream_index]->time_base) * 1000;
	
	mutex.unlock();
	return p;
}

int XFFmpeg::Decode(const AVPacket * pkt)
{
	mutex.lock();
	if (!ic)
	{
		mutex.unlock();
		return 0;
	}

	if (yuv == nullptr)
	{
		yuv = av_frame_alloc();
	}
	if (pcm == nullptr)
	{
		pcm = av_frame_alloc();
	}

	AVFrame *frame = yuv;
	if (pkt->stream_index == audioStream)
	{
		frame = pcm;
	}

	int re = avcodec_send_packet(ic->streams[pkt->stream_index]->codec, pkt);
	if (re != 0)
	{
		mutex.unlock();
		return 0;
	}
	re = avcodec_receive_frame(ic->streams[pkt->stream_index]->codec, frame);
	if (re != 0)
	{
		mutex.unlock();
		return 0;
	}
	mutex.unlock();
	int p = frame->pts * r2d(ic->streams[pkt->stream_index]->time_base) * 1000;
	if (pkt->stream_index == audioStream)
	{
		this->pts = p;
	}
	return pts;
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

int XFFmpeg::ToPCM(char * out)
{
	mutex.lock();
	if (!ic || !pcm || !out)
	{
		mutex.unlock();
		return 0;
	}

	AVCodecContext *ctx = ic->streams[audioStream]->codec;
	if (aCtx == nullptr)
	{
		aCtx = swr_alloc();
		swr_alloc_set_opts(aCtx, ctx->channel_layout,
			AV_SAMPLE_FMT_S16,
			ctx->sample_rate, ctx->channels, ctx->sample_fmt,
			ctx->sample_rate,
			0, 0);
		swr_init(aCtx);
	}

	uint8_t *data[1];
	data[0] = (uint8_t*)out;
	int len = swr_convert(aCtx, data, 10000,
		(const uint8_t **)pcm->data,
		pcm->nb_samples);
	if (len <= 0)
	{
		mutex.unlock();
		return 0;
	}
	int outsize = av_samples_get_buffer_size(nullptr, ctx->channels,
		pcm->nb_samples, AV_SAMPLE_FMT_S16, 0);
	
	mutex.unlock();
	return outsize;
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
