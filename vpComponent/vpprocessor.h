#ifndef VPNODE_H
#define VPNODE_H

#include <QApplication>
#include <QGraphicsPixmapItem>

#include "vpabstractnode.h"

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QRectF;
QT_END_NAMESPACE

//! [0]
class VPProcessor : public QGraphicsRectItem, public VPAbstractNode
{
	Q_DECLARE_TR_FUNCTIONS(VPProcessor)

public:
	//////////////////////////////////////////////////////////////////////////
	// typedef and enum
	//////////////////////////////////////////////////////////////////////////
	enum { Type = UserType + 11 };
	enum { Diameter = 20, HeadHeight = 25 };

	//////////////////////////////////////////////////////////////////////////
	// ctor and dtor
	//////////////////////////////////////////////////////////////////////////
	VPProcessor( QMenu *contextMenu,
			unsigned int nbinput, unsigned int nboutput,
			AProcess* process,
			unsigned int id,			//node id
			const QString& name,		//node name
			QGraphicsItem *parent = 0, QGraphicsScene *scene = 0 );

	~VPProcessor();

	//////////////////////////////////////////////////////////////////////////
	// reimplementation virtual functions
	//////////////////////////////////////////////////////////////////////////
 	QRectF	boundingRect() const;
 	QPainterPath shape() const;
	int		type() const	{ return Type;}

	//////////////////////////////////////////////////////////////////////////
	// reimplementation VPAbstractNode's virtual functions
	//////////////////////////////////////////////////////////////////////////
	VPConnector*	getConnector(const QPointF& sPos);
	QString			getNodeTips(const QPointF& sPos);
	QPointF	 scenePosition() const;

protected:
	//////////////////////////////////////////////////////////////////////////
	// reimplementation QGraphicsRectItem's virtual functions
	//////////////////////////////////////////////////////////////////////////
	
	void	mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

	void	contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
					QWidget *widget = 0);


private:
	//////////////////////////////////////////////////////////////////////////
	// Ë½ÓÐº¯Êý
	//////////////////////////////////////////////////////////////////////////
	int	roundness(double size) const;
};

#endif
