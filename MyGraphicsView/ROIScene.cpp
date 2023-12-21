#include "ROIScene.h"
#include <qdebug.h>
ROIScene::ROIScene(QWidget *parent) : QGraphicsScene(parent)
{

}

ROIScene::~ROIScene()
{

}

bool ROIScene::SetBackground(const QString & filename)
{
	if (!background.load(filename))
	{
		QFile file(filename);
		if (!file.open(QIODevice::ReadOnly))
		{
			return false;
		}
		background.loadFromData(file.readAll());
	}
	QGraphicsView* pView = views().first();
	QRectF b = background.rect();
	QRectF a = QRectF(b.x() - b.width(), b.y() - b.height(), 3 * b.width(), 3 * b.height());
	setSceneRect(a);
	update();
	return true;
}

void ROIScene::SetBackground(const QImage * imageSrc)
{
	background = *imageSrc;
	QGraphicsView* pView = views().first();
	QRectF b = background.rect();
	QRectF a = QRectF(b.x() - b.width(), b.y() - b.height(), 3 * b.width(), 3 * b.height());
	setSceneRect(a);
	update();
}

QRectF ROIScene::GetShowRect()
{
	return background.rect();
}

QImage ROIScene::GetBackGround()
{
	return background;
}

void ROIScene::removeBackGround()
{
	background.load("");
	update();
}

void ROIScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	if (views().count() == 0)
	{
		return;
	}
	painter->drawImage(background.rect(), background, QRect());
}
