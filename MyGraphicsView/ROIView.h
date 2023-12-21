#pragma once
#include <QGraphicsView>
#include <qfiledialog.h>
#include <QPointF>
#include "ROIScene.h"
#include "ROIItems.h"
#include "items.h"

#if defined(ROIVIEW_EXPORT) 
#define ROIVIEW_CLASS_EXPORT __declspec(dllexport) 
#else 
#define ROIVIEW_CLASS_EXPORT __declspec(dllimport) 
#endif

//这是基于QGraphicsView派生的View类
//支持动态加载图片、删除图片、右键自适应、中键拖动
//支持加入半径可变的圆形、圆环、夹角边长可变的四边形，支持删除标注
//支持设置打开文件夹对话框的初始显示路径
class ROIVIEW_CLASS_EXPORT ROIView :
	public QGraphicsView
{

	Q_OBJECT

public:
	ROIView(QWidget * parent = nullptr);
	ROIView(ROIScene * scene,QWidget * parent = nullptr);
	~ROIView();
	//从默认文件夹打开文件对话框以选择图片，打开图片，设置标注的最大边
	void OpenPicture();
	//打开文件路径为picture的图片，设置标注的最大边
	void OpenPicture(QString picture);
	//打开以QImage形式存储的图片，设置标注的最大边
	void OpenPicture(const QImage * picture);
	//移除已经载入的图片
	void RemovePicture();
	//移除选中的ROI标注，未选中则删除全部ROI
	void RemoveROI();
	//重置图片大小与位置
	void ResetPicture();
	//设置打开文件夹的默认路径
	void SetFolderPath(QString path);
	//测量距离
	void Measure();
	//加入文字到画面中
	void AddText(const QString & str , const QFont & font = QFont("宋体", 10), QPointF defaultPosition = QPointF());
	//显示序号的QGraphicsTextItem实例，默认绿色
	void ShowTextIndex(const QString & text, const QFont & font = QFont("宋体", 10), const QColor& color = QColor(0, 255, 0), QPointF defaultPosition = QPointF(0, 0));
	//显示结果的QGraphicsTextItem实例，默认红色
	void ShowTextResult(const QString & text, const QFont & font = QFont("宋体", 10), const QColor& color = QColor(255, 0, 0), QPointF defaultPosition = QPointF(0, 20));
	//加入四边形到画面中，可设置水平边长、斜边边长、四边形夹角，不设置均为默认参数
	void AddQuadri(qreal defaultVerticalLength = -1, qreal defaultBevelLength = -1, qreal defaultAngel = -1, QPointF defaultPosition = QPointF());
	//加入矩形到画面中，可设置宽、高、位置，不设置均为默认参数
	void AddRect(qreal defaultWidth = -1, qreal defaultHeight = -1, QPointF defaultPosition = QPointF());
	//加入圆形到画面中，可设置半径、位置，不设置均为默认参数
	void AddCircle(qreal defaultRadius = -1, QPointF defaultPosition = QPointF());
	//加入同心圆到画面中，可设置大半径、小半径、位置，不设置均为默认参数
	void AddDonut(qreal defaultBigRadius = -1, qreal defaultSmallRadius = -1, QPointF defaultPosition = QPointF());
	//设置加入四边形时的默认水平边长、斜边边长与夹角大小
	void SetDefaultQuadri(qreal v, qreal b, qreal angel);
	//设置加入矩形时的默认长宽
	void SetDefaultRect(qreal w, qreal h);
	//设置加入圆形时的默认半径
	void SetDefaultCircle(qreal r);
	//设置加入椭圆时的默认半径
	void SetDefaultDonut(qreal bigr, qreal smallr);
	//获取Scene指针
	ROIScene * GetROIScene();
	//确认是否拥有图片
	bool HasPicture();
	void SetOutColor(const QColor & color);
	void TestNew();
	void TestQuadri();
	void AddPoly();

signals:
	void PositionMessage(QString);
	void MeasureMessage(QLineF);

protected:
	//以下为事件鼠标事件、滚轮事件响应函数
	//完成了缩放、平移、还原的功能
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
private:
	QColor m_OutColor = Qt::gray;
	qreal m_dDefaultWidth = 150;
	qreal m_dDefaultHeight = 200;
	qreal m_dDefaultRadius = 80;
	qreal m_dDefaultBigRadius = 90;
	qreal m_dDefaultSmallRadius = 40;
	qreal m_dDafaultVerticalLength = 200;
	qreal m_dDafaultBevelLength = 200;
	qreal m_dDafaultAngel = 45;
	QString m_qstrFolderPath = "";
	ROIScene *m_ptrScene;
	QPointF m_pPressed;
	QGraphicsLineItem MeasureLine;
	QGraphicsTextItem* m_textIndex;
	QGraphicsTextItem* m_textResult;
	bool isMousePressed = false;
	bool isMicro = false;
	bool Drag = false;
	bool Measuring = false;
};

