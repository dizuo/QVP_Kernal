#include <QtGui>

#include "vpterminal.h"
#include "vpconnector.h"

#include "../em/agraph_exec.hh"

//////////////////////////////////////////////////////////////////////////
//	VPConsumer
//	构造函数增加一个proxyWidget参数，设置Provider的Rect
//////////////////////////////////////////////////////////////////////////

VPTerminal::VPTerminal( QGraphicsItem *parent, Qt::WindowFlags wFlags)
		   : QGraphicsProxyWidget(parent, wFlags)
		   , VPAbstractNode()
		   , _type(PROVIDER)
{
	setProcess(0);
	setInSlotCount(0);
	setOutSlotCount(0);
	setText("Ternimal");

	setHeadColor(QColor(65,105,225));
	VPAbstractNode::setFont( QFont("Verdana", 15, QFont::Normal) );

	setAttribute(Qt::WA_DeleteOnClose);		
	//当调用closeEvent函数以后，会发消息删除代理窗口，进而调用VPAbstractNode的析构函数
	//所以在closeEvent中没有必要destroy
}

VPTerminal::~VPTerminal()
{
}

// reimplementation virtual functions:
QRectF	VPTerminal::boundingRect() const
{
	int slotSize = VPAbstractNode::SlotSize;

	if ( getType()==PROVIDER )
		return QGraphicsProxyWidget::boundingRect().adjusted(0, 0, slotSize, 0);

	return QGraphicsProxyWidget::boundingRect().adjusted(-slotSize, 0, 0, 0);
}	

QPainterPath VPTerminal::shape() const
{
	QPainterPath path = QGraphicsProxyWidget::shape();

	foreach (VPConnector* slot, inputSlots)
		path.addRect( slot->area() );

	return path;
}


// reimplementation VPAbstractNode's virtual functions:
VPConnector* VPTerminal::getConnector(const QPointF& sPos)
{
	return VPAbstractNode::getConnector( QGraphicsItem::mapFromScene(sPos) );	
}

QString
	VPTerminal::getNodeTips(const QPointF& sPos)
{
	return VPAbstractNode::getNodeTips( QGraphicsItem::mapFromScene(sPos) );	
}

QPointF VPTerminal::scenePosition() const
{
	// return the scene position
	return QGraphicsItem::pos();
}

// 终端中不显示端口的名字
void VPTerminal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w)
{
	// 更新myRect和端口位置
	updateSlots();			

	painter->setRenderHint(QPainter::Antialiasing);

	//////////////////////////////////////////////////////////////////////////
	// draw body color
	painter->setPen(Qt::NoPen);
	painter->setBrush( QColor(167,192,220) );
	painter->drawRoundRect( myRect, roundness( myRect.width()), roundness( myRect.height()));

	//////////////////////////////////////////////////////////////////////////
	// draw proxyed widget
	QGraphicsProxyWidget::paint(painter, option, w);

	//////////////////////////////////////////////////////////////////////////
	// draw head area
	QRectF headArea( myRect.left(), myRect.top(), 
					 myRect.width(), WindowHeadWidth+1);
	painter->setPen( Qt::NoPen );

	float Right = headArea.right();
	float Top = headArea.top();
	float Left = headArea.left();
	float Bottom = headArea.bottom();
	float Radius = 20;

	QPainterPath upRoundRectPath;
	upRoundRectPath.moveTo(Right, Top + Radius/2.0);
	upRoundRectPath.arcTo(Right - Radius, Top, Radius, Radius, 0.0, 90.0);
	upRoundRectPath.lineTo(Left + Radius/2.0, Top);
	upRoundRectPath.arcTo(Left, Top, Radius, Radius, 90.0, 90.0);
	upRoundRectPath.lineTo(Left, Bottom);
	upRoundRectPath.lineTo(Right, Bottom);
	upRoundRectPath.closeSubpath();

	painter->setPen(Qt::NoPen);
	painter->fillPath(upRoundRectPath, QColor(167,192,220));

	//////////////////////////////////////////////////////////////////////////
	// draw head text
	painter->setPen(Qt::black);
	painter->setFont( VPAbstractNode::font() );
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawText( myRect, Qt::AlignHorizontal_Mask & Qt::AlignLeft, text());

	//////////////////////////////////////////////////////////////////////////
	// draw slots
	renderSlot(painter);
}

void VPTerminal::closeEvent( QCloseEvent *e )
{
	e->ignore();
}

//////////////////////////////////////////////////////////////////////////
//	不同于处理器（Processor）终端（Terminal）的尺寸可以改变
//	当用户改变终端结点的宽高以后，需要更新myRect和端口位置
//////////////////////////////////////////////////////////////////////////
void VPTerminal::updateSlots()
{
	//代理窗口尺寸改变以后，更新脚点的位置
	myRect = widget()->rect().adjusted(-WindowFrameWidth, -(WindowFrameWidth+WindowHeadWidth), 
								/*VPAbstractNode::SlotSize + */WindowFrameWidth, WindowFrameWidth);


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

void VPTerminal::setupProcess(AProcess* proc, int id)
{
	if (!proc || getProcess() 
		|| -1!=getId() || -1==id)
		return;

	// 设定终端内部逻辑进程
	setProcess(proc);

	// 设定终端的ID
	setId(id);

	// 设定终端的名字
	setText( QString(getProcess()->name().c_str()) );	

	// 设定终端的输入端口个数
	setInSlotCount( getProcess()->nb_inputs() );

	// 设定终端的输出端口个数
	setOutSlotCount( getProcess()->nb_outputs() );		

	// 调整终端区域myRect的宽度和高度
	adjustRectByContent();

	// 装载终端的输入和输出端口
	setupSlots();

	// 设置代理窗口的区域
	QGraphicsProxyWidget::setGeometry(myRect);
}

void VPTerminal::setupProxyWidget(QWidget* proxyW)
{
	proxyW->setWindowTitle( text() );
	setWidget(proxyW);	
	myRect = proxyW->rect().adjusted(-WindowFrameWidth, -(WindowFrameWidth+WindowHeadWidth), 
				/*VPAbstractNode::SlotSize + */WindowFrameWidth, WindowFrameWidth);	
}

//////////////////////////////////////////////////////////////////////////
// 计算圆角率
//////////////////////////////////////////////////////////////////////////
int VPTerminal::roundness(double size) const
{
	return 100 * Diameter / int(size);
}

//////////////////////////////////////////////////////////////////////////
// 11/6/2010 RYF
//		- setupProxyWidget, updateSlots 中myRect rightAdjust过大
//////////////////////////////////////////////////////////////////////////
