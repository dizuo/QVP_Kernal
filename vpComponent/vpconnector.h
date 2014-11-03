#ifndef VPCONNECTOR_H
#define VPCONNECTOR_H

#include <qlist>
#include <qrectf>

class VPAbstractNode;
class VPLink;
class QPainter;

class VPConnector
{
	typedef QList<VPLink*> LinkList;
public:
	VPConnector( const QRectF& _area, const QPointF& _offset,
				bool input, int number,
				VPAbstractNode* box );
	~VPConnector();

	bool input() const;
	bool output() const;

	int		number() const;
	VPAbstractNode* parent();

	VPLink*	getLink( unsigned index );
	void	addLink( VPLink* arrow );
	void	delLink( VPLink* arrow );
	void	redraw();
	
	void	setOffset(const QPointF& off);

	void	render(QPainter* painter);

	unsigned getLinkNumber();
	
	const QRectF& area() const;
	const QPointF& offset() const;
private:
	VPAbstractNode*		node;

	int			myNumber;
	bool		myInput;
	QRectF		myArea;
	QPointF		myOffset;	

	LinkList	links;
};

// 
#endif