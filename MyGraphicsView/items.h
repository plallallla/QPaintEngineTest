#pragma once
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <math.h>

//经由更新后矩形类已经集成到四边形类中
//这个类派生的其他图形有待需求的进一步明确后更新
//***********************************************
//这是基于QGraphicsItem派生的Item类
//支持选中后光标的动态变化、中心对称的形状更改、点按中心可以移动
//可用于设置子类笔刷、画刷

#if defined(ROIVIEW_EXPORT) 
#define ROIVIEW_CLASS_EXPORT __declspec(dllexport) 
#else 
#define ROIVIEW_CLASS_EXPORT __declspec(dllimport) 
#endif

class BaseItem :
	public QGraphicsItem
{
public:
	//共派生除三种实际图形
	enum TYPE
	{
		RECT,//矩形
		CIRCLE,//圆形
		DONUT,//同心圆环
		POLYGON,//平行四边形
		QUADRI,//超级四边形
	};
	explicit BaseItem(QGraphicsItem *parent = nullptr);
	~BaseItem();
	//设置元素外接矩形
	void SetRect(const QRectF &rect);
	//设置元素外接矩形
	inline void SetRect(qreal x, qreal y, qreal w, qreal h);
	//设置选中/未选中笔刷，同心圆有重新增加的接口
	void SetSelectedPen(const QPen &pen);
	void SetNotSelectedPen(const QPen &pen);
	//设置选中/未选中画刷
	void SetSelectedBrush(const QBrush &brush);
	void SetNotSelectedBrush(const QBrush &brush);
	//最大边设置函数
	static void SetMaxWidth(qreal);
	static void SetMaxHeight(qreal);
	//需要子类重载类型判断函数,用于后续需求明确后的标注设置
	virtual TYPE Type() = 0;
protected:
	enum SHAPEAREA
	{
		SIZEALL_AREA,//拖动中心区域
		ARROR_AREA,//箭头内部区域
		FDIAG_AREA,//左上与右下斜向双箭头区域
		BDIAG_AREA,//左下与右上斜向双箭头区域
		VER_AREA,//上下垂直双箭头区域
		HOR_AREA,//左右水平双箭头区域
		CROSS_AREA,
	};
	//标注最大边长
	static qreal maxW;
	static qreal maxH;
	//画刷
	QBrush m_selectedbrush = QBrush(QColor(0, 0, 0, 40));
	QBrush m_notselbrush = QBrush(QColor(0, 0, 0, 20));
	//笔刷
	QPen m_selectedpen = QPen(Qt::green);
	QPen m_notselpen = QPen(Qt::red);
	//外接矩形
	QRectF m_rect;
	//边界矩形
	mutable QRectF m_boundingRect;
	//完成边界函数的重载
	QRectF boundingRect() const override;
	//需要子类重载鼠标位置判断函数
	virtual SHAPEAREA whereMyPos(QPointF pos) = 0;
};

inline void BaseItem::SetRect(qreal ax, qreal ay, qreal w, qreal h)
{
	SetRect(QRectF(ax, ay, w, h));
}

//这是基于BaseItem派生的矩形类
//可获取的特征为四个端点
class ROIVIEW_CLASS_EXPORT RectItem :
	public BaseItem
{
public:
	enum ABILITY//矩形行为标志
	{
		ABLETOMOVE,//可移动标志
		ABLETOROTATE,//可旋转标志
		ABLETORESIZE,//可改变大小标志
	};
	//默认空构造函数
	explicit RectItem(QGraphicsItem *parent = nullptr);
	//传入矩形的长宽作为构造函数参数
	explicit RectItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);
	//默认空析构函数
	~RectItem();
	//将左上角，右上角，右下角，左下角的坐标输出到QList中并返回
	QList<QPointF> rect();
	//重写类型判断函数
	TYPE Type() override;
	//重写的碰撞检测函数，实际返回图像的可选中区域
	QPainterPath shape() const;
	//重写的图像绘制函数
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
	//设置行为能力函数、传入参数为矩形的行为标志
	void SetAbility(ABILITY ability, bool tag);
protected:
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
	SHAPEAREA whereMyPos(QPointF pos) override;
	enum CHANGEMODE//形变区域
	{
		NONE,//无变化
		UP_LEFT,//左上角变化
		UP,//上方变化
		UP_RIGHT,//右上角变化
		RIGHT,//右方变化
		DOWN_RIGHT,//右下角变化
		DOWN,//下方变化
		DOWN_LEFT,//左下角变化
		LEFT,//左方变化
		ROTATE,//旋转变化
	} changeMode;
	bool abletomove = true;//移动能力标记
	bool abletorotate = true;//旋转能力标记
	bool abletoresize = true;//缩放能力标记
	bool change = false;//形变允许标记
	QPointF stableUL;
	QPointF stableBR;
	QPointF stableUR;
	QPointF stableBL;
};

//这是基于BaseItem派生的圆形类
//可获取的特征为半径与圆心
class ROIVIEW_CLASS_EXPORT CircleItem :
	public BaseItem
{
public:
	//默认空构造函数
	explicit CircleItem(QGraphicsItem *parent = nullptr);
	//传入半径值作为参数的构造函数
	explicit CircleItem(qreal r, QGraphicsItem *parent = nullptr);
	~CircleItem();
	//将圆心横坐标，纵坐标，半径输出到QList中并返回
	QList<qreal> circle();
	//设置圆形的半径
	void setRadius(qreal r);
	//重写类型判断函数
	TYPE Type() override;
	//重写的碰撞检测函数，实际返回图像的可选中区域
	QPainterPath shape() const;
	//重写的图像绘制函数
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
protected:
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	SHAPEAREA whereMyPos(QPointF pos) override;
private:
	bool change = false;
	int m_radius;
};

//这是基于BaseItem派生的圆环类
//可获取的特征为大半径、小半径与圆心
//内圆、外圆的笔刷提供了接口设置
class ROIVIEW_CLASS_EXPORT DonutItem :
	public BaseItem
{
public:
	//默认空构造函数
	explicit DonutItem(QGraphicsItem *parent = nullptr);
	//传入半径值作为参数的构造函数
	explicit DonutItem(qreal bigr, qreal smallr, QGraphicsItem *parent = nullptr);
	~DonutItem();
	//将圆心横坐标，纵坐标，大半径，小半径输出到QList中并返回
	QList<qreal> donut();
	//设置小圆形的半径
	void SetSmallRadius(qreal r);
	//设置大圆形的半径
	void SetBigRadius(qreal r);
	//设置选中的小圆圆环颜色
	void SetSelectedSmallPen(const QPen &pen);
	//设置选中的大圆圆环颜色
	void SetSelectedBigPen(const QPen &pen);
	//重写类型判断函数
	TYPE Type() override;
	//重写的碰撞检测函数，实际返回图像的可选中区域
	QPainterPath shape() const;
	//重写的图像绘制函数
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
protected:
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private:
	bool change = false;
	qreal m_SmallRadius;
	qreal m_BigRaidus;
	QPen m_SelectedSmallPen = QPen(Qt::yellow);
	QPen m_SelectedBigPen = QPen(Qt::blue);
	SHAPEAREA whereMyPos(QPointF pos) override;
};

//平行四边形类
//此方案特征为长短边、角度
class ROIVIEW_CLASS_EXPORT PolygonItem :
	public BaseItem
{
public:
	//默认空构造函数
	explicit PolygonItem(QGraphicsItem *parent = nullptr);
	//传入长宽与角度作为构造函数参数
	explicit PolygonItem(qreal l, qreal s, qreal angel, QGraphicsItem *parent = nullptr);
	//默认析构函数
	~PolygonItem();
	//设置平行四边形形状
	void SetPoly();
	//设置角度并刷新
	void SetAngel(qreal angel);
	//重写类型判断函数
	TYPE Type() override;
	//重写的碰撞检测函数，实际返回图像的可选中区域
	QPainterPath shape() const;
	//重写的图像绘制函数
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
protected:
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
	//属性成员变量
	QPolygonF m_qpfPoly;//多边形
	qreal m_dbVerLength;//水平边长
	qreal m_dbHorLength;//斜边长
	qreal m_dbAngel;//角度
	QPointF m_ptCenter;//平行四边形中心
	//动作功能完成的承载成员变量
	enum CHANGEMODE
	{
		NONE,
		REANGEL,
		RESIZE,
	} changeMode;
	QPointF m_ptStart;//平行四边形中心
	QPointF m_ptLeft;//平行四边形左边中心
	QPointF m_ptUR;//平行四边形右上角
	bool change = false;//形变允许标记
	//需要子类重载鼠标位置判断函数
	SHAPEAREA whereMyPos(QPointF pos) override;
};


//已经将完全体更新到了ROIItem中的派生类中
//这是基于BaseItem派生的矩形类
//可获取的特征为四个端点
class ROIVIEW_CLASS_EXPORT QuadriItem :
	public BaseItem
{
public:
	//默认空构造函数
	explicit QuadriItem(QGraphicsItem *parent = nullptr);
	//传入四边形的水平边边长、斜边边长、角度作为构造函数参数
	explicit QuadriItem(qreal v, qreal b, qreal angel = 90, QGraphicsItem *parent = nullptr);
	//默认空析构函数
	~QuadriItem();
	//特征更新处理函数
	void Reset(qreal v, qreal b, qreal angel);
	//将左上角，右上角，右下角，左下角的坐标输出到QList中并返回
	QList<QPointF> vertex();
	//重写类型判断函数
	TYPE Type() override;
	//重写的碰撞检测函数，实际返回图像的可选中区域
	QPainterPath shape() const;
	//重写的图像绘制函数
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
protected:
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
	SHAPEAREA whereMyPos(QPointF pos) override;
	//根据左上角、右上角顶点生成四边形区域
	QPolygonF Polygon(QPointF LVertex, QPointF RVertex) const;
	//判断两点位置是否足够接近
	bool Approach(QPointF p1, QPointF p2);
	//特征成员变量
	qreal m_dbVerticalLength;//水平边边长
	qreal m_dbBevelLength;//斜边边长
	qreal m_dbAngel;//左上角夹角角度
	//缓存成员变量
	//这些成员变量需要内部生成、频繁访问，因此设立缓存以提高性能
	QPointF m_ptDLVertex;//左上角顶点坐标
	QPointF m_ptDRVertex;//右上角顶点坐标
	QPointF m_ptULVertex;//左下角顶点坐标
	QPointF m_ptURVertex;//右下角顶点坐标
	QPolygonF m_Polygon;//实时绘制形状
	QPolygonF m_tPolygon;//临时绘制形状
	QPainterPath m_Shape;//碰撞检测
	//行为支撑变量
	bool m_bChanging = false;//正在改变检查标记
	QPointF m_ptClick;//鼠标点击位置
	QPointF m_ptRotatePone;//旋转锚点
	QPointF m_ptReangelPone;//夹角改变锚点
	QPointF m_ptResizePone;//大小改变锚点
	QPointF m_ptULStable;//左上角的场景坐标
	QPointF m_ptDRStable;//右下角的场景坐标

	//动作功能完成的承载成员变量
	enum CHANGEMODE
	{
		NONE,
		REANGEL,
		ROTATE,
		RESIZE,
	} changeMode;
	QPointF m_ptDownCenterPoint;
	qreal m_dbRotateAngel = 0;
	//鼠标光标
	QPixmap m_curRotation = QPixmap("./Cursor/Rotation.png");
	QPixmap m_curReangel = QPixmap("./Cursor/Parallelogram.png");
};

