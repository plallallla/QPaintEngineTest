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

//���ǻ���QGraphicsView������View��
//֧�ֶ�̬����ͼƬ��ɾ��ͼƬ���Ҽ�����Ӧ���м��϶�
//֧�ּ���뾶�ɱ��Բ�Ρ�Բ�����нǱ߳��ɱ���ı��Σ�֧��ɾ����ע
//֧�����ô��ļ��жԻ���ĳ�ʼ��ʾ·��
class ROIVIEW_CLASS_EXPORT ROIView :
	public QGraphicsView
{

	Q_OBJECT

public:
	ROIView(QWidget * parent = nullptr);
	ROIView(ROIScene * scene,QWidget * parent = nullptr);
	~ROIView();
	//��Ĭ���ļ��д��ļ��Ի�����ѡ��ͼƬ����ͼƬ�����ñ�ע������
	void OpenPicture();
	//���ļ�·��Ϊpicture��ͼƬ�����ñ�ע������
	void OpenPicture(QString picture);
	//����QImage��ʽ�洢��ͼƬ�����ñ�ע������
	void OpenPicture(const QImage * picture);
	//�Ƴ��Ѿ������ͼƬ
	void RemovePicture();
	//�Ƴ�ѡ�е�ROI��ע��δѡ����ɾ��ȫ��ROI
	void RemoveROI();
	//����ͼƬ��С��λ��
	void ResetPicture();
	//���ô��ļ��е�Ĭ��·��
	void SetFolderPath(QString path);
	//��������
	void Measure();
	//�������ֵ�������
	void AddText(const QString & str , const QFont & font = QFont("����", 10), QPointF defaultPosition = QPointF());
	//��ʾ��ŵ�QGraphicsTextItemʵ����Ĭ����ɫ
	void ShowTextIndex(const QString & text, const QFont & font = QFont("����", 10), const QColor& color = QColor(0, 255, 0), QPointF defaultPosition = QPointF(0, 0));
	//��ʾ�����QGraphicsTextItemʵ����Ĭ�Ϻ�ɫ
	void ShowTextResult(const QString & text, const QFont & font = QFont("����", 10), const QColor& color = QColor(255, 0, 0), QPointF defaultPosition = QPointF(0, 20));
	//�����ı��ε������У�������ˮƽ�߳���б�߱߳����ı��μнǣ������þ�ΪĬ�ϲ���
	void AddQuadri(qreal defaultVerticalLength = -1, qreal defaultBevelLength = -1, qreal defaultAngel = -1, QPointF defaultPosition = QPointF());
	//������ε������У������ÿ��ߡ�λ�ã������þ�ΪĬ�ϲ���
	void AddRect(qreal defaultWidth = -1, qreal defaultHeight = -1, QPointF defaultPosition = QPointF());
	//����Բ�ε������У������ð뾶��λ�ã������þ�ΪĬ�ϲ���
	void AddCircle(qreal defaultRadius = -1, QPointF defaultPosition = QPointF());
	//����ͬ��Բ�������У������ô�뾶��С�뾶��λ�ã������þ�ΪĬ�ϲ���
	void AddDonut(qreal defaultBigRadius = -1, qreal defaultSmallRadius = -1, QPointF defaultPosition = QPointF());
	//���ü����ı���ʱ��Ĭ��ˮƽ�߳���б�߱߳���нǴ�С
	void SetDefaultQuadri(qreal v, qreal b, qreal angel);
	//���ü������ʱ��Ĭ�ϳ���
	void SetDefaultRect(qreal w, qreal h);
	//���ü���Բ��ʱ��Ĭ�ϰ뾶
	void SetDefaultCircle(qreal r);
	//���ü�����Բʱ��Ĭ�ϰ뾶
	void SetDefaultDonut(qreal bigr, qreal smallr);
	//��ȡSceneָ��
	ROIScene * GetROIScene();
	//ȷ���Ƿ�ӵ��ͼƬ
	bool HasPicture();
	void SetOutColor(const QColor & color);
	void TestNew();
	void TestQuadri();
	void AddPoly();

signals:
	void PositionMessage(QString);
	void MeasureMessage(QLineF);

protected:
	//����Ϊ�¼�����¼��������¼���Ӧ����
	//��������š�ƽ�ơ���ԭ�Ĺ���
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

