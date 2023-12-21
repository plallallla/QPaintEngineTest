#include "GdiPaint.h"
#include "MFCpaint.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	GdiPaint w;
    w.show();
    return a.exec();
}
