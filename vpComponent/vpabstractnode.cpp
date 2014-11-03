#include <QtGui>

#include "vpconnector.h"
#include "../em/agraph_exec.hh"

#include "vpabstractnode.h"

QColor VPAbstractNode::HeadColor = QColor(102,255,172);		//LightGreen
QColor VPAbstractNode::BodyColor = QColor(227,228,232);		//LightGray
QColor VPAbstractNode::InSlotColor = QColor(0,0,255);		//Blue	
QColor VPAbstractNode::OutSlotColor = QColor(255,0,0);		//Red

int VPAbstractNode::SlotSize = 15;
//int VPAbstractNode::SlotSize = 8;

const int gNodeLeft = -100;
const int gNodeTop = -40;
const int gNodeWidth = 200;
const int gNodeHeight = 100;

QFont 	gSlotFont("Verdana", 12, QFont::Normal);

//////////////////////////////////////////////////////////////////////////
//	Node的尺寸影响因素：
//		inSlotNumber, outSlotNumber 
//		inSlotNameLength, outSlotNameLength, headNameLength

//////////////////////////////////////////////////////////////////////////
//	被VPProcessor的构造调用
//	下面所有的操作都是构造Processor所特有！
//////////////////////////////////////////////////////////////////////////
VPAbstractNode::VPAbstractNode( QMenu *contextMenu,
			   AProcess* proc,
			   unsigned int nbinput, unsigned int nboutput,
			   unsigned int id,
			   const QString& name )
			   : myInCount(nbinput), myOutCount(nboutput)
			   , myId(id)
			   , myProcess(proc)
{
	// 设置右键菜单！
	myContextMenu = contextMenu;

	// 设置名字！
	setText(name);

	// 设置字体！
	setFont( QFont("Verdana", 15, QFont::Normal) );

	// 设置默认的结点区域，在局部坐标系下！
	myRect.setRect(gNodeLeft, gNodeTop, gNodeWidth, gNodeHeight);
	
	// 调整结点区域/myRect！
	adjustRectByContent();

	// 装载端口！
	setupSlots();
}

//////////////////////////////////////////////////////////////////////////
//	被VPTerminal的构造函数调用
//	初始化列表中只是一些默认初始化！
//////////////////////////////////////////////////////////////////////////
VPAbstractNode::VPAbstractNode()
	: myInCount(0), myOutCount(0)
	, myId(-1)
	, myContextMenu(0)
	, myText("")
	, myProcess(0)
{}

VPAbstractNode::~VPAbstractNode()
{
	destroy();
}

void VPAbstractNode::setText(const QString &text)
{
	myText = text;
}

const QString& VPAbstractNode::text() const
{
	return myText;
}

const QFont& VPAbstractNode::font() const
{
	return myFont;
}

void VPAbstractNode::setFont(const QFont &f)
{
	myFont = f;
}

QString
	VPAbstractNode::getNodeTips(const QPointF& localPos)  
{
	SlotInfoType info = querySlot(localPos);

	int idx = info.second;

	if (-1==idx) 
		return text(); //wrong

	return QString( getProcess()->slot_type(info.first, idx) );
}

//	函数的参数mouseP是局部坐标系下
VPAbstractNode::SlotInfoType
	VPAbstractNode::querySlot(const QPointF& localPos)
{
	int retIdx(-1);
	SlotType retType(NONE);

	//QPointF localMousePos = mapFromScene(mouseP);

	for (int i(0); i<myInCount; i++)
	{
		if ( inputSlots[i]->area().contains(localPos) )
		{
			retIdx = i;
			retType = IN;
		}
	}

	for (int i(0); i<myOutCount; i++)
	{
		if (outputSlots[i]->area().contains(localPos) )
		{
			retIdx = i;
			retType = OUT;
		}
	}

	return SlotInfoType(retType, retIdx);
}

QPointF VPAbstractNode::slotOffset(int idx, SlotType sType)
{
	if ( sType==NONE
		|| (idx < 0)
		|| (idx > myInCount && sType==IN)
		|| (idx > myOutCount && sType==OUT) )
		return QPointF(0, 0);

	int headHeight = gNodeHeight/4;			//head height
	int right = myRect.right();
	int left = myRect.left();
	int top = myRect.top() + headHeight;		//body top
	int bottom = myRect.bottom();
	int wid = myRect.width();						
	int height = myRect.height() - headHeight;	//body height

	int deltaY(0);

	if (sType==IN)
	{
		// slots : left & right
		deltaY = height/(myInCount+1);
		
		// In slots are in the widget rect.
		//return QPointF( left+VPAbstractNode::SlotSize, top + deltaY*(idx+1) );

		// Out slots are out the widget rect. Need adjust the BoundingRect.
		return QPointF( left-VPAbstractNode::SlotSize/2, top + deltaY*(idx+1) );
	}

	//sType==OUT
	//idx = idx - myInCount;

	deltaY = height/(myOutCount+1);

	// Out slots are in the widget rect
	// return QPointF( right-VPAbstractNode::SlotSize, top + deltaY*(idx+1) );

	// Out slots are out the widget rect, need broaden the BoundingRect.
	return QPointF( right+VPAbstractNode::SlotSize/2, top + deltaY*(idx+1) );	
}

//////////////////////////////////////////////////////////////////////////
//	1，根据结点名字和端口名字（对于Processor）调整myRect的宽度
//	2，根据输入、输出端口的个数调整myRect的高度
//////////////////////////////////////////////////////////////////////////
//	Note: 
//		1, this function is called in VPAbstractNode constructor and
//			VPTerminal::setupProcess
//		2, this function may change the myRect, so it must be called after
//			VPAbstractNode::setupSlots
//////////////////////////////////////////////////////////////////////////
void VPAbstractNode::adjustRectByContent()
{
//!	[Adjust myRect width]
	QFontMetricsF headFontMetrics( font() ); 
	
	//!	[compute the head text width]
	qreal headTextWid = headFontMetrics.width( text() );
	//!	[compute the head text width]
	
	//!	[compute the slot text width]
	int slotSize = VPAbstractNode::SlotSize;
	QFontMetricsF slotFontMetrics(gSlotFont); 

	qreal inSlotMaxW = 0;
	for (int i(0); i<myInCount; i++)
	{
		qreal slotW = slotFontMetrics.width( QString(getProcess()->slot_name(true, i)) );
		if ( slotW > inSlotMaxW )
			inSlotMaxW = slotW;
	}

	int outSlotMaxW = 0;
	for (int i(0); i<myOutCount; i++)
	{
		qreal slotW = slotFontMetrics.width( QString(getProcess()->slot_name(false, i)) );
		
		if ( slotW > outSlotMaxW )
			outSlotMaxW = slotW;
	}
	//!	[compute the slot text width]
	
	qreal maxW = (inSlotMaxW+outSlotMaxW)>headTextWid ? (inSlotMaxW+outSlotMaxW) : headTextWid;
	
	if (myRect.width() < maxW)
		myRect.adjust(0,0, maxW-myRect.width(), 0);

//!	[Adjust myRect width]

//!	[Adjust myRect height]
	//According to slot size, adjust the node height 
	int maxCount = myInCount>myOutCount ? myInCount : myOutCount;
	int adjustHeight(0);
	if (maxCount-3>0)
	{
		int deltaY = myRect.height()*3/16;	//估计值
		//deltaY = 30;
		adjustHeight = (maxCount-3) * deltaY;

		myRect.adjust(0, 0, 0, adjustHeight);
	}
//!	[Adjust myRect height]
}

// mouseP必须处于item的局部坐标系
VPAbstractNode::ConnectorTypePtr 
	VPAbstractNode::getConnector(const QPointF& mouseP)
{
	SlotInfoType info = querySlot(mouseP);

	if (info.first==IN)
		return inputSlots[info.second];
	else if (info.first==OUT)
		return outputSlots[info.second];

	return 0;
}

// add by RYF 
// 11/28/2010 Administrator
VPAbstractNode::ConnectorTypePtr
	VPAbstractNode::queryConnector(int idx, SlotType sType)
{
	if (sType==IN)
		return inputSlots[idx];
	else if (sType==OUT)
		return outputSlots[idx];

	return 0;
}

void 
VPAbstractNode::querySlotsState(bool bBuf[], bool isIn)
{
	if (isIn)
		for (int i(0); i<myInCount; i++)
			bBuf[i] = static_cast<bool>( inputSlots[i]->getLinkNumber() );
	else
		for (int o(0); o<myOutCount; o++)
			bBuf[o] = static_cast<bool>( outputSlots[o]->getLinkNumber() );
}
	
void VPAbstractNode::setupSlots()
{
	int slotSize = VPAbstractNode::SlotSize;

	inputSlots.clear();
	inputSlots.resize(myInCount);

	for (int i(0); i<myInCount; i++)
	{
		QPointF tRectCenter = slotOffset(i, IN);
		QRectF tRect(tRectCenter.x()-slotSize/2, tRectCenter.y()-slotSize/2, slotSize, slotSize);

		inputSlots[i] = new ConnectorType(tRect, tRectCenter, true, i, this);
	}

	outputSlots.clear();
	outputSlots.resize(myOutCount);

	for (int i(0); i<myOutCount; i++)
	{
		QPointF tRectCenter = slotOffset(i, OUT);
		QRectF tRect(tRectCenter.x()-slotSize/2, tRectCenter.y()-slotSize/2, slotSize, slotSize);

		outputSlots[i] = new ConnectorType(tRect, tRectCenter, false, i, this);
	}
}

void VPAbstractNode::renderSlot(QPainter* painter)
{
	painter->setPen(Qt::NoPen);
	painter->setBrush( VPAbstractNode::InSlotColor );
	foreach (VPConnector* slot, inputSlots)
		slot->render(painter);

	painter->setBrush( VPAbstractNode::OutSlotColor );
	for (int i(0); i<myOutCount; i++)
		outputSlots[i]->render(painter);

}

void VPAbstractNode::renderSlotName(QPainter* painter)
{
	int slotSize = VPAbstractNode::SlotSize;
	QFontMetricsF fontMetrics(gSlotFont); 

	painter->setPen(Qt::black);
	painter->setFont(gSlotFont);
	for (int i(0); i<myInCount; i++)
	{
		// get slot offset
		QPointF namePos = inputSlots[i]->offset();
		namePos.rx() += slotSize;
		namePos.ry() += slotSize*0.5;

		painter->drawText( namePos, QString( getProcess()->slot_name(true, i) ) );
	}

	for (int i(0); i<myOutCount; i++)
	{
		const char* name = getProcess()->slot_name(false, i);

		QPointF namePos = outputSlots[i]->offset();
		namePos.rx() -= (slotSize*1.5 + fontMetrics.width(name));
		namePos.ry() += 0.5*slotSize;

		painter->drawText( namePos, QString(name) );
	}
}

//////////////////////////////////////////////////////////////////////////
//	释放端口内存
//////////////////////////////////////////////////////////////////////////
void VPAbstractNode::destroy()
{
	// 释放输入端口数组
	for (unsigned i = 0; i < myInCount; i++) {
		delete inputSlots[i];
		inputSlots[i] = 0;
	}
	inputSlots.clear();
	
	// 释放输出端口数组
	for (unsigned i = 0; i < myOutCount; i++) {
		delete outputSlots[i];
		outputSlots[i] = 0;
	}
	outputSlots.clear();
		
	// 
	setProcess(0);		//Do not delete the myProcess;

	// 
	qDebug() << "call VPAbstractNode::destroy()";
}
//////////////////////////////////////////////////////////////////////////
void VPAbstractNode::setHeadColor(const QColor &color)
{
	HeadColor = color;
}

void VPAbstractNode::setBodyColor(const QColor &color)
{
	BodyColor = color;
}

void VPAbstractNode::setInSlotColor(const QColor &color)
{
	InSlotColor = color;
}

void VPAbstractNode::setOutSlotColor(const QColor &color)
{
	OutSlotColor = color;
}
