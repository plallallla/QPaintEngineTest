#pragma once
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QPainter>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QVector2D>
#include <QVector3D>
#include <qDebug>

#if defined(ROIVIEW_EXPORT) 
#define ROIVIEW_CLASS_EXPORT __declspec(dllexport) 
#else 
#define ROIVIEW_CLASS_EXPORT __declspec(dllimport) 
#endif

class ROIBaseItem :
	public QGraphicsItem
{
public:
	enum TYPE
	{
		QUADRI,//四边形
		RECT,//矩形
	};
	enum POSITION
	{
		COMMON,
		UP_LEFT,
		UP_MID,
		UP_RIGHT,
		LEFT_MID,
		RIGHT_MID,
		DOWN_LEFT,
		DOWN_MID,
		DOWN_RIGHT,
	};
	enum CHANGEMODE
	{
		NONE,
		REANGEL,
		ROTATE,
		RESIZE,
	};
	QPainterPath shape() const;
	QRectF boundingRect() const override;
	virtual TYPE Type() = 0;
protected:

	bool Approach(QPointF p1, QPointF p2);
	QPainterPath m_shape;
	QRectF m_boundary;
};

class ROIVIEW_CLASS_EXPORT ROIQuadriItem :
	public ROIBaseItem
{
public:
	//构造与析构
	explicit ROIQuadriItem();
	explicit ROIQuadriItem(qreal v, qreal b, qreal angel);
	~ROIQuadriItem();
	//功能函数
	void SetEnable(CHANGEMODE tag, bool able = false);
	//重载函数
	TYPE Type() override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
protected:
	//事件响应函数,移动、形变均基于以下函数的联动
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
	void Reset(qreal v, qreal b, qreal angel);
	void SetTempShow();
	QPolygonF MakePolygon(qreal v, qreal b, qreal angel) const;
	POSITION WhereIsPos(QPointF);
private:
	bool m_bRotate = true;
	bool m_bResize = true;
	bool m_bReangel = true;
	qreal m_dbVerticalLength;//水平边边长
	qreal m_dbBevelLength;//斜边边长
	qreal m_dbAngle;//夹角角度
	QPolygonF m_Polygon;//生成多边形
	QGraphicsPolygonItem m_tempShow;//预览图形
	QPointF m_ptReangel;//夹角改变锚点
	QPointF m_ptRotate;//标注旋转锚点
	QPointF m_ptResize;//大小改变锚点
	CHANGEMODE m_changemode;//变化模式
	QPointF m_ptClick;//点击记录点
	QPointF m_ptULStable;//左上角稳定点坐标

	//旋转优化
	QTransform m_RotateTransform;
	qreal m_angle = 0;
};