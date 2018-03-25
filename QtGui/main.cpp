#include "QtGui.h"
#include <QtWidgets/QApplication>
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

#pragma comment(lib, "avformat.lib")

int main(int argc, char *argv[])
{
	av_register_all();
	QApplication a(argc, argv);
	QtGui w;
	w.show();
	return a.exec();
}
