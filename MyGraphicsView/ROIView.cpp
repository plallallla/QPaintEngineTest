#include "ROIView.h"
#include "ROIView.h"
#include "ROIView.h"
#include <QWheelEvent>
#include <qscrollbar.h>
#include <QDebug>
#include <QMessageBox>

ROIView::ROIView(QWidget* parent) : QGraphicsView(parent)
{
	m_ptrScene = new ROIScene();
	MeasureLine.setPen(QPen(Qt::red, 2));
	setScene(m_ptrScene);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);//强制全部刷新
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平条
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平条
	setStyleSheet(QString("#%0{background-color: rgb(%1,%2,%3);}").arg(objectName())
		.arg(m_OutColor.red()).arg(m_OutColor.green()).arg(m_OutColor.blue()));

	m_textIndex = new QGraphicsTextItem("");
	m_textResult = new QGraphicsTextItem("");
	m_ptrScene->addItem(m_textIndex);
	m_ptrScene->addItem(m_textResult);
}

ROIView::ROIView(ROIScene * scene, QWidget * parent)
{
	m_ptrScene = scene;
	setScene(m_ptrScene);
	MeasureLine.setPen(QPen(Qt::red, 2));
	isMicro = true;
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平条
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平条
	setStyleSheet(QString("#%0{background-color: rgb(%1,%2,%3);}").arg(objectName())
		.arg(m_OutColor.red()).arg(m_OutColor.green()).arg(m_OutColor.blue()));
}


ROIView::~ROIView()
{

}

void ROIView::OpenPicture()
{
	QString fileName = QFileDialog::getOpenFileName(
		this, "open image file",
		m_qstrFolderPath,
		"Image files (*.bmp *.jpg *.jpeg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
	if (fileName.isNull() || !m_ptrScene->SetBackground(fileName))
	{
		QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("打开图片文件失败！%1").arg(fileName));
		return;
	}
	ResetPicture();
	BaseItem::SetMaxHeight(m_ptrScene->GetShowRect().height());
	BaseItem::SetMaxWidth(m_ptrScene->GetShowRect().width());
}

void ROIView::OpenPicture(QString picture)
{
	if (!m_ptrScene->SetBackground(picture))
	{
		QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("打开图片文件失败！%1").arg(picture));
		return;
	}
	ResetPicture();
	BaseItem::SetMaxHeight(m_ptrScene->GetShowRect().height());
	BaseItem::SetMaxWidth(m_ptrScene->GetShowRect().width());
}

void ROIView::OpenPicture(const QImage * picture)
{
	m_ptrScene->SetBackground(picture);
	ResetPicture();
	BaseItem::SetMaxHeight(m_ptrScene->GetShowRect().height());
	BaseItem::SetMaxWidth(m_ptrScene->GetShowRect().width());
}

void ROIView::RemovePicture()
{
	m_ptrScene->removeBackGround();
}

void ROIView::RemoveROI()
{
	if (!m_ptrScene->selectedItems().isEmpty())
	{
		for (QGraphicsItem *i : m_ptrScene->selectedItems())
		{
			m_ptrScene->removeItem(i);
		}
	}
	else
	{
		for (QGraphicsItem *i : m_ptrScene->items())
		{
			m_ptrScene->removeItem(i);
		}
	}
}

void ROIView::ResetPicture()
{
	if (m_ptrScene->GetBackGround().isNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	QRectF b = m_ptrScene->GetShowRect();
	QRectF a = QRectF(b.x() - b.width(), b.y() - b.height(), 3 * b.width(), 3 * b.height());
	setSceneRect(a);
	fitInView(m_ptrScene->GetShowRect(), Qt::KeepAspectRatio);
}

void ROIView::SetFolderPath(QString path)
{
	m_qstrFolderPath = path;
}

void ROIView::Measure()
{
	if (m_ptrScene->GetBackGround().isNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	Measuring = true;
}

void ROIView::AddText(const QString & str, const QFont & font, QPointF defaultPosition)
{
	if (m_ptrScene->GetBackGround().isNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	QGraphicsTextItem * i = new QGraphicsTextItem(str);
	i->setFont(font);
	if (defaultPosition.isNull())
	{
		i->setPos(m_ptrScene->GetShowRect().center());
	}
	else
	{
		i->setPos(defaultPosition);
	}
	m_ptrScene->addItem(i);
}

void ROIView::ShowTextIndex(const QString & text, const QFont & font, const QColor& color, QPointF defaultPosition)
{
	if (m_ptrScene->GetBackGround().isNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	//m_textIndex->setPlainText(text);
	m_textIndex->setFont(font);
	m_textIndex->setDefaultTextColor(color);
	m_textIndex->setPos(defaultPosition);
	m_textIndex->setHtml(QString("<div style='background:rgba(255, 255, 255, 100%);'>" + text + QString("</div>")));
}

void ROIView::ShowTextResult(const QString & text, const QFont & font, const QColor& color, QPointF defaultPosition)
{
	if (m_ptrScene->GetBackGround().isNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	//m_textResult->setPlainText(text);
	m_textResult->setFont(font);
	m_textResult->setDefaultTextColor(color);
	m_textResult->setPos(defaultPosition);
	m_textResult->setHtml(QString("<div style='background:rgba(255, 255, 255, 100%);'>" + text + QString("</div>")));
}

void ROIView::AddQuadri(qreal defaultVerticalLength, qreal defaultBevelLength, qreal defaultAngel, QPointF defaultPosition)
{
	if (m_ptrScene->GetBackGround().isNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	ROIQuadriItem *i;
	if (defaultVerticalLength == -1 || defaultBevelLength == -1 || defaultAngel == -1)
	{
		i = new ROIQuadriItem(m_dDefaultWidth, m_dDefaultHeight, m_dDafaultAngel);
	}
	else
	{
		i = new ROIQuadriItem(defaultVerticalLength, defaultBevelLength, defaultAngel);
	}
	if (defaultPosition.isNull())
	{
		i->setPos(m_ptrScene->GetShowRect().center());
	}
	else
	{
		i->setPos(defaultPosition);
	}
	m_ptrScene->addItem(i);
}

void ROIView::AddRect(qreal defaultWidth, qreal defaultHeight, QPointF defaultPosition)
{
	if (m_ptrScene->GetBackGround().isNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	ROIQuadriItem *i;
	if (defaultWidth == -1 || defaultHeight == -1)
	{
		i = new ROIQuadriItem(m_dDefaultWidth, m_dDefaultHeight, 90);
	}
	else
	{
		i = new ROIQuadriItem(defaultWidth, defaultHeight, 90);
	}
	if (defaultPosition.isNull())
	{
		i->setPos(m_ptrScene->GetShowRect().center());
	}
	else
	{
		i->setPos(defaultPosition);
	}
	i->SetEnable(ROIBaseItem::REANGEL,false);
	m_ptrScene->addItem(i);
}

void ROIView::AddCircle(qreal defaultRadius, QPointF defaultPosition)
{
	if (m_ptrScene->GetBackGround().isNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	CircleItem *i;
	if (defaultRadius == -1)
	{
		i = new CircleItem(m_dDefaultRadius);
	}
	else
	{
		i = new CircleItem(defaultRadius);
	}
	if (defaultPosition.isNull())
	{
		i->setPos(m_ptrScene->GetShowRect().center());
	}
	else
	{
		i->setPos(defaultPosition);
	}
	m_ptrScene->addItem(i);
}

void ROIView::AddDonut(qreal defaultBigRadius, qreal defaultSmallRadius, QPointF defaultPosition)
{
	if (m_ptrScene->GetBackGround().isNull())
	{
		QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未添加图形！"));
		return;
	}
	DonutItem *i;
	if (defaultBigRadius == -1 || defaultSmallRadius == -1)
	{
		i = new DonutItem(m_dDefaultBigRadius, m_dDefaultSmallRadius);
	}
	else
	{
		if (defaultBigRadius <= defaultSmallRadius)
		{
			QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("圆环参数错误！"));
			return;
		}
		i = new DonutItem(defaultBigRadius, defaultSmallRadius);
	}
	if (defaultPosition.isNull())
	{
		i->setPos(m_ptrScene->GetShowRect().center());
	}
	else
	{
		i->setPos(defaultPosition);
	}
	m_ptrScene->addItem(i);
}

void ROIView::SetDefaultQuadri(qreal v, qreal b, qreal angel)
{
	m_dDafaultVerticalLength = v;
	m_dDafaultBevelLength = b;
	m_dDafaultAngel = angel;
}

void ROIView::SetDefaultRect(qreal w, qreal h)
{
	m_dDefaultWidth = w;
	m_dDefaultHeight = h;
}

void ROIView::SetDefaultCircle(qreal r)
{
	m_dDefaultRadius = r;
}

void ROIView::SetDefaultDonut(qreal bigr, qreal smallr)
{
	m_dDefaultBigRadius = bigr;
	m_dDefaultSmallRadius = smallr;
}

ROIScene * ROIView::GetROIScene()
{
	return m_ptrScene;
}

bool ROIView::HasPicture()
{
	return !m_ptrScene->GetBackGround().isNull();
}

void ROIView::SetOutColor(const QColor & color)
{
	m_OutColor = color;
	setStyleSheet(QString("#%0{background-color: rgb(%1,%2,%3);}").arg(objectName())
		.arg(m_OutColor.red()).arg(m_OutColor.green()).arg(m_OutColor.blue()));
}

void ROIView::TestNew()
{
	ROIQuadriItem *i = new ROIQuadriItem(100, 100, 45);
	i->setPos(m_ptrScene->GetShowRect().center());
	i->setFlag(QGraphicsItem::ItemIsMovable);
	i->setFlag(QGraphicsItem::ItemIsSelectable);
	m_ptrScene->addItem(i);
}

void ROIView::TestQuadri()
{
	ROIQuadriItem *i = new ROIQuadriItem(300, 400 ,45);
	i->setPos(m_ptrScene->GetShowRect().center());
	i->setFlag(QGraphicsItem::ItemIsMovable);
	i->setFlag(QGraphicsItem::ItemIsSelectable);
	m_ptrScene->addItem(i);
}

void ROIView::AddPoly()
{
	PolygonItem *i = new PolygonItem(100, 100, 45);
	i->setPos(m_ptrScene->GetShowRect().center());
	i->setFlag(QGraphicsItem::ItemIsMovable);
	i->setFlag(QGraphicsItem::ItemIsSelectable);
	m_ptrScene->addItem(i);
}

void ROIView::wheelEvent(QWheelEvent * event)
{
	if (isMicro)
	{
		return;
	}
	if ((event->delta() > 0) && (this->matrix().m11() >= 10))
	{
		return;
	}
	else if ((event->delta() < 0) && (this->matrix().m11() <= 0.01))
	{
		return;
	}
	else
	{
		int wheelDeltaValue = event->delta();
		if (wheelDeltaValue > 0)
		{
			scale(1.2, 1.2);
		}
		else
		{
			scale(1.0 / 1.2, 1.0 / 1.2);
		}
	}
}

void ROIView::mousePressEvent(QMouseEvent * event)
{
	if (isMicro)
	{
		return;
	}
	//中键点击拖动移动画面
	if (event->button() == Qt::MidButton)
	{
		QGraphicsView::mousePressEvent(event);
		m_pPressed = mapToScene(event->pos());
		if (m_ptrScene->selectedItems().length() == 0)
		{
			isMousePressed = true;
			Drag = true;
		}
	}
	//右键单击自适应画面
	else if (event->button() == Qt::RightButton)
	{
		ResetPicture();
	}
	//测量模式下左键划线测量
	else if (event->button() == Qt::LeftButton && Measuring)
	{
		
		MeasureLine.setLine(QLineF(mapToScene(event->pos()), mapToScene(event->pos())));
		m_ptrScene->addItem(&MeasureLine);
		isMousePressed = true;
	}
	QGraphicsView::mousePressEvent(event);
}

void ROIView::mouseMoveEvent(QMouseEvent * event)
{
	if (isMicro)
	{
		return;
	}
	QPoint scenePos = QPoint(mapToScene(event->pos()).x(), mapToScene(event->pos()).y());
	if (isMousePressed && Drag)
	{
		QPointF gap = 0.1*(mapToScene(event->pos()) - m_pPressed);
		verticalScrollBar()->setValue(-gap.y() + verticalScrollBar()->value());
		horizontalScrollBar()->setValue(-gap.x() + horizontalScrollBar()->value());
	}
	//范围限制
	if (!m_ptrScene->GetBackGround().isNull() && scenePos.x() >= 0 && scenePos.y() >= 0
		&& scenePos.x() <= m_ptrScene->GetShowRect().width()
		&& scenePos.y() <= m_ptrScene->GetShowRect().height())
	{
		if (Measuring && isMousePressed)
		{
			MeasureLine.setLine(QLineF(MeasureLine.line().p1(), mapToScene(event->pos())));
			emit MeasureMessage(MeasureLine.line());
			return;
		}
		QImage temp = m_ptrScene->GetBackGround();
		QRgb RGBvalue = temp.pixel(scenePos);
		QString msg = QString("Position:(%0,%1)").arg(scenePos.x()).arg(scenePos.y());
		msg += QString("    RGB:(%0,%1,%2)").arg(qRed(RGBvalue)).arg(qGreen(RGBvalue)).arg(qBlue(RGBvalue));
		emit PositionMessage(msg);
	}

	QGraphicsView::mouseMoveEvent(event);
}

void ROIView::mouseReleaseEvent(QMouseEvent * event)
{
	if (isMicro)
	{
		return;
	}
	if (Measuring)
	{
		m_ptrScene->removeItem(&MeasureLine);
		Measuring = false;
	}
	isMousePressed = false;
	if (Drag)
	{
		Drag = false;
	}
	QGraphicsView::mouseReleaseEvent(event);
}

