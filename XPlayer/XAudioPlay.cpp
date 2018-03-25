#include "XAudioPlay.h"
#include "qaudiooutput.h"
#include "qmutex.h"

class CXAudioPlay : public XAudioPlay
{
public:
	//CXAudioPlay();
	//~CXAudioPlay();
	QAudioOutput * output = nullptr;
	QIODevice *io = nullptr;
	QMutex mutex;
	virtual bool Start()
	{
		Stop();
		mutex.lock();
		QAudioFormat fmt;
		fmt.setSampleRate(this->sampleRate);
		fmt.setSampleSize(this->sampleSize);
		fmt.setChannelCount(this->channel);
		fmt.setCodec("audio/pcm");
		fmt.setByteOrder(QAudioFormat::LittleEndian);
		fmt.setSampleType(QAudioFormat::UnSignedInt);

		output = new QAudioOutput(fmt);		
		io = output->start();
		mutex.unlock();
		return true;
	}

	virtual void Stop()
	{
		mutex.lock();
		if (output)
		{
			output->stop();
			delete output;
			output = nullptr;
			io = nullptr;
		}
		mutex.unlock();
	}

	virtual void Play(bool isPlay)
	{
		mutex.lock();
		if (!output)
		{
			mutex.unlock();
			return;
		}

		if (isPlay)
		{
			output->resume();
		}
		else
		{
			output->suspend();
		}
		mutex.unlock();
		return;
	}

	virtual int GetFree()
	{
		mutex.lock();
		if (!output)
		{
			mutex.unlock();
			return 0;
		}
		
		int free = output->bytesFree();
		mutex.unlock();
		return free;
	}

	virtual bool Write(const char*data, int datasize)
	{
		mutex.lock();
		if (io)
		{
			io->write(data, datasize);
		}
		mutex.unlock();
		
		return true;
	}

private:

};

XAudioPlay* XAudioPlay::Get()
{
	static CXAudioPlay ap;
	return &ap;
}

//CXAudioPlay::CXAudioPlay()
//{
//}
//
//CXAudioPlay::~CXAudioPlay()
//{
//}

XAudioPlay::~XAudioPlay()
{
}

XAudioPlay::XAudioPlay()
{
}
