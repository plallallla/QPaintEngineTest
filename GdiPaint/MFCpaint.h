#pragma once

#include <QWidget>
#include <QPaintEngine>
#include <QPaintEvent>
class MFCpaint  : public QWidget
{
	Q_OBJECT

public:
	MFCpaint(QWidget *parent = nullptr, void(*MFCdraw)(HWND) = nullptr);
	~MFCpaint();
	void SetDraw(void(*tag)(HWND));
	void paintEvent(QPaintEvent *event);
	QPaintEngine *paintEngine() const override;
private:
	void(*draw)(HWND);
};
