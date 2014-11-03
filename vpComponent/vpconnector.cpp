#include "vpconnector.h"

#include <qpainter>

#include <qDebug>

#include "vplink.h"
#include "vpabstractnode.h"

//	位于Node的局部坐标系。offset表示Connector和坐标系原点的偏移量~
VPConnector::VPConnector( const QRectF& _area,
						  const QPointF& _offset,
						  bool _input, int _number,
						  VPAbstractNode* _node )
	: myInput(_input)
	, myNumber(_number)
	, myArea(_area)
	, myOffset(_offset)
{
	node = _node;
}

VPConnector::~VPConnector()
{
	VPLink* arrow;

	LinkList::const_iterator i;
	for ( i = links.begin(); i != links.end(); i++)
	{
		arrow = ((VPLink*)(*i));
		arrow->remove(this);
		delete arrow;
	}
	links.clear();

	qDebug() << "call VPConnector::~VPConnector";
}

const QRectF& VPConnector::area() const 
{
	return myArea;
}

const QPointF& VPConnector::offset() const 
{
	return myOffset;
}

void VPConnector::setOffset(const QPointF& off)
{
	myOffset = off;
	myArea = QRectF( myOffset.x()-myArea.width()/2.0, myOffset.y()-myArea.height()/2.0,
					 myArea.width(), myArea.height() );
}

bool VPConnector::input() const
{
	return myInput;
}

bool VPConnector::output() const
{
	return !myInput;
}

int	VPConnector::number() const
{
	return myNumber;
}

VPAbstractNode* VPConnector::parent()
{
	return node;
}

VPLink* VPConnector::getLink( unsigned index )
{
	LinkList::const_iterator i;
	for ( i = links.begin(); index > 0 && i != links.end(); index--, i++);
	return (*i);
}

unsigned VPConnector::getLinkNumber()
{
	return links.size(); 
}

void VPConnector::addLink( VPLink* flux )
{
	links.push_front( flux ); 
}

void VPConnector::delLink( VPLink* flux )
{
	LinkList::iterator i;
	for ( i = links.begin(); i != links.end(); i++)
		if (((VPLink*)(*i)) == flux )
		{
			links.erase(i);
			break;
		}
}

void VPConnector::redraw()
{
	LinkList::const_iterator i;
	for ( i = links.begin(); i != links.end(); i++)
		((VPLink*)(*i))->redraw();
}

void VPConnector::render(QPainter* painter)
{
	painter->drawEllipse(myArea);
	//painter->drawRect(myArea);
}
