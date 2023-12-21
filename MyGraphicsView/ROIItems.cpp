#include "ROIItems.h"

inline double ToRadian(double x)
{
	return 3.1415 / 180 * (x);
}

inline double ToAngle(double x)
{
	return 180 / 3.1415 * (x);
}

QPainterPath ROIBaseItem::shape() const
{
	return m_shape;
}

QRectF ROIBaseItem::boundingRect() const
{
	return m_boundary;
}

bool ROIBaseItem::Approach(QPointF p1, QPointF p2)
{
	if ((p1 - p2).manhattanLength() <= 6)
	{
		return true;
	}
	return false;
}

ROIQuadriItem::ROIQuadriItem()
{
}

ROIQuadriItem::ROIQuadriItem(qreal v, qreal b, qreal angel)
{
	setAcceptHoverEvents(true);
	Reset(v, b, angel);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
}

ROIQuadriItem::~ROIQuadriItem()
{
}

void ROIQuadriItem::SetEnable(CHANGEMODE tag, bool able)
{
	switch (tag)
	{
	case ROIQuadriItem::ROTATE:
		m_bRotate = able;
		return;
	case ROIQuadriItem::RESIZE:
		m_bResize = able;
		return;
	case ROIQuadriItem::REANGEL:
		m_bReangel = able;
		return;
	}
}


ROIQuadriItem::TYPE ROIQuadriItem::Type()
{
	return ROIQuadriItem::QUADRI;
}


void ROIQuadriItem::Reset(qreal v, qreal b, qreal angel)
{
	//设置四边形属性
	m_dbVerticalLength = v;
	m_dbBevelLength = b;
	m_dbAngle = angel;
	m_Polygon.clear();
	m_Polygon = MakePolygon(v, b, angel);
	//设置画布为四边形外接矩形
	m_boundary = m_Polygon.boundingRect();
	m_boundary = QRectF(m_Polygon.boundingRect().x() - 10, m_Polygon.boundingRect().y() - 10,
		m_Polygon.boundingRect().width() + 20, m_Polygon.boundingRect().height() + 20);
	//设置点击判定范围为四边形周边
	QPainterPath big, small;
	big.addPolygon(MakePolygon(v + 10, b + 10, angel));
	small.addPolygon(MakePolygon(v - 10, b - 10, angel));
	m_shape = big - small;
	//设置锚点
	qreal Rangel = ToRadian(m_dbAngle);
	//夹角改变锚点为上方平行边的中间点
	m_ptReangel = (QPointF(-v + b * cos(Rangel), -b * sin(Rangel)) / 2
		+ QPointF(v + b * cos(Rangel), -b * sin(Rangel)) / 2) / 2;
	//标注旋转锚点为左下方端点
	m_ptRotate = QPointF(-v - b * cos(Rangel), b * sin(Rangel)) / 2;
	//大小改变锚点为右下方端点
	m_ptResize = QPointF(v - b * cos(Rangel), b * sin(Rangel)) / 2;

}

void ROIQuadriItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	QGraphicsItem::hoverEnterEvent(event);
	if (!isSelected())
	{
		return;
	}
	switch (WhereIsPos(event->pos()))
	{
	case COMMON:
		setFlag(QGraphicsItem::ItemIsMovable, true);
		setToolTip(QStringLiteral("点击拖动以移动"));
		m_changemode = NONE;
		return;
	case UP_MID:
		setFlag(QGraphicsItem::ItemIsMovable, false);
		m_changemode = m_bReangel ? REANGEL : NONE;
		if (m_changemode != NONE)
		{
			setToolTip(QStringLiteral("点击拖动以改变夹角"));
		}
		return;
	case DOWN_LEFT:
		setFlag(QGraphicsItem::ItemIsMovable, false);
		m_changemode = m_bRotate ? ROTATE : NONE;
		if (m_changemode != NONE)
		{
			setToolTip(QStringLiteral("点击拖动以旋转"));
		}
		return;
	case UP_RIGHT:
		setFlag(QGraphicsItem::ItemIsMovable, false);
		m_changemode = m_bResize ? RESIZE : NONE;
		if (m_changemode != NONE)
		{
			setToolTip(QStringLiteral("点击拖动以改变长短(ctrl)"));
		}
		return;
	}
}

void ROIQuadriItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mousePressEvent(event);
	SetTempShow();
	scene()->addItem(&m_tempShow);
	m_ptClick = event->scenePos();
	//左上角场景坐标
	m_ptULStable = mapToScene(QPointF(-m_dbVerticalLength + m_dbBevelLength * cos(ToRadian(m_dbAngle)),
		-m_dbBevelLength * sin(ToRadian(m_dbAngle))) / 2);
}

void ROIQuadriItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	qreal gap;
	QLineF now, diagonal;
	switch (m_changemode)
	{
	case NONE:
		return;
	case REANGEL:
		gap = QLineF(QPointF(0, 0), mapFromScene(m_ptClick)).angleTo(QLineF(QPointF(0, 0), event->pos()));
		Reset(m_dbVerticalLength, m_dbBevelLength, m_dbAngle + gap);
		m_ptClick = event->scenePos();
		update();
		return;
	case ROTATE:
		now = QLineF(scenePos(), event->scenePos());
		diagonal = QLineF(scenePos(), scenePos() + m_ptRotate);
		setRotation(now.angleTo(diagonal));
		m_ptClick = event->scenePos();
		return;
	case RESIZE:
		if ((event->modifiers()&Qt::ControlModifier) != 0)//按下control作为修饰键
		{
			//对称式缩放
			qreal sita1 = QLineF(QPointF(0, 0), QPointF(1, 0)).angleTo(QLineF(QPointF(0, 0), event->pos()));
			qreal sita2 = m_dbAngle - sita1;
			qreal sita3 = 180 - m_dbAngle;
			qreal length = QLineF(QPointF(0, 0), event->pos()).length() * 2;
			qreal v = abs(sin(ToRadian(sita2))) / abs(sin(ToRadian(sita3)))*length;
			qreal b = abs(sin(ToRadian(sita1))) / abs(sin(ToRadian(sita3)))*length;
			Reset(v, b, m_dbAngle);
		}
		else
		{
			//非对称式缩放
			QPointF center = (m_ptULStable + event->scenePos()) / 2;
			qreal sita1 = QLineF(QPointF(0, 0), QPointF(1, 0)).angleTo(QLineF(QPointF(0, 0), event->pos()));
			qreal sita2 = m_dbAngle - sita1;
			qreal sita3 = 180 - m_dbAngle;
			qreal length = QLineF(center, event->scenePos()).length() * 2;
			qreal v = abs(sin(ToRadian(sita2))) / abs(sin(ToRadian(sita3)))*length;
			qreal b = abs(sin(ToRadian(sita1))) / abs(sin(ToRadian(sita3)))*length;
			Reset(v, b, m_dbAngle);
			setPos(center);
		}
		update();
		return;
	}
}

void ROIQuadriItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	scene()->removeItem(&m_tempShow);
}


void ROIQuadriItem::SetTempShow()
{
	m_tempShow.setPolygon(m_Polygon);
	m_tempShow.setZValue(-1);
	QPen pen(Qt::red);
	pen.setStyle(Qt::DotLine);
	m_tempShow.setPen(pen);
	m_tempShow.setBrush(Qt::transparent);
	m_tempShow.setRotation(rotation());
	m_tempShow.setPos(scenePos());
}

QPolygonF ROIQuadriItem::MakePolygon(qreal v, qreal b, qreal angel) const
{
	QPolygonF ans;
	qreal Rangel = ToRadian(angel);
	ans << QPointF(v - b * cos(Rangel), b * sin(Rangel)) / 2//右下角
		<< QPointF(-v - b * cos(Rangel), b * sin(Rangel)) / 2//左下角
		<< QPointF(-v + b * cos(Rangel), -b * sin(Rangel)) / 2//左上角
		<< QPointF(v + b * cos(Rangel), -b * sin(Rangel)) / 2;//右上角
	return ans;
}



ROIBaseItem::POSITION ROIQuadriItem::WhereIsPos(QPointF pos)
{
	if (Approach(pos, m_ptReangel))
	{
		return UP_MID;
	}
	else if (Approach(pos, m_ptRotate))
	{
		return DOWN_LEFT;
	}
	else if (Approach(pos, m_ptResize))
	{
		return UP_RIGHT;
	}
	return COMMON;
}


void ROIQuadriItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(widget);
	painter->setBrush(Qt::transparent);
	painter->setRenderHint(QPainter::SmoothPixmapTransform);
	if (option->state & QStyle::State_Selected)
	{
		//选中状态画法
		painter->setPen(Qt::green);
		painter->drawPolygon(m_Polygon);
		painter->setPen(Qt::black);
		painter->setBrush(Qt::transparent);
		//锚点绘制准备
		QPainterPath path, verpath(QPointF(2, 2)), horpath(QPointF(-2, 2));
		verpath.lineTo(-2, -2);
		horpath.lineTo(2, -2);
		path.addPath(verpath);
		path.addPath(horpath);
		//夹角改变锚点绘制
		painter->translate(m_ptReangel);
		if (m_bReangel)
		{
			painter->drawPath(path);
		}
		//旋转锚点绘制
		painter->translate(m_ptRotate - m_ptReangel);
		if (m_bRotate)
		{
			painter->drawPath(path);
		}
		//大小改变锚点绘制
		painter->translate(m_ptResize - m_ptRotate);
		if (m_bResize)
		{
			painter->drawPath(path);
		}
	}
	else
	{
		//未选中状态画法
		painter->setPen(Qt::red);
		painter->drawPolygon(m_Polygon);
	}
}

