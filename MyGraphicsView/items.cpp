#include "items.h"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QKeyEvent>
#include <QApplication>
#include <qDebug> 

inline double ToRadian(double x)
{
	return 3.1415 / 180 * (x);
}

//这是基于QGraphicsItem派生的抽象类
//用于设置元素外接矩形、边界矩形、设置笔刷、画刷
BaseItem::BaseItem(QGraphicsItem * parent)
{
}

BaseItem::~BaseItem()
{
}

void BaseItem::SetRect(const QRectF & rect)
{
	//if (rect.width() >= 8 && rect.height() >= 8 && rect.width() <= maxW && rect.height() <= maxH)
	//{
	//	if (m_rect == rect)
	//	{
	//		return;
	//	}
	//	prepareGeometryChange();
	//	m_rect = rect;
	//	m_boundingRect = QRectF();
	//	update();
	//}
	//基于超级四边形的最小角度bug作出以下修改
	//移除了最小外接矩形的检查
	//后续预备将其他元素重构过程中将这一检查（包括最大检查）下放
	if (rect.width() <= maxW && rect.height() <= maxH)
	{
		if (m_rect == rect)
		{
			return;
		}
		prepareGeometryChange();
		m_rect = rect;
		m_boundingRect = QRectF();
		update();
	}
	return;
}

void BaseItem::SetSelectedPen(const QPen &pen)
{
	m_selectedpen = pen;
	update();
}

void BaseItem::SetNotSelectedPen(const QPen & pen)
{
	m_notselpen = pen;
	update();
}

void BaseItem::SetSelectedBrush(const QBrush &brush)
{
	m_selectedbrush = brush;
	update();
}

void BaseItem::SetNotSelectedBrush(const QBrush & brush)
{
	m_notselbrush = brush;
	update();
}

qreal BaseItem::maxW = 1000;
void BaseItem::SetMaxWidth(qreal w)
{
	maxW = w;
}

qreal BaseItem::maxH = 1000;
void BaseItem::SetMaxHeight(qreal h)
{
	maxH = h;
}

//右下画布的延申目的在于矩形的旋转锚点的可用性
//后续优化在矩形旋转锚点的重新选用后将 + 24 -》 + 12
QRectF BaseItem::boundingRect() const
{
	QRectF mb = QRectF(m_rect.x() - 6, m_rect.y() - 6, m_rect.width() + 24, m_rect.height() + 24);
	if (m_boundingRect.isNull())
	{
		m_boundingRect = mb;
	}
	return m_boundingRect;
}

RectItem::RectItem(QGraphicsItem * parent)
{
}

RectItem::RectItem(qreal w, qreal h, QGraphicsItem * parent)
{
	SetRect(-w / 2, -h / 2, w, h);
	setFlag(ItemIsSelectable);
	setAcceptHoverEvents(true);
}

RectItem::~RectItem()
{
}

QList<QPointF> RectItem::rect()
{
	QList<QPointF> rect;
	rect.append(mapToScene(QPointF(m_rect.x(), m_rect.y())));//左上角
	rect.append(mapToScene(QPointF(m_rect.x() + m_rect.width(), m_rect.y())));//右上角
	rect.append(mapToScene(QPointF(m_rect.x() + m_rect.width(), m_rect.y() + m_rect.height())));//右下角
	rect.append(mapToScene(QPointF(m_rect.x(), m_rect.y() + m_rect.height())));//左下角
	return rect;
}

QPainterPath RectItem::shape() const
{
	QPainterPath path;
	path.addRect(QRectF(m_rect.x() - 6, m_rect.y() - 6, m_rect.width() + 24, m_rect.height() + 24));
	return path;
}

RectItem::SHAPEAREA RectItem::whereMyPos(QPointF pos)
{
	qreal x = pos.x();
	qreal y = pos.y();
	qreal halfW = m_rect.width() / 2;
	qreal halfH = m_rect.height() / 2;
	if (pos.manhattanLength() < 20)
	{
		return SIZEALL_AREA;
	}
	else if (abs(x) > halfW - 6 && abs(x) < halfW + 6 && abs(y) > halfH - 6 && abs(y) < halfH + 6)
	{
		if (x * y > 0)
		{
			return FDIAG_AREA;
		}
		else if (x * y < 0)
		{
			return BDIAG_AREA;
		}
	}
	else if (abs(x) < halfW - 6 && abs(y) > halfH - 6 && abs(y) < halfH + 6)
	{
		return VER_AREA;
	}
	else if (abs(x) > halfW - 6 && abs(x) < halfW + 6 && abs(y) < halfH - 6)
	{
		return HOR_AREA;
	}
	else if (x > halfW && y > halfH)
	{
		return CROSS_AREA;
	}
	else if ((abs(x) < halfW - 6 && abs(y) < halfH - 6) || abs(x) > halfW + 6 || abs(y) > halfH + 6)
	{
		return ARROR_AREA;
	}
	return ARROR_AREA;
}

RectItem::TYPE RectItem::Type()
{
	return RECT;
}

void RectItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(widget);
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(m_notselpen);
	painter->setBrush(m_notselbrush);
	painter->drawRect(m_rect);
	if (option->state & QStyle::State_Selected)
	{
		painter->setPen(m_selectedpen);
		painter->setBrush(m_selectedbrush);
		painter->drawRect(m_rect);
		if (abletomove)
		{
			QPainterPath path, verpath(QPointF(-6, 0)), horpath(QPointF(0, -6));
			verpath.lineTo(6, 0);
			horpath.lineTo(0, 6);
			path.addPath(verpath);
			path.addPath(horpath);
			painter->drawPath(path);
		}
		if (abletoresize)
		{
			painter->setPen(Qt::black);
			painter->setBrush(Qt::white);
			painter->translate(m_rect.x(), m_rect.y());
			painter->drawEllipse(-3, -3, 6, 6);
			painter->translate(m_rect.width() / 2, 0);
			painter->drawEllipse(-3, -3, 6, 6);
			painter->translate(m_rect.width() / 2, 0);
			painter->drawEllipse(-3, -3, 6, 6);
			painter->translate(0, m_rect.height() / 2);
			painter->drawEllipse(-3, -3, 6, 6);
			painter->translate(0, m_rect.height() / 2);
			painter->drawEllipse(-3, -3, 6, 6);
			painter->translate(-m_rect.width() / 2, 0);
			painter->drawEllipse(-3, -3, 6, 6);
			painter->translate(-m_rect.width() / 2, 0);
			painter->drawEllipse(-3, -3, 6, 6);
			painter->translate(0, -m_rect.height() / 2);
			painter->drawEllipse(-3, -3, 6, 6);
		}
		if (abletorotate)
		{
			painter->setPen(Qt::transparent);
			painter->setBrush(Qt::white);
			painter->translate(12 - m_rect.x() * 2, 12 - m_rect.y());
			painter->drawEllipse(-4, -4, 8, 8);
		}
	}
}

void RectItem::SetAbility(ABILITY ability, bool tag)
{
	switch (ability)
	{
	case ABLETOMOVE:
		abletomove = tag;
		break;
	case ABLETOROTATE:
		abletorotate = tag;
		break;
	case ABLETORESIZE:
		abletoresize = tag;
		break;
	}
}

//鼠标滑动改变样式，设置拉伸模式
void RectItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	if (abs(event->pos().x()) < m_rect.width() / 2 && abs(event->pos().y()) < m_rect.height() / 2)
	{
		setFlag(ItemIsSelectable, true);
	}
	else if (!isSelected())
	{
		setFlag(ItemIsSelectable, false);
	}
	else if (event->pos().x() > m_rect.width() / 2 + 10 && event->pos().y() > m_rect.height() / 2 + 10 && abletorotate)
	{
		setCursor(Qt::CrossCursor);
	}
	if (!isSelected())
	{
		setCursor(Qt::ArrowCursor);
		change = false;
		return;
	}
	switch (whereMyPos(event->pos()))
	{
	case SIZEALL_AREA:
		if (abletomove)
		{
			setCursor(Qt::SizeAllCursor);//设置鼠标为十字移动
			setFlag(QGraphicsItem::ItemIsMovable, true);
			changeMode = NONE;
			change = false;
			return;
		}
		else
		{
			changeMode = NONE;
			change = false;
			return;
		}
	case HOR_AREA:
		if (abletoresize)
		{
			setCursor(Qt::SizeHorCursor); //设置鼠标为垂直拉伸
			setFlag(QGraphicsItem::ItemIsMovable, false);
			changeMode = (event->pos().x() > 0 ? RIGHT : LEFT);
			change = true;
			return;
		}
		else
		{
			setCursor(Qt::ArrowCursor); //设置鼠标为默认箭头
			changeMode = NONE;
			change = false;
			return;
		}
	case VER_AREA:
		if (abletoresize)
		{
			setCursor(Qt::SizeVerCursor); //设置鼠标为水平拉伸
			setFlag(QGraphicsItem::ItemIsMovable, false);
			changeMode = (event->pos().y() > 0 ? DOWN : UP);
			change = true;
			return;
		}
		else
		{
			setCursor(Qt::ArrowCursor); //设置鼠标为默认箭头
			changeMode = NONE;
			change = false;
			return;
		}
	case FDIAG_AREA:
		if (abletoresize)
		{
			setCursor(Qt::SizeFDiagCursor); //设置鼠标为左上，右下拉伸
			setFlag(QGraphicsItem::ItemIsMovable, false);
			changeMode = (event->pos().y() > 0 ? DOWN_RIGHT : UP_LEFT);
			change = true;
			return;
		}
		else
		{
			setCursor(Qt::ArrowCursor); //设置鼠标为默认箭头
			changeMode = NONE;
			change = false;
			return;
		}
	case BDIAG_AREA:
		if (abletoresize)
		{
			setCursor(Qt::ArrowCursor); //设置鼠标为默认箭头
			setCursor(Qt::SizeBDiagCursor); //设置鼠标为左下，右上拉伸
			setFlag(QGraphicsItem::ItemIsMovable, false);
			changeMode = (event->pos().y() > 0 ? DOWN_LEFT : UP_RIGHT);
			change = true;
			return;
		}
		else
		{
			setCursor(Qt::ArrowCursor); //设置鼠标为默认箭头
			changeMode = NONE;
			change = false;
			return;
		}
	case ARROR_AREA:
		setCursor(Qt::ArrowCursor); //设置鼠标为默认箭头
		setFlag(QGraphicsItem::ItemIsMovable, false);
		changeMode = NONE;
		change = false;
		return;
	case CROSS_AREA:
		if (abletorotate)
		{
			setCursor(Qt::CrossCursor); //设置鼠标为十字箭头
			setFlag(QGraphicsItem::ItemIsMovable, false);
			changeMode = ROTATE;
			change = true;
		}
		else
		{
			setCursor(Qt::ArrowCursor); //设置鼠标为默认箭头
			changeMode = NONE;
			change = false;
			return;
		}
	}
}

//保存点击时四角的场景位置，提高拉伸性能
void RectItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mousePressEvent(event);
	if (!isSelected())
	{
		return;
	}
	if (change)//非对称变换时保存
	{
		stableUL = mapToScene(QPointF(m_rect.x(), m_rect.y()));
		stableBR = stableUL + QPointF(m_rect.width(), m_rect.height());
		stableUR = stableUL + QPointF(m_rect.width(), 0);
		stableBL = stableUL + QPointF(0, m_rect.height());
	}
}

//拉伸变换
//=
//重设矩形坐标->非对称变换需要此步骤,由于模式切换中存在跳变,实际不保留此功能
//+
//改变矩形形状
//qreal w = (center - stable).x();
//qreal h = (center - stable).y();
//SetRect(QRectF(QPointF(-w,-h),QPointF(w,h)));
void RectItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	if (isSelected() && change)
	{
		qreal w = m_rect.width() / 2;
		qreal h = m_rect.height() / 2;
		QPointF center = mapToScene(m_rect.center());
		switch (changeMode)
		{
		case NONE:
			return;
		case RIGHT:
			if ((event->modifiers()&Qt::ControlModifier) != 0)//执行对称缩放
			{
				w = event->pos().x();
				h = m_rect.height() / 2;
				break;
			}
			else//不执行非对称缩放、后续增加修改条件即可实现
			{
				center = (stableUL + mapToScene(QPointF(event->pos().x(), m_rect.y() + m_rect.height()))) / 2;
				w = (center - stableUL).x();
				h = (center - stableUL).y();
				break;
			}
		case LEFT:
			if ((event->modifiers()&Qt::ControlModifier) != 0)
			{
				w = -event->pos().x();
				h = m_rect.height() / 2;
				break;
			}
			else
			{
				center = (stableBR + mapToScene(QPointF(event->pos().x(), m_rect.y()))) / 2;
				w = (stableBR - center).x();
				h = (stableBR - center).y();
				break;
			}
		case UP:
			if ((event->modifiers()&Qt::ControlModifier) != 0)
			{
				w = m_rect.width() / 2;
				h = -event->pos().y();
				break;
			}
			else
			{
				center = (stableBR + mapToScene(QPointF(m_rect.x(), event->pos().y()))) / 2;
				w = (stableBR - center).x();
				h = (stableBR - center).y();
				break;
			}
		case DOWN:
			if ((event->modifiers()&Qt::ControlModifier) != 0)
			{
				w = m_rect.width() / 2;
				h = event->pos().y();
				break;
			}
			else
			{
				center = (stableUL + mapToScene(QPointF(m_rect.x() + m_rect.width(), event->pos().y()))) / 2;
				w = (center - stableUL).x();
				h = (center - stableUL).y();
				break;
			}
		case DOWN_RIGHT:
			if ((event->modifiers()&Qt::ControlModifier) != 0)
			{
				QPointF gap = event->pos() - QPointF(m_rect.x() + m_rect.width(), m_rect.y() + m_rect.height());
				w = m_rect.width() / 2 + gap.x();
				h = m_rect.height() / 2 + gap.y();
				break;
			}
			else
			{
				center = (stableUL + mapToScene(event->pos())) / 2;
				w = (center - stableUL).x();
				h = (center - stableUL).y();
				break;
			}
		case DOWN_LEFT:
			if ((event->modifiers()&Qt::ControlModifier) != 0)
			{
				QPointF gap = event->pos() - QPointF(m_rect.x(), m_rect.y() + m_rect.height());
				w = m_rect.width() / 2 - gap.x();
				h = m_rect.height() / 2 + gap.y();
				break;
			}
			else
			{
				center = (stableUR + mapToScene(event->pos())) / 2;
				w = (stableUR - center).x();
				h = -(stableUR - center).y();
				break;
			}
		case UP_LEFT:
			if ((event->modifiers()&Qt::ControlModifier) != 0)
			{
				QPointF gap = event->pos() - QPointF(m_rect.x(), m_rect.y());
				w = m_rect.width() / 2 - gap.x();
				h = m_rect.height() / 2 - gap.y();
				break;
			}
			else
			{
				center = (stableBR + mapToScene(event->pos())) / 2;
				w = (stableBR - center).x();
				h = (stableBR - center).y();
				break;
			}
		case UP_RIGHT:
			if ((event->modifiers()&Qt::ControlModifier) != 0)
			{
				QPointF gap = event->pos() - QPointF(m_rect.x() + m_rect.width(), m_rect.y());
				w = m_rect.width() / 2 + gap.x();
				h = m_rect.height() / 2 - gap.y();
				break;
			}
			else
			{
				center = (stableBL + mapToScene(event->pos())) / 2;
				w = -(stableBL - center).x();
				h = (stableBL - center).y();
				break;
			}
		case ROTATE:
			QLineF now = QLineF(mapToScene(QPointF(0, 0)), mapToScene(event->pos()));
			QLineF diagonal = QLineF(mapToScene(QPointF(0, 0)), mapToScene(QPointF(0, 0)) + QPointF(m_rect.width(), m_rect.height()));
			setRotation(now.angleTo(diagonal));
			return;
		}
		setPos(center);
		SetRect(QRectF(QPointF(-w, -h), QPointF(w, h)));
	}
}

void RectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	changeMode = NONE;
}

CircleItem::CircleItem(QGraphicsItem *parent)
{

}

CircleItem::CircleItem(qreal r, QGraphicsItem *parent)
{
	setRadius(r);
	setFlag(ItemIsSelectable);
	setAcceptHoverEvents(true);
}

CircleItem::~CircleItem()
{
}

QList<qreal> CircleItem::circle()
{
	QList<qreal> circle;
	QPointF center = mapToScene(m_rect.center());
	circle.append(center.x());
	circle.append(center.y());
	circle.append(m_radius);
	return circle;
}

QPainterPath CircleItem::shape() const
{
	QPainterPath path;
	path.addEllipse(-m_radius, -m_radius, m_radius * 2, m_radius * 2);
	return path;
}

void CircleItem::setRadius(qreal r)
{
	if (r <= qMin(maxW, maxH) / 2)
	{
		m_radius = r;
		QPointF rVer = QPointF(r, r);
		SetRect(QRectF(m_rect.center() - rVer, m_rect.center() + rVer));
		return;
	}
	return;
}

void CircleItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(widget);
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(m_notselpen);
	painter->setBrush(m_notselbrush);
	painter->drawEllipse(-m_radius, -m_radius, m_radius * 2, m_radius * 2);
	if (option->state & QStyle::State_Selected)
	{
		painter->setPen(m_selectedpen);
		painter->setBrush(m_selectedbrush);
		painter->drawEllipse(-m_radius, -m_radius, m_radius * 2, m_radius * 2);
		QPainterPath path, verpath(QPointF(-6, 0)), horpath(QPointF(0, -6));
		verpath.lineTo(6, 0);
		horpath.lineTo(0, 6);
		path.addPath(verpath);
		path.addPath(horpath);
		painter->drawPath(path);
		painter->setPen(Qt::black);
		painter->setBrush(Qt::white);
		painter->translate(m_radius, 0);
		painter->drawEllipse(-5, -5, 10, 10);
	}
}

void CircleItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	if (!isSelected())
	{
		setCursor(Qt::ArrowCursor);
		return;
	}
	switch (whereMyPos(event->pos()))
	{
	case SIZEALL_AREA:
		setCursor(Qt::SizeAllCursor);//设置鼠标为十字移动
		setFlag(QGraphicsItem::ItemIsMovable, true);
		change = false;
		return;
	case VER_AREA:
		setCursor(Qt::SizeVerCursor); //设置鼠标为水平拉伸
		setFlag(QGraphicsItem::ItemIsMovable, false);
		change = true;
		return;
	case HOR_AREA:
		setCursor(Qt::SizeHorCursor); //设置鼠标为垂直拉伸
		setFlag(QGraphicsItem::ItemIsMovable, false);
		change = true;
		return;
	case FDIAG_AREA:
		setCursor(Qt::SizeFDiagCursor); //设置鼠标为左上，右下拉伸
		setFlag(QGraphicsItem::ItemIsMovable, false);
		change = true;
		return;
	case BDIAG_AREA:
		setCursor(Qt::SizeBDiagCursor); //设置鼠标为左下，右上拉伸
		setFlag(QGraphicsItem::ItemIsMovable, false);
		change = true;
		return;
	case ARROR_AREA:
		setCursor(Qt::ArrowCursor); //设置鼠标为默认箭头
		setFlag(QGraphicsItem::ItemIsMovable, false);
		change = false;
		return;
	}
}

void CircleItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	if (change)
	{
		QLineF Radius = QLineF(m_rect.center(), event->pos());
		if (Radius.length() < m_radius + 8 && Radius.length() > m_radius - 8)
		{
			setRadius(Radius.length());
		}
	}
}

CircleItem::SHAPEAREA CircleItem::whereMyPos(QPointF pos)
{
	QLineF hor = QLineF(m_rect.center(), QPointF(1, 0));
	QLineF Radius = QLineF(m_rect.center(), pos);
	qreal angel = hor.angleTo(Radius);
	if (pos.manhattanLength() < 20)
	{
		return SIZEALL_AREA;
	}
	else if (Radius.length() < m_radius + 8 && Radius.length() > m_radius - 8)
	{
		if (0 <= angel && angel < 10 || 170 <= angel && angel < 190 || 350 <= angel && angel < 360)
		{
			return HOR_AREA;
		}
		else if (10 <= angel && angel < 80 || 190 <= angel && angel < 260)
		{
			return BDIAG_AREA;
		}
		else if (100 <= angel && angel < 170 || 280 <= angel && angel < 350)
		{
			return FDIAG_AREA;
		}
		else if (80 <= angel && angel < 100 || 260 <= angel && angel < 280)
		{
			return VER_AREA;
		}
	}
	return ARROR_AREA;
}

CircleItem::TYPE CircleItem::Type()
{
	return CIRCLE;
}

DonutItem::DonutItem(QGraphicsItem * parent)
{
}

DonutItem::DonutItem(qreal bigr, qreal smallr, QGraphicsItem * parent)
{
	SetBigRadius(bigr);
	SetSmallRadius(smallr);
	setFlag(ItemIsSelectable);
	setAcceptHoverEvents(true);
}

DonutItem::~DonutItem()
{
}

QList<qreal> DonutItem::donut()
{
	QList<qreal> donut;
	QPointF center = mapToScene(m_rect.center());
	donut.append(center.x());
	donut.append(center.y());
	donut.append(m_BigRaidus);
	donut.append(m_SmallRadius);
	return donut;
}

QPainterPath DonutItem::shape() const
{
	QPainterPath path;
	path.addEllipse(-m_BigRaidus, -m_BigRaidus, m_BigRaidus * 2, m_BigRaidus * 2);
	return path;
}

void DonutItem::SetSmallRadius(qreal r)
{
	if (r < m_BigRaidus)
	{
		m_SmallRadius = r;
		update();
	}
}

void DonutItem::SetBigRadius(qreal r)
{
	if (r <= qMin(maxH, maxW) / 2 && r > m_SmallRadius)
	{
		m_BigRaidus = r;
		QPointF rVer = QPointF(r, r);
		SetRect(QRectF(m_rect.center() - rVer, m_rect.center() + rVer));
	}
}

void DonutItem::SetSelectedSmallPen(const QPen & pen)
{
	m_SelectedSmallPen = pen;
}

void DonutItem::SetSelectedBigPen(const QPen & pen)
{
	m_SelectedBigPen = pen;
}

void DonutItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	if (!isSelected())
	{
		setCursor(Qt::ArrowCursor);
		return;
	}
	switch (whereMyPos(event->pos()))
	{
	case SIZEALL_AREA:
		setCursor(Qt::SizeAllCursor);//设置鼠标为十字移动
		setFlag(QGraphicsItem::ItemIsMovable, true);
		change = false;
		return;
	case VER_AREA:
		setCursor(Qt::SizeVerCursor); //设置鼠标为水平拉伸
		setFlag(QGraphicsItem::ItemIsMovable, false);
		change = true;
		return;
	case HOR_AREA:
		setCursor(Qt::SizeHorCursor); //设置鼠标为垂直拉伸
		setFlag(QGraphicsItem::ItemIsMovable, false);
		change = true;
		return;
	case FDIAG_AREA:
		setCursor(Qt::SizeFDiagCursor); //设置鼠标为左上，右下拉伸
		setFlag(QGraphicsItem::ItemIsMovable, false);
		change = true;
		return;
	case BDIAG_AREA:
		setCursor(Qt::SizeBDiagCursor); //设置鼠标为左下，右上拉伸
		setFlag(QGraphicsItem::ItemIsMovable, false);
		change = true;
		return;
	case ARROR_AREA:
		setCursor(Qt::ArrowCursor); //设置鼠标为默认箭头
		setFlag(QGraphicsItem::ItemIsMovable, false);
		change = false;
		return;
	}
}

void DonutItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	if (change)
	{
		QLineF Radius = QLineF(m_rect.center(), event->pos());
		if (Radius.length() < m_BigRaidus + 8 && Radius.length() > m_BigRaidus - 8)
		{
			SetBigRadius(Radius.length());
		}
		else if (Radius.length() < m_SmallRadius + 8 && Radius.length() > m_SmallRadius - 8)
		{
			SetSmallRadius(Radius.length());
		}
	}
}

DonutItem::SHAPEAREA DonutItem::whereMyPos(QPointF pos)
{
	QLineF hor = QLineF(m_rect.center(), QPointF(1, 0));
	QLineF Radius = QLineF(m_rect.center(), pos);
	qreal angel = hor.angleTo(Radius);
	if (pos.manhattanLength() < 20)
	{
		return SIZEALL_AREA;
	}
	else if ((Radius.length() < m_BigRaidus + 8 && Radius.length() > m_BigRaidus - 8) || (Radius.length() < m_SmallRadius + 8 && Radius.length() > m_SmallRadius - 8))
	{
		if (0 <= angel && angel < 10 || 170 <= angel && angel < 190 || 350 <= angel && angel < 360)
		{
			return HOR_AREA;
		}
		else if (10 <= angel && angel < 80 || 190 <= angel && angel < 260)
		{
			return BDIAG_AREA;
		}
		else if (100 <= angel && angel < 170 || 280 <= angel && angel < 350)
		{
			return FDIAG_AREA;
		}
		else if (80 <= angel && angel < 100 || 260 <= angel && angel < 280)
		{
			return VER_AREA;
		}
	}
	return ARROR_AREA;
}

DonutItem::TYPE DonutItem::Type()
{
	return DONUT;
}

void DonutItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(widget);
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(Qt::transparent);
	painter->setBrush(m_notselbrush);
	QPainterPath bigCircle;
	bigCircle.addEllipse(-m_BigRaidus, -m_BigRaidus, m_BigRaidus * 2, m_BigRaidus * 2);
	QPainterPath smallCircle;
	smallCircle.addEllipse(-m_SmallRadius, -m_SmallRadius, m_SmallRadius * 2, m_SmallRadius * 2);
	painter->drawPath(bigCircle - smallCircle);
	painter->setPen(m_notselpen);
	painter->setBrush(Qt::transparent);
	painter->drawEllipse(-m_BigRaidus, -m_BigRaidus, m_BigRaidus * 2, m_BigRaidus * 2);
	painter->drawEllipse(-m_SmallRadius, -m_SmallRadius, m_SmallRadius * 2, m_SmallRadius * 2);
	if (option->state & QStyle::State_Selected)
	{
		painter->setPen(Qt::transparent);
		painter->setBrush(m_selectedbrush);
		QPainterPath bigCircle;
		bigCircle.addEllipse(-m_BigRaidus, -m_BigRaidus, m_BigRaidus * 2, m_BigRaidus * 2);
		QPainterPath smallCircle;
		smallCircle.addEllipse(-m_SmallRadius, -m_SmallRadius, m_SmallRadius * 2, m_SmallRadius * 2);
		painter->drawPath(bigCircle - smallCircle);
		painter->setBrush(Qt::transparent);
		painter->setPen(m_SelectedBigPen);
		painter->drawEllipse(-m_BigRaidus, -m_BigRaidus, m_BigRaidus * 2, m_BigRaidus * 2);
		painter->setPen(m_SelectedSmallPen);
		painter->drawEllipse(-m_SmallRadius, -m_SmallRadius, m_SmallRadius * 2, m_SmallRadius * 2);
		QPainterPath path, verpath(QPointF(-6, 0)), horpath(QPointF(0, -6)), linkpath(QPointF(m_SmallRadius, 0));
		verpath.lineTo(6, 0);
		horpath.lineTo(0, 6);
		linkpath.lineTo(m_BigRaidus, 0);
		path.addPath(verpath);
		path.addPath(horpath);
		painter->drawPath(path);
		QPen pen = m_selectedpen;
		pen.setStyle(Qt::DotLine);
		painter->setPen(pen);
		painter->drawPath(linkpath);
		painter->setPen(Qt::black);
		painter->setBrush(Qt::white);
		painter->translate(m_SmallRadius, 0);
		painter->drawEllipse(-5, -5, 10, 10);
		painter->translate(m_BigRaidus - m_SmallRadius, 0);
		painter->drawEllipse(-5, -5, 10, 10);
	}
}

PolygonItem::PolygonItem(QGraphicsItem * parent)
{
}

PolygonItem::PolygonItem(qreal l, qreal s, qreal angel, QGraphicsItem * parent)
{
	setFlag(ItemIsSelectable);
	setAcceptHoverEvents(true);
	m_dbVerLength = l;
	m_dbHorLength = s;
	m_dbAngel = angel;
	SetPoly();
}

PolygonItem::~PolygonItem()
{
}

void PolygonItem::SetPoly()
{
	m_qpfPoly.clear();
	QPointF p1 = QPointF(0, 0);
	QPointF p2 = p1 + QPointF(m_dbHorLength, 0);
	QLineF line = QLineF(p1, p1 + QPointF(m_dbHorLength, 0));
	line.setAngle(m_dbAngel);
	QPointF p3 = line.p2();
	m_qpfPoly << p1 << p2 << p2 + p3 - p1 << p3;
	m_ptCenter = QLineF(p2, p3).center();
	m_ptLeft = QLineF(p1, p3).center();
	m_ptUR = p3;
	SetRect(m_qpfPoly.boundingRect());
}

void PolygonItem::SetAngel(qreal angel)
{
	m_dbAngel = angel;
	SetPoly();
	update();
}

PolygonItem::TYPE PolygonItem::Type()
{
	return POLYGON;
}

QPainterPath PolygonItem::shape() const
{
	QPainterPath a;
	a.addPolygon(m_qpfPoly);
	return a;
}

void PolygonItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(widget);
	painter->drawRect(m_boundingRect);

	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(m_notselpen);
	painter->setBrush(m_notselbrush);
	painter->drawPolygon(m_qpfPoly);
	if (option->state & QStyle::State_Selected)
	{
		painter->setPen(m_selectedpen);
		painter->setBrush(m_selectedbrush);
		painter->drawPolygon(m_qpfPoly);
		painter->translate(m_ptCenter);
		QPainterPath path, verpath(QPointF(-3, 0)), horpath(QPointF(0, -3));
		verpath.lineTo(3, 0);
		horpath.lineTo(0, 3);
		path.addPath(verpath);
		path.addPath(horpath);
		painter->drawPath(path);
	}
}

void PolygonItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	QGraphicsItem::hoverMoveEvent(event);
	switch (whereMyPos(event->pos()))
	{
	case SIZEALL_AREA://中心平移区域
		setCursor(Qt::SizeAllCursor);//设置鼠标为十字移动
		setFlag(QGraphicsItem::ItemIsMovable, true);
		change = false;
		changeMode = NONE;
		return;
	case VER_AREA://左边改变角度区域
		setCursor(Qt::SizeVerCursor); //设置鼠标为水平箭头
		setFlag(QGraphicsItem::ItemIsMovable, false);
		change = true;
		changeMode = REANGEL;
		return;
	case HOR_AREA:
		setCursor(Qt::SizeHorCursor); //设置鼠标为水平箭头
		setFlag(QGraphicsItem::ItemIsMovable, false);
		change = true;
		changeMode = RESIZE;
	case ARROR_AREA://其他区域
		setCursor(Qt::ArrowCursor); //设置鼠标为默认箭头
		setFlag(QGraphicsItem::ItemIsMovable, false);
		change = false;
		changeMode = NONE;
		return;
	}
}

void PolygonItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mousePressEvent(event);
	if (!isSelected())
	{
		return;
	}
	if (change)
	{
		m_ptStart = event->pos();
	}
}

void PolygonItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	if (isSelected() && change)
	{
		qreal angelgap;
		switch (changeMode)
		{
		case REANGEL:
			angelgap = QLineF(QPointF(0, 0), m_ptStart).angleTo(QLineF(QPointF(0, 0), event->pos()));
			qDebug() << angelgap;
			SetAngel(m_dbAngel + angelgap);
			m_ptStart = event->pos();
			return;
		case RESIZE:
			//qreal sita1 = QLineF(QPointF(0, 0), QPointF(1, 0)).angleTo(QLineF(QPointF(0, 0), event->pos()));
			//qreal sita2 = m_dbAngel - sita1;
			//qreal sita3 = 180 - m_dbAngel;
			//qreal v, b;
			//qreal length = QLineF(QPointF(0, 0), event->pos()).length();
			//b = length * sin(ToRadian(sita1)) / sin(sita3);
			return;
		case NONE:
			return;
		}
	}
	QGraphicsItem::mouseMoveEvent(event);
}

void PolygonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseReleaseEvent(event);
}

PolygonItem::SHAPEAREA PolygonItem::whereMyPos(QPointF pos)
{
	if (QLineF(pos, m_ptCenter).length() < 5)//位于中心返回平移区域
	{
		return SIZEALL_AREA;
	}
	else if (QLineF(QPointF(0, 0), pos).length()*
		sin(ToRadian(QLineF(QPointF(0, 0), pos).angleTo(QLineF(QPointF(0, 0), QPointF(m_ptLeft)))) < 10))
		//位于左边返回变角区域
	{
		return VER_AREA;
	}
	else if (QLineF(m_ptUR,pos).length() < 10)
	{
		return HOR_AREA;
	}
	return ARROR_AREA;
}

QuadriItem::QuadriItem(QGraphicsItem * parent)
{
}

QuadriItem::QuadriItem(qreal v, qreal b, qreal angel, QGraphicsItem * parent)
{
	Reset(v, b, angel);
	m_tPolygon = m_Polygon;
	setFlag(ItemIsSelectable);
	setAcceptHoverEvents(true);
}

QuadriItem::~QuadriItem()
{
}

void QuadriItem::Reset(qreal v, qreal b, qreal angel)
{
	//设置特征成员变量
	m_dbVerticalLength = v;
	m_dbBevelLength = b;
	m_dbAngel = angel;
	//计算缓存成员变量
	qreal Rangel = ToRadian(m_dbAngel);
	m_ptDLVertex = QPointF(-v / 2 + b * cos(Rangel) / 2, b * sin(Rangel) / 2);//左下角
	m_ptDRVertex = QPointF(v / 2 + b * cos(Rangel) / 2, b * sin(Rangel) / 2);//右下角
	m_ptULVertex = QPointF(-m_ptDRVertex.x(), -m_ptDRVertex.y());
	m_ptURVertex = QPointF(-m_ptDLVertex.x(), -m_ptDLVertex.y());
	m_Polygon.clear();
	m_Polygon = Polygon(m_ptDLVertex, m_ptDRVertex);
	QPainterPath big;
	QPainterPath small;
	qreal bigv = v + 7;
	qreal bigb = b + 7;
	qreal smallv = v - 7;
	qreal smallb = b - 7;
	big.addPolygon(Polygon(QPointF(-bigv / 2 + bigb * cos(Rangel) / 2, bigb * sin(Rangel) / 2),
		QPointF(bigv / 2 + bigb * cos(Rangel) / 2, bigb * sin(Rangel) / 2)));
	small.addPolygon(Polygon(QPointF(-smallv / 2 + smallb * cos(Rangel) / 2, smallb * sin(Rangel) / 2),
		QPointF(smallv / 2 + smallb * cos(Rangel) / 2, smallb * sin(Rangel) / 2)));
	m_Shape = big - small;
	//设置行为支撑变量
	m_ptRotatePone = m_ptULVertex;//左下角端点作为旋转锚点
	m_ptResizePone = m_ptDRVertex;//右上角端点作为大小改变锚点
	m_ptULStable = mapToScene(m_ptULVertex);
	m_ptDRStable = mapToScene(m_ptDRVertex);
	m_ptReangelPone = (m_ptULVertex + m_ptURVertex) / 2;//上方平行边中点作为角度改变锚点
	//设置父类边界矩形空间
	SetRect(m_Polygon.boundingRect());
}

QList<QPointF> QuadriItem::vertex()
{
	QList<QPointF> ans;
	ans.append(mapToScene(m_ptDLVertex));
	ans.append(mapToScene(m_ptDRVertex));
	ans.append(mapToScene(m_ptURVertex));
	ans.append(mapToScene(m_ptULVertex));
	return ans;
}

QuadriItem::TYPE QuadriItem::Type()
{
	return QUADRI;
}

QPainterPath QuadriItem::shape() const
{
	return m_Shape;
}

void QuadriItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(widget);
	//将边界绘制出来
	//painter->drawRect(m_boundingRect);


	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(m_notselpen);
	painter->setBrush(Qt::transparent);
	painter->drawPolygon(m_Polygon);
	if (option->state & QStyle::State_Selected)
	{
		//将交互形状绘制出来
		//painter->setPen(Qt::transparent);
		//painter->setBrush(m_selectedbrush);
		//painter->drawPath(m_Shape);

		if (m_bChanging)
		{
			painter->setPen(Qt::red);
			painter->setBrush(Qt::transparent);
			painter->drawPolygon(m_tPolygon);
			painter->setPen(Qt::green);
			painter->setBrush(Qt::transparent);
			painter->drawPolygon(m_Polygon);
		}
		else
		{
			painter->setPen(Qt::green);
			painter->setBrush(Qt::transparent);
			painter->drawPolygon(m_Polygon);
			//绘制锚点标记
			painter->setPen(Qt::black);
			painter->setBrush(Qt::transparent);
			painter->translate(m_ptResizePone);
			QPainterPath path, verpath(QPointF(1, 1)), horpath(QPointF(-1, 1));
			verpath.lineTo(-1, -1);
			horpath.lineTo(1, -1);
			path.addPath(verpath);
			path.addPath(horpath);
			painter->drawPath(path);
			painter->translate(m_ptRotatePone - m_ptResizePone);
			painter->drawEllipse(-1, -1, 2, 2);
			painter->translate(m_ptReangelPone - m_ptRotatePone);
			painter->drawRect(-1, -1, 2, 2);
		}
	}
}

void QuadriItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	QGraphicsItem::hoverMoveEvent(event);
	if (!isSelected())
	{
		setCursor(Qt::ArrowCursor);
		return;
	}
	switch (whereMyPos(event->pos()))
	{
	case SIZEALL_AREA:
		setFlag(QGraphicsItem::ItemIsMovable, true);
		changeMode = NONE;
		setCursor(Qt::ArrowCursor);
		return;
	case HOR_AREA:
		setFlag(QGraphicsItem::ItemIsMovable, false);
		changeMode = RESIZE;
		setCursor(Qt::ArrowCursor);
		return;
	case VER_AREA:
		setFlag(QGraphicsItem::ItemIsMovable, false);
		changeMode = REANGEL;
		setCursor(m_curReangel);
		return;
	case CROSS_AREA:
		setFlag(QGraphicsItem::ItemIsMovable, false);
		changeMode = ROTATE;
		setCursor(m_curRotation);
		return;
	}
}

void QuadriItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mousePressEvent(event);
	m_ptClick = event->pos();
}

void QuadriItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	if (isSelected())
	{
		QPointF center;
		QLineF now, diagonal;
		qreal gap, v, b, length;
		switch (changeMode)
		{
		case NONE:
			break;
		case REANGEL://夹角改变实现，方法为计算差值角度
			gap = QLineF(QPointF(0, 0), m_ptClick).angleTo(QLineF(QPointF(0, 0), event->pos()));
			Reset(m_dbVerticalLength, m_dbBevelLength, m_dbAngel - gap);
			m_ptClick = event->pos();
			m_bChanging = true;
			break;
		case ROTATE://旋转改变实现，方法为计算偏差角度
			now = QLineF(mapToScene(QPointF(0, 0)), mapToScene(event->pos()));
			diagonal = QLineF(mapToScene(QPointF(0, 0)), mapToScene(QPointF(0, 0)) + m_ptRotatePone);
			setRotation(now.angleTo(diagonal));
			m_ptClick = event->pos();
			m_bChanging = true;
			break;
		case RESIZE://大小改变依据与正弦定理
			qreal sita1 = QLineF(QPointF(0, 0), event->pos()).angleTo(QLineF(QPointF(0, 0), QPointF(1,0)));
			qreal sita2 = m_dbAngel - sita1;
			qreal sita3 = 180 - m_dbAngel;
			qDebug() << sita1 << sita2 << sita3;
			length = QLineF(QPointF(0,0), event->pos()).length()*2;
			v = abs(sin(ToRadian(sita2))) / abs(sin(ToRadian(sita3)))*length;
			b = abs(sin(ToRadian(sita1))) / abs(sin(ToRadian(sita3)))*length;
			center = (mapToScene(m_ptULVertex) + event->scenePos()) / 2;
			//setPos(center);
			Reset(v, b, m_dbAngel);
			m_bChanging = true;
			break;
		}
	}

}

void QuadriItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	m_tPolygon = m_Polygon;
	m_bChanging = false;
	update();
}

QuadriItem::SHAPEAREA QuadriItem::whereMyPos(QPointF pos)
{
	if (QLineF(m_ptURVertex, pos).length()*
		sin(ToRadian(QLineF(m_ptURVertex, pos).angleTo(QLineF(m_ptURVertex, m_ptDLVertex))) < 10))
	{
		if (Approach(pos, m_ptRotatePone))
		{
			return CROSS_AREA;//旋转区域
		}
		else if (Approach(pos, m_ptResizePone))
		{
			return HOR_AREA;//大小改变区域
		}
		else if (Approach(pos, m_ptReangelPone))
		{
			return VER_AREA;//夹角改变区域
		}
		return SIZEALL_AREA;
	}
	return ARROR_AREA;
}

QPolygonF QuadriItem::Polygon(QPointF LVertex, QPointF RVertex) const
{
	QPolygonF ans;
	ans << LVertex << RVertex
		<< QPointF(-LVertex.x(), -LVertex.y())
		<< QPointF(-RVertex.x(), -RVertex.y());
	return ans;
}

bool QuadriItem::Approach(QPointF p1, QPointF p2)
{
	if ((p1 - p2).manhattanLength() <= 4)
	{
		return true;
	}
	return false;
}
