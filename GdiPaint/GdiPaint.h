#pragma once

#include <QtWidgets/QWidget>
#include "ui_GdiPaint.h"

class GdiPaint : public QWidget
{
    Q_OBJECT

public:
    GdiPaint(QWidget *parent = nullptr);
    ~GdiPaint();
	//void paintEvent(QPaintEvent *event);
	//QPaintEngine *paintEngine() const override;

private:
    Ui::GdiPaintClass ui;
	bool MFCtag = false;
};
