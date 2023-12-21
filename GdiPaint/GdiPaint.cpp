#include "GdiPaint.h"
#include "Test.h"
#include <QPixMap>
#include <QtWin>
#include <qDebug>
#include <QPainter>
#include <QLabel>
#include "MFCpaint.h"

//将绘图组件与ROIView叠加在同一个stackwidget中
GdiPaint::GdiPaint(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	ui.stackedWidget->setCurrentIndex(0);
	//viewer组件打开图片
	connect(ui.pushButton_open, &QPushButton::pressed, [=]() {
		ui.viewer->OpenPicture("src.png");
	});
	//viewer组件加入四边形
	connect(ui.pushButton_quadri, &QPushButton::pressed, [=]() {
		ui.viewer->AddQuadri();
	});
	//传入函数指针以实现动态绘图，函数指针类型为void(*)(HWND)
	connect(ui.pushButton_test, &QPushButton::pressed, [=]() {
		if (MFCtag)
		{
			ui.stackedWidget->setCurrentIndex(0);
			ui.pushButton_open->setEnabled(true);
			ui.pushButton_quadri->setEnabled(true);
			ui.pushButton_blue->setEnabled(false);
			ui.pushButton_red->setEnabled(false);
			ui.pushButton_green->setEnabled(false);
		}
		else
		{
			ui.stackedWidget->setCurrentIndex(1);
			ui.pushButton_open->setEnabled(false);
			ui.pushButton_quadri->setEnabled(false);
			ui.pushButton_blue->setEnabled(true);
			ui.pushButton_red->setEnabled(true);
			ui.pushButton_green->setEnabled(true);
			ui.MFCcanvas->SetDraw(CDCtest);
		}
		MFCtag = !MFCtag;
	});
	//传入DrawBlue使得背景颜色为蓝色
	connect(ui.pushButton_blue, &QPushButton::pressed, [=]() {
		ui.MFCcanvas->SetDraw(DrawBlue);
	});
	//传入DrawGreen使得背景颜色为绿色
	connect(ui.pushButton_green, &QPushButton::pressed, [=]() {
		ui.MFCcanvas->SetDraw(DrawGreen);
	});
	//传入DrawRed使得背景颜色为红色
	connect(ui.pushButton_red, &QPushButton::pressed, [=]() {
		ui.MFCcanvas->SetDraw(DrawRed);
	});
}

GdiPaint::~GdiPaint()
{}

