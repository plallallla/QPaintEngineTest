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
		QUADRI,//�ı���
		RECT,//����
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
	//����������
	explicit ROIQuadriItem();
	explicit ROIQuadriItem(qreal v, qreal b, qreal angel);
	~ROIQuadriItem();
	//���ܺ���
	void SetEnable(CHANGEMODE tag, bool able = false);
	//���غ���
	TYPE Type() override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
protected:
	//�¼���Ӧ����,�ƶ����α���������º���������
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
	qreal m_dbVerticalLength;//ˮƽ�߱߳�
	qreal m_dbBevelLength;//б�߱߳�
	qreal m_dbAngle;//�нǽǶ�
	QPolygonF m_Polygon;//���ɶ����
	QGraphicsPolygonItem m_tempShow;//Ԥ��ͼ��
	QPointF m_ptReangel;//�нǸı�ê��
	QPointF m_ptRotate;//��ע��תê��
	QPointF m_ptResize;//��С�ı�ê��
	CHANGEMODE m_changemode;//�仯ģʽ
	QPointF m_ptClick;//�����¼��
	QPointF m_ptULStable;//���Ͻ��ȶ�������

	//��ת�Ż�
	QTransform m_RotateTransform;
	qreal m_angle = 0;
};