#pragma once
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <math.h>

//���ɸ��º�������Ѿ����ɵ��ı�������
//���������������ͼ���д�����Ľ�һ����ȷ�����
//***********************************************
//���ǻ���QGraphicsItem������Item��
//֧��ѡ�к���Ķ�̬�仯�����ĶԳƵ���״���ġ��㰴���Ŀ����ƶ�
//���������������ˢ����ˢ

#if defined(ROIVIEW_EXPORT) 
#define ROIVIEW_CLASS_EXPORT __declspec(dllexport) 
#else 
#define ROIVIEW_CLASS_EXPORT __declspec(dllimport) 
#endif

class BaseItem :
	public QGraphicsItem
{
public:
	//������������ʵ��ͼ��
	enum TYPE
	{
		RECT,//����
		CIRCLE,//Բ��
		DONUT,//ͬ��Բ��
		POLYGON,//ƽ���ı���
		QUADRI,//�����ı���
	};
	explicit BaseItem(QGraphicsItem *parent = nullptr);
	~BaseItem();
	//����Ԫ����Ӿ���
	void SetRect(const QRectF &rect);
	//����Ԫ����Ӿ���
	inline void SetRect(qreal x, qreal y, qreal w, qreal h);
	//����ѡ��/δѡ�б�ˢ��ͬ��Բ���������ӵĽӿ�
	void SetSelectedPen(const QPen &pen);
	void SetNotSelectedPen(const QPen &pen);
	//����ѡ��/δѡ�л�ˢ
	void SetSelectedBrush(const QBrush &brush);
	void SetNotSelectedBrush(const QBrush &brush);
	//�������ú���
	static void SetMaxWidth(qreal);
	static void SetMaxHeight(qreal);
	//��Ҫ�������������жϺ���,���ں���������ȷ��ı�ע����
	virtual TYPE Type() = 0;
protected:
	enum SHAPEAREA
	{
		SIZEALL_AREA,//�϶���������
		ARROR_AREA,//��ͷ�ڲ�����
		FDIAG_AREA,//����������б��˫��ͷ����
		BDIAG_AREA,//����������б��˫��ͷ����
		VER_AREA,//���´�ֱ˫��ͷ����
		HOR_AREA,//����ˮƽ˫��ͷ����
		CROSS_AREA,
	};
	//��ע���߳�
	static qreal maxW;
	static qreal maxH;
	//��ˢ
	QBrush m_selectedbrush = QBrush(QColor(0, 0, 0, 40));
	QBrush m_notselbrush = QBrush(QColor(0, 0, 0, 20));
	//��ˢ
	QPen m_selectedpen = QPen(Qt::green);
	QPen m_notselpen = QPen(Qt::red);
	//��Ӿ���
	QRectF m_rect;
	//�߽����
	mutable QRectF m_boundingRect;
	//��ɱ߽纯��������
	QRectF boundingRect() const override;
	//��Ҫ�����������λ���жϺ���
	virtual SHAPEAREA whereMyPos(QPointF pos) = 0;
};

inline void BaseItem::SetRect(qreal ax, qreal ay, qreal w, qreal h)
{
	SetRect(QRectF(ax, ay, w, h));
}

//���ǻ���BaseItem�����ľ�����
//�ɻ�ȡ������Ϊ�ĸ��˵�
class ROIVIEW_CLASS_EXPORT RectItem :
	public BaseItem
{
public:
	enum ABILITY//������Ϊ��־
	{
		ABLETOMOVE,//���ƶ���־
		ABLETOROTATE,//����ת��־
		ABLETORESIZE,//�ɸı��С��־
	};
	//Ĭ�Ͽչ��캯��
	explicit RectItem(QGraphicsItem *parent = nullptr);
	//������εĳ�����Ϊ���캯������
	explicit RectItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);
	//Ĭ�Ͽ���������
	~RectItem();
	//�����Ͻǣ����Ͻǣ����½ǣ����½ǵ����������QList�в�����
	QList<QPointF> rect();
	//��д�����жϺ���
	TYPE Type() override;
	//��д����ײ��⺯����ʵ�ʷ���ͼ��Ŀ�ѡ������
	QPainterPath shape() const;
	//��д��ͼ����ƺ���
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
	//������Ϊ�����������������Ϊ���ε���Ϊ��־
	void SetAbility(ABILITY ability, bool tag);
protected:
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
	SHAPEAREA whereMyPos(QPointF pos) override;
	enum CHANGEMODE//�α�����
	{
		NONE,//�ޱ仯
		UP_LEFT,//���ϽǱ仯
		UP,//�Ϸ��仯
		UP_RIGHT,//���ϽǱ仯
		RIGHT,//�ҷ��仯
		DOWN_RIGHT,//���½Ǳ仯
		DOWN,//�·��仯
		DOWN_LEFT,//���½Ǳ仯
		LEFT,//�󷽱仯
		ROTATE,//��ת�仯
	} changeMode;
	bool abletomove = true;//�ƶ��������
	bool abletorotate = true;//��ת�������
	bool abletoresize = true;//�����������
	bool change = false;//�α�������
	QPointF stableUL;
	QPointF stableBR;
	QPointF stableUR;
	QPointF stableBL;
};

//���ǻ���BaseItem������Բ����
//�ɻ�ȡ������Ϊ�뾶��Բ��
class ROIVIEW_CLASS_EXPORT CircleItem :
	public BaseItem
{
public:
	//Ĭ�Ͽչ��캯��
	explicit CircleItem(QGraphicsItem *parent = nullptr);
	//����뾶ֵ��Ϊ�����Ĺ��캯��
	explicit CircleItem(qreal r, QGraphicsItem *parent = nullptr);
	~CircleItem();
	//��Բ�ĺ����꣬�����꣬�뾶�����QList�в�����
	QList<qreal> circle();
	//����Բ�εİ뾶
	void setRadius(qreal r);
	//��д�����жϺ���
	TYPE Type() override;
	//��д����ײ��⺯����ʵ�ʷ���ͼ��Ŀ�ѡ������
	QPainterPath shape() const;
	//��д��ͼ����ƺ���
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
protected:
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	SHAPEAREA whereMyPos(QPointF pos) override;
private:
	bool change = false;
	int m_radius;
};

//���ǻ���BaseItem������Բ����
//�ɻ�ȡ������Ϊ��뾶��С�뾶��Բ��
//��Բ����Բ�ı�ˢ�ṩ�˽ӿ�����
class ROIVIEW_CLASS_EXPORT DonutItem :
	public BaseItem
{
public:
	//Ĭ�Ͽչ��캯��
	explicit DonutItem(QGraphicsItem *parent = nullptr);
	//����뾶ֵ��Ϊ�����Ĺ��캯��
	explicit DonutItem(qreal bigr, qreal smallr, QGraphicsItem *parent = nullptr);
	~DonutItem();
	//��Բ�ĺ����꣬�����꣬��뾶��С�뾶�����QList�в�����
	QList<qreal> donut();
	//����СԲ�εİ뾶
	void SetSmallRadius(qreal r);
	//���ô�Բ�εİ뾶
	void SetBigRadius(qreal r);
	//����ѡ�е�СԲԲ����ɫ
	void SetSelectedSmallPen(const QPen &pen);
	//����ѡ�еĴ�ԲԲ����ɫ
	void SetSelectedBigPen(const QPen &pen);
	//��д�����жϺ���
	TYPE Type() override;
	//��д����ײ��⺯����ʵ�ʷ���ͼ��Ŀ�ѡ������
	QPainterPath shape() const;
	//��д��ͼ����ƺ���
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

//ƽ���ı�����
//�˷�������Ϊ���̱ߡ��Ƕ�
class ROIVIEW_CLASS_EXPORT PolygonItem :
	public BaseItem
{
public:
	//Ĭ�Ͽչ��캯��
	explicit PolygonItem(QGraphicsItem *parent = nullptr);
	//���볤����Ƕ���Ϊ���캯������
	explicit PolygonItem(qreal l, qreal s, qreal angel, QGraphicsItem *parent = nullptr);
	//Ĭ����������
	~PolygonItem();
	//����ƽ���ı�����״
	void SetPoly();
	//���ýǶȲ�ˢ��
	void SetAngel(qreal angel);
	//��д�����жϺ���
	TYPE Type() override;
	//��д����ײ��⺯����ʵ�ʷ���ͼ��Ŀ�ѡ������
	QPainterPath shape() const;
	//��д��ͼ����ƺ���
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
protected:
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
	//���Գ�Ա����
	QPolygonF m_qpfPoly;//�����
	qreal m_dbVerLength;//ˮƽ�߳�
	qreal m_dbHorLength;//б�߳�
	qreal m_dbAngel;//�Ƕ�
	QPointF m_ptCenter;//ƽ���ı�������
	//����������ɵĳ��س�Ա����
	enum CHANGEMODE
	{
		NONE,
		REANGEL,
		RESIZE,
	} changeMode;
	QPointF m_ptStart;//ƽ���ı�������
	QPointF m_ptLeft;//ƽ���ı����������
	QPointF m_ptUR;//ƽ���ı������Ͻ�
	bool change = false;//�α�������
	//��Ҫ�����������λ���жϺ���
	SHAPEAREA whereMyPos(QPointF pos) override;
};


//�Ѿ�����ȫ����µ���ROIItem�е���������
//���ǻ���BaseItem�����ľ�����
//�ɻ�ȡ������Ϊ�ĸ��˵�
class ROIVIEW_CLASS_EXPORT QuadriItem :
	public BaseItem
{
public:
	//Ĭ�Ͽչ��캯��
	explicit QuadriItem(QGraphicsItem *parent = nullptr);
	//�����ı��ε�ˮƽ�߱߳���б�߱߳����Ƕ���Ϊ���캯������
	explicit QuadriItem(qreal v, qreal b, qreal angel = 90, QGraphicsItem *parent = nullptr);
	//Ĭ�Ͽ���������
	~QuadriItem();
	//�������´�����
	void Reset(qreal v, qreal b, qreal angel);
	//�����Ͻǣ����Ͻǣ����½ǣ����½ǵ����������QList�в�����
	QList<QPointF> vertex();
	//��д�����жϺ���
	TYPE Type() override;
	//��д����ײ��⺯����ʵ�ʷ���ͼ��Ŀ�ѡ������
	QPainterPath shape() const;
	//��д��ͼ����ƺ���
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
protected:
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
	SHAPEAREA whereMyPos(QPointF pos) override;
	//�������Ͻǡ����ϽǶ��������ı�������
	QPolygonF Polygon(QPointF LVertex, QPointF RVertex) const;
	//�ж�����λ���Ƿ��㹻�ӽ�
	bool Approach(QPointF p1, QPointF p2);
	//������Ա����
	qreal m_dbVerticalLength;//ˮƽ�߱߳�
	qreal m_dbBevelLength;//б�߱߳�
	qreal m_dbAngel;//���ϽǼнǽǶ�
	//�����Ա����
	//��Щ��Ա������Ҫ�ڲ����ɡ�Ƶ�����ʣ���������������������
	QPointF m_ptDLVertex;//���ϽǶ�������
	QPointF m_ptDRVertex;//���ϽǶ�������
	QPointF m_ptULVertex;//���½Ƕ�������
	QPointF m_ptURVertex;//���½Ƕ�������
	QPolygonF m_Polygon;//ʵʱ������״
	QPolygonF m_tPolygon;//��ʱ������״
	QPainterPath m_Shape;//��ײ���
	//��Ϊ֧�ű���
	bool m_bChanging = false;//���ڸı�����
	QPointF m_ptClick;//�����λ��
	QPointF m_ptRotatePone;//��תê��
	QPointF m_ptReangelPone;//�нǸı�ê��
	QPointF m_ptResizePone;//��С�ı�ê��
	QPointF m_ptULStable;//���Ͻǵĳ�������
	QPointF m_ptDRStable;//���½ǵĳ�������

	//����������ɵĳ��س�Ա����
	enum CHANGEMODE
	{
		NONE,
		REANGEL,
		ROTATE,
		RESIZE,
	} changeMode;
	QPointF m_ptDownCenterPoint;
	qreal m_dbRotateAngel = 0;
	//�����
	QPixmap m_curRotation = QPixmap("./Cursor/Rotation.png");
	QPixmap m_curReangel = QPixmap("./Cursor/Parallelogram.png");
};

