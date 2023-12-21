#include "GdiPaint.h"
#include "Test.h"
#include <QPixMap>
#include <QtWin>
#include <qDebug>
#include <QPainter>
#include <QLabel>
#include "MFCpaint.h"

//����ͼ�����ROIView������ͬһ��stackwidget��
GdiPaint::GdiPaint(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	ui.stackedWidget->setCurrentIndex(0);
	//viewer�����ͼƬ
	connect(ui.pushButton_open, &QPushButton::pressed, [=]() {
		ui.viewer->OpenPicture("src.png");
	});
	//viewer��������ı���
	connect(ui.pushButton_quadri, &QPushButton::pressed, [=]() {
		ui.viewer->AddQuadri();
	});
	//���뺯��ָ����ʵ�ֶ�̬��ͼ������ָ������Ϊvoid(*)(HWND)
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
	//����DrawBlueʹ�ñ�����ɫΪ��ɫ
	connect(ui.pushButton_blue, &QPushButton::pressed, [=]() {
		ui.MFCcanvas->SetDraw(DrawBlue);
	});
	//����DrawGreenʹ�ñ�����ɫΪ��ɫ
	connect(ui.pushButton_green, &QPushButton::pressed, [=]() {
		ui.MFCcanvas->SetDraw(DrawGreen);
	});
	//����DrawRedʹ�ñ�����ɫΪ��ɫ
	connect(ui.pushButton_red, &QPushButton::pressed, [=]() {
		ui.MFCcanvas->SetDraw(DrawRed);
	});
}

GdiPaint::~GdiPaint()
{}

