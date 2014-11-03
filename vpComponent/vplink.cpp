#include <QtGui>

#include "vpabstractnode.h"
#include "vplink.h"
#include <math.h>

#include "vpconnector.h"
#include "../em/agraph_exec.hh"


const	qreal Pi = 3.14;

//QColor	gLinkColor(50,205,50, 120);
QColor	gLinkColor(220,220,10, 150);
int		VPLink::lineWidth = 4;
//! [0]
VPLink::VPLink(VPConnector* start, VPConnector* end, 
			   int id, 
			   AGraphExec* g,
			   QGraphicsItem *parent /* = 0 */,
			   QGraphicsScene *scene /* = 0 */)
    : QGraphicsLineItem(parent, scene)
	, myId(id)
	, graphSingleton(g)
	, myError(false)
{
	myStart = start;
	myEnd = end;

	/* add flow to graph */
	myError |= ! graphSingleton->hang_flow_in( myEnd->parent()->getId(),
										myId,
										myEnd->number() );
	if (!myError)
	{
		int tmp =
			graphSingleton->hang_flow_out( myStart->parent()->getId(),
									myId,
									myStart->number() );
		if (tmp <= -1)
			myError = true;
		else
			mySlot = (unsigned)tmp;
	}
	
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	redraw();

	controlPoints.resize(4);

	myColor =  QColor(144,238,144, 120);
}
//! [0]

VPLink::~VPLink()
{
	if (myError)
		QMessageBox::critical(0,
		"Connection error",
		graphSingleton->error().c_str(),
		QMessageBox::Ok, QMessageBox::NoButton);
	
	/* remove from graphe */
	graphSingleton->del_flow(myId);

	/* remove from scene */
	if (myStart)
		myStart->delLink(this);
	if (myEnd)
		myEnd->delLink(this);

	controlPoints.clear();

	qDebug() << "call VPLink::VPLink";
}

//! [1]
QRectF VPLink::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
//! [1]

//! [2]
QPainterPath VPLink::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
	//path.addPolygon(arrowHead);
    return path;
}
//! [2]

//! [3]
// mapFromItem函数返回mItem在Scene中的位置
// 鼠标移动Item的时候改变Item在Scene中的位置
void VPLink::redraw()
{
	prepareGeometryChange();

	// pos函数返回的是scene坐标系中的坐标
	QPointF startP = myStart->parent()->scenePosition();
	QPointF endP = myEnd->parent()->scenePosition();
	
	startP += myStart->offset();
	endP += myEnd->offset();

    QLineF line( startP, endP );
    setLine(line);
	
	update();
}
//! [3]

void VPLink::remove(VPConnector *c)
{
	if (myStart == c)
		myStart = 0;
	else if (myEnd == c)
		myEnd = 0;
}


//! [4]
void VPLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    //QPen myPen = pen();
    //myPen.setColor(myColor);
    //painter->setPen(myPen);
    //painter->setBrush(myColor);

	redraw();

	//////////////////////////////////////////////////////////////////////////
	// painter->drawLine( line() );
	if (!computeBezierControlPoints())
		return;

	// Construct the path
	QPainterPath path;
	path.moveTo(controlPoints.at(0));

	int i=1;
	while (i + 2 < controlPoints.size()) 
	{
		path.cubicTo(controlPoints.at(i), controlPoints.at(i+1), controlPoints.at(i+2));
		i += 3;
	}

	painter->setRenderHint(QPainter::Antialiasing);
	//painter->strokePath(path, QPen(Qt::black, 2, Qt::SolidLine));
	painter->strokePath(path, QPen( gLinkColor, VPLink::lineWidth, Qt::SolidLine) );

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
    if (isSelected()) 
	{
        //painter->setPen(QPen(myColor, 1, Qt::DashLine));
		painter->setPen(QPen(Qt::red, 2, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}
//! [7]

//! [8]
int VPLink::computeBezierControlPoints()
{
	controlPoints[0] = line().p1();
	controlPoints[3] = line().p2();

	float dist = abs( controlPoints[3].x() - controlPoints[0].x() ) / 2.0f;

	controlPoints[1] = controlPoints[0];
	controlPoints[1].rx() += dist;

	controlPoints[2] = controlPoints[3];
	controlPoints[2].rx() -= dist;

	return 1;
}
//! [8]

//! [9]
// point1 & point2 are both scene's coordinate
void VPLink::setRect(const QPointF& point1, const QPointF& point2)
{
	setLine ( QLineF(point1, point2) );

	controlPoints[0] = point1;
	controlPoints[3] = point2;
}
//! [9]