#include "MFCpaint.h"
#include "Test.h"
#include <qDebug>

MFCpaint::MFCpaint(QWidget *parent, void(*MFCdraw)(HWND))
	: QWidget(parent), draw(MFCdraw)
{
	setAutoFillBackground(false);
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);
}

MFCpaint::~MFCpaint()
{}

void MFCpaint::SetDraw(void(*tag)(HWND))
{
	draw = tag;
	update();
}

void MFCpaint::paintEvent(QPaintEvent * event)
{
	if (draw)
	{
		draw(HWND(winId()));
	}
	else
	{
		CDCtest(HWND(winId()));
	}
}

QPaintEngine * MFCpaint::paintEngine() const
{
	return nullptr;
}
