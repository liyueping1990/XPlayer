#include "XPlayer.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
static bool isPressSlider = false;
static bool isPlay = false;
#define PAUSE "border-image:url(:/XPlayer/Resources/pause_normal.png);"
#define PLAY "border-image:url(:/XPlayer/Resources/play_normal.png);"

XPlayer::XPlayer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	startTimer(10);
}

XPlayer::~XPlayer()
{
}

void XPlayer::resizeEvent(QResizeEvent * e)
{
	ui.openGLWidget->resize(size());
	ui.playButton->move(this->width() / 2, this->height() - 70);
	ui.openButton->move(this->width() / 2 - 80, this->height() - 70);

	ui.playTime->move(40, this->height() - 70);
	ui.totalTime->move(100, this->height() - 70);

	ui.playSlider->resize(this->width() - 80, 20);
	ui.playSlider->move(40, this->height() - 100);
}


void XPlayer::openFile(QString name)
{
	if (name.isEmpty())
	{
		return;
	}
	this->setWindowTitle(name);
	int totalMs = XFFmpeg::Get()->Open(name.toLocal8Bit());
	if (totalMs <= 0)
	{
		QMessageBox::information(this, "err", "file open failed!");
		return;
	}

	XAudioPlay::Get()->sampleRate = XFFmpeg::Get()->sampleRate;
	XAudioPlay::Get()->channel = XFFmpeg::Get()->channel;
	XAudioPlay::Get()->sampleSize = 16;
	XAudioPlay::Get()->Start();

	char buf[1024] = { 0 };
	int hour = ((totalMs / 1000) / 60) / 60;
	int min = ((totalMs / 1000) / 60) % 60;
	int sec = (totalMs / 1000) % 60;
	sprintf(buf, "/%02d:%02d:%02d", hour, min, sec);
	ui.totalTime->setText(buf);

	isPlay = false;
	play();
}

void XPlayer::open()
{
	QString name = QFileDialog::getOpenFileName(
		this, QString::fromLocal8Bit("选择视频文件"));
	openFile(name);

}

void XPlayer::timerEvent(QTimerEvent * e)
{
	char buf[1024] = { 0 };
	int hour = ((XFFmpeg::Get()->pts / 1000) / 60) / 60;
	int min = ((XFFmpeg::Get()->pts / 1000) / 60) % 60;
	int sec = (XFFmpeg::Get()->pts / 1000) % 60;
	sprintf(buf, "%02d:%02d:%02d", hour, min, sec);
	ui.playTime->setText(buf);

	if (XFFmpeg::Get()->totalMs > 0)
	{
		float rate = (float)(XFFmpeg::Get()->pts) / (float)(XFFmpeg::Get()->totalMs);
		if (!isPressSlider)
		{
			ui.playSlider->setValue(rate * 1000);
		}
	}
}

void XPlayer::play()
{
	isPlay = !isPlay;
	XFFmpeg::Get()->isPlay = isPlay;
	if (isPlay)
	{
		// pause
		ui.playButton->setStyleSheet(PAUSE);
	}
	else
	{
		ui.playButton->setStyleSheet(PLAY);
	}
}

void XPlayer::sliderPress()
{
	isPressSlider = true;
}

void XPlayer::sliderRelease()
{
	float pos = 0;
	pos = (float)ui.playSlider->value() / (float)(ui.playSlider->maximum() + 1);
	XFFmpeg::Get()->Seek(pos);
	isPressSlider = false;
}
