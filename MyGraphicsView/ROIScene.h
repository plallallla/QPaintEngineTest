#pragma once
#include <qfile.h>
#include <QGraphicsScene>
#include <qgraphicsview.h>

//���ǻ���QGraphicsScene������Scene��
//֧�����ñ���ͼƬ��֧�ֻ�ȡ��ʾ�����������ʾͼƬ�����Image��
class ROIScene :
	public QGraphicsScene
{
public:
	//Ĭ�Ͽչ��캯��
	ROIScene(QWidget *parent = nullptr);
	//Ĭ�Ͽ���������
	~ROIScene();
	//����ͼƬ�������ɹ�����true
	bool SetBackground(const QString &);
	void SetBackground(const QImage *);
	//����ʵ����ʾͼ������
	QRectF GetShowRect();
	//���ر���ͼƬ�����Image����
	QImage GetBackGround();
	//�Ƴ�����ͼƬ
	void removeBackGround();
protected:
	void drawBackground(QPainter *painter, const QRectF &rect) override;
private:
	QImage background;
};

