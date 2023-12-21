#pragma once
#include <qfile.h>
#include <QGraphicsScene>
#include <qgraphicsview.h>

//这是基于QGraphicsScene派生的Scene类
//支持设置背景图片，支持获取显示区域矩形与显示图片构造的Image类
class ROIScene :
	public QGraphicsScene
{
public:
	//默认空构造函数
	ROIScene(QWidget *parent = nullptr);
	//默认空析构函数
	~ROIScene();
	//设置图片背景，成功返回true
	bool SetBackground(const QString &);
	void SetBackground(const QImage *);
	//返回实际显示图形区域
	QRectF GetShowRect();
	//返回背景图片构造的Image对象
	QImage GetBackGround();
	//移除背景图片
	void removeBackGround();
protected:
	void drawBackground(QPainter *painter, const QRectF &rect) override;
private:
	QImage background;
};

