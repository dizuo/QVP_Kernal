#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>


QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE


class VPConnector;
class AGraphExec;
//! [0]
class VPLink : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    VPLink(VPConnector* start, VPConnector* end,
			int id,
			AGraphExec* g,
			 QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

	~VPLink();

    QRectF boundingRect() const;
    QPainterPath shape() const;
	int type() const	{ return Type; }

	bool error() const  { return myError; }
	int	 id() const		{ return myId; }

    VPConnector*	startItem() const	{ return myStart; }
    VPConnector*	endItem() const		{ return myEnd; }

	void remove(VPConnector *c);
	
	void setColor(const QColor& color) { myColor = color; }
	void setRect(const QPointF& point1, const QPointF& point2);

public:
    void redraw();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
	
private:
	VPConnector*	myStart;
	VPConnector*	myEnd;
	AGraphExec*		graphSingleton;

	int				myId;
	int				mySlot;

	bool			myError;
	QColor			myColor;

	static int		lineWidth;

	//QPolygonF		arrowHead;

	//////////////////////////////////////////////////////////////////////////
	void	initializePoints();
	int		computeBezierControlPoints();

	QVector<QPointF> controlPoints;
};
//! [0]

// Arrow has:
//		startItem	|	startItemSlotIdx
//		endItem		|	endItemSlotIdx

#endif
