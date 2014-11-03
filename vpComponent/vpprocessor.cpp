#include <QtGui>
#include "vplink.h"
#include "vpprocessor.h"
#include "vpconnector.h"

//Color Schemes 2 :
QColor	gNodeHeadColor(102,255,172);		//LightGreen
QColor	gNodeBodyColor(227,228,232);		//LightGray
QColor	gInputSlotColor(0,0,255);			//Blue	
QColor	gOutputSlotColor(255,0,0);			//Red

//////////////////////////////////////////////////////////////////////////
VPProcessor::VPProcessor( QMenu *contextMenu,
			    unsigned int nbinput, unsigned int nboutput,
				AProcess* process,
				unsigned int id,
				const QString& name,
				QGraphicsItem *parent, QGraphicsScene *scene )
	: QGraphicsRectItem(parent, scene)
	, VPAbstractNode(contextMenu, process, nbinput, nboutput, id, name)
{
	setRect(myRect);			//must

	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);

	setHeadColor(gNodeHeadColor);
	//setAcceptsHoverEvents(true);
}

VPProcessor::~VPProcessor()
{
}

QPointF VPProcessor::scenePosition() const
{
	return QGraphicsItem::pos();
}

VPConnector* VPProcessor::getConnector(const QPointF& sPos)
{
	// dispatch
	return VPAbstractNode::getConnector( QGraphicsItem::mapFromScene(sPos) );
}

QString
	VPProcessor::getNodeTips(const QPointF& sPos)
{
	// dispatch
	return VPAbstractNode::getNodeTips( QGraphicsItem::mapFromScene(sPos) );	
}

//////////////////////////////////////////////////////////////////////////
// 端口脚点在结点之外，需要调整区域的包围盒
//////////////////////////////////////////////////////////////////////////
QRectF VPProcessor::boundingRect() const
{
	QRectF retRect = rect();	
	int slotSize = VPAbstractNode::SlotSize;

	return retRect.adjusted(-slotSize, 0, slotSize, 0 );
}

//////////////////////////////////////////////////////////////////////////
// 端口脚点在结点之外，需要调整区域的包围盒
//////////////////////////////////////////////////////////////////////////
QPainterPath VPProcessor::shape() const
{
	QPainterPath path = QGraphicsRectItem::shape();
	
	foreach (ConnectorTypePtr slot, inputSlots)
		path.addRect( slot->area() );

	foreach (ConnectorTypePtr slot, outputSlots)
		path.addRect( slot->area() );

	return path;
}

void VPProcessor::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	scene()->clearSelection();
	setSelected(true);
	myContextMenu->exec(event->screenPos());
}

// 该函数在ccw,cw的时候并没有调用
QVariant VPProcessor::itemChange(GraphicsItemChange change,
							const QVariant &value)
{
	if (change == QGraphicsItem::ItemPositionChange) 
	{
		//redraw output connector's offset
		for (int i(0); i<myInCount; i++)
			inputSlots[i]->setOffset( slotOffset(i, IN) );
		
		//redraw input connector's offset
		for (int i(0); i<myOutCount; i++)
			outputSlots[i]->setOffset( slotOffset(i, OUT) );

		// redraw input connector's links~
		foreach (ConnectorTypePtr slot, outputSlots)
			slot->redraw();
	
		// redraw output connector's links~
		foreach (ConnectorTypePtr slot, inputSlots)
			slot->redraw();		
	}

	return value;
}

void VPProcessor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
				   QWidget *widget)
{
	QPen pen(Qt::darkBlue);
	int slotSize = VPAbstractNode::SlotSize;

	if ( isSelected() ) 
	{
		pen.setStyle(Qt::DotLine);
		pen.setWidth(2);
	}

	// antialiasing
	painter->setRenderHint(QPainter::Antialiasing);

	//-----------------First time: draw body-------------------
	painter->setPen(pen);
	painter->setBrush( VPAbstractNode::BodyColor );
	painter->drawRoundRect( rect(), roundness( rect().width()), roundness( rect().height()));
	
	//-----------------Second time: draw head area-------------
	float Right = rect().right();
	float Top = rect().top();
	float Left = rect().left();
	float Bottom = rect().top() + VPProcessor::HeadHeight;
	float Radius = VPProcessor::Diameter;

	QPainterPath upRoundRectPath;
	upRoundRectPath.moveTo(Right, Top + Radius/2.0);
	upRoundRectPath.arcTo(Right - Radius, Top, Radius, Radius, 0.0, 90.0);
	upRoundRectPath.lineTo(Left + Radius/2.0, Top);
	upRoundRectPath.arcTo(Left, Top, Radius, Radius, 90.0, 90.0);
	upRoundRectPath.lineTo(Left, Bottom);
	upRoundRectPath.lineTo(Right, Bottom);
	upRoundRectPath.closeSubpath();

	painter->setPen(Qt::NoPen);
	painter->fillPath(upRoundRectPath, gNodeHeadColor/*VPAbstractNode::HeadColor*/);

	//-----------------Third time: draw head text-------------------
	painter->setPen(Qt::black);
	painter->setFont( font() );
	painter->drawText( rect(), Qt::AlignHorizontal_Mask & Qt::AlignLeft, myText);
	
	//-----------------Fourth time: draw slots-------------------
	renderSlot(painter);

	//-----------------Sixth time: draw slot name-------------------
	renderSlotName(painter);
}

void VPProcessor::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() != Qt::LeftButton)
		return;
	
	//////////////////////////////////////////////////////////////////////////
	// change the node name:
	//QString text = QInputDialog::getText(event->widget(),
	//				tr("Edit Text"), tr("Enter new text:"),
	//QLineEdit::Normal, myText);
	//if (!text.isEmpty())
	//	setText(text);
}

int VPProcessor::roundness(double size) const
{
	return 100 * Diameter / int(size);
}
