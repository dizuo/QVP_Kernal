#ifndef VPABSTRACTNODE_H
#define VPABSTRACTNODE_H

//	8/6/2010 
//	Created by RYF
//	Email : dizuo@126.com
//	QQ:546952720

#include <QList>
#include <QFont>
#include <QMenu>
#include <QVector>
#include <QRectF>
#include <QColor>
#include <QString>

#include <utility>

class VPLink;
class VPConnector;
class AProcess;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 下面三个函数在VPTerminal和VPProcessor中分别被重写
//	scenePosition 
//	getConnector
//	getNodeTips
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class VPAbstractNode
{
public:
	//////////////////////////////////////////////////////////////////////////
	// 类型和枚举定义
	//////////////////////////////////////////////////////////////////////////
	typedef	VPConnector		ConnectorType;
	typedef	ConnectorType*	ConnectorTypePtr;

	enum SlotType
	{
		OUT = 0, 
		IN = 1, 
		NONE = 2
	};

	typedef std::pair<SlotType, int>	SlotInfoType;

	//////////////////////////////////////////////////////////////////////////
	// 构造函数、析构函数
	//////////////////////////////////////////////////////////////////////////
	VPAbstractNode();

	VPAbstractNode(QMenu *contextMenu, AProcess* proc,
		unsigned int nbinput, unsigned int nboutput,
		unsigned int id,			//node id
		const QString& name);		//node name

	virtual ~VPAbstractNode();

	//////////////////////////////////////////////////////////////////////////
	// 一般功能函数
	//////////////////////////////////////////////////////////////////////////
	
	// text & color
	void	setText(const QString &text);
	const QString& text() const;

	// font
	const QFont&	font() const;
	void	setFont(const QFont &f);

	void setId(int i)	{ myId = i; }
	unsigned int		getId() const { return myId; }

	AProcess* getProcess() const { return myProcess; }
	void setProcess(AProcess* proc) { myProcess = proc; }
	
	ConnectorTypePtr			queryConnector(int idx, SlotType sType);
	void	querySlotsState(bool bBuf[], bool isIn=true);

	//////////////////////////////////////////////////////////////////////////
	// 虚函数申明
	//////////////////////////////////////////////////////////////////////////
	// abstract virtual interfaces
	virtual QPointF			scenePosition() const = 0;

	// virtual functions
	virtual QString				getNodeTips(const QPointF& localPos);
	virtual ConnectorTypePtr	getConnector(const QPointF& localPos);


	//////////////////////////////////////////////////////////////////////////
	// 静态函数申明
	//////////////////////////////////////////////////////////////////////////
	static void	setHeadColor(const QColor &color);
	static void	setBodyColor(const QColor &color);
	static void	setInSlotColor(const QColor &color);
	static void	setOutSlotColor(const QColor &color);

protected:
	//////////////////////////////////////////////////////////////////////////
	// 保护成员函数
	//////////////////////////////////////////////////////////////////////////
	void	destroy();

	// slot
	void	setupSlots();
	void	setInSlotCount(int iC) { myInCount = iC; }
	void	setOutSlotCount(int oC) { myOutCount = oC; }

	void	renderSlot(QPainter* painter);
	void	renderSlotName(QPainter* painter);

	void	adjustRectByContent();

	QPointF			slotOffset(int idx, SlotType sType);
	SlotInfoType 	querySlot(const QPointF& mouseP);

	//////////////////////////////////////////////////////////////////////////
	// 成员数据定义
	//////////////////////////////////////////////////////////////////////////

	QMenu*			myContextMenu;

	unsigned int	myId;
	AProcess*		myProcess;

	QString			myText;
	QRectF			myRect;
	QFont			myFont;

	int							myInCount;
	int							myOutCount;
	QVector<ConnectorTypePtr>	inputSlots;
	QVector<ConnectorTypePtr>	outputSlots;

	static QColor	HeadColor;
	static QColor	BodyColor;
	static QColor	InSlotColor;
	static QColor	OutSlotColor;

	static int SlotSize;
};

//////////////////////////////////////////////////////////////////////////
//	8/20/2010 RYF
//	增加VPAbstractNode构造函数的参数：AGraphExec*

//	12/1/2010 RYF
//		去掉VPAbstractNode中参数：AGraphExec*
//		VPabstractNode不在管理AProcess的生命周期
//////////////////////////////////////////////////////////////////////////

#endif