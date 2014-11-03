#include "vpfactory.h"


VPFactory::VPFactory()
{
	patchDB.clear();
	flowDB.clear();
}

VPFactory::~VPFactory()
{
	patchDB.clear();
	flowDB.clear();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

VPFactory::VectorType
	VPFactory::getPatchKeys(const Category type) const
{
	// if catg == PROCESSOR	| PROVIDER	|  CONSUMER

	VectorType	retVector;

	for (PatchDBConstIter it = patchDB.begin();
		it != patchDB.end();
		it++)
	{
		if (type==it->catg)
			retVector.push_back(it->processName);
	}

	return retVector;
}

VPFactory::VectorType
	VPFactory::getFlowKeys() const
{
	VectorType	retVector;

	for ( FlowDBConstIter it = flowDB.begin();
		  it != flowDB.end();
		  it ++ )
	{
		retVector.push_back( it->flowName );
	}

	return retVector;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
VPFactory::ProcessCreator 
	VPFactory::queryProcessor(const KeyType& procname) const
{
	// find_if
	PatchDBConstIter pos = 
		std::find_if(patchDB.begin(), patchDB.end(), ProcessFinder(procname));

	if (pos==patchDB.end())
		return NULL;

	return pos->pCreator;
}

VPFactory::FlowCreator
	VPFactory::qureyFlow(const KeyType& flowname) const 
{
	// find_if
	FlowDBConstIter pos = 
		std::find_if(flowDB.begin(), flowDB.end(), FlowFinder(flowname));

	if (pos==flowDB.end())
		return NULL;

	return pos->fCreator;	
}

bool
	//VPFactory::queryTerminal(const KeyType& patchname, TerminalCreator& tcreator, 
	//						 KeyType& procename, ProcessCreator& pcreator) const
	VPFactory::queryTerminal(const KeyType& procename, TerminalCreator& tcreator, 
							 KeyType& patchname, ProcessCreator& pcreator) const
{
	// find_if
	PatchDBConstIter pos = 
		std::find_if(patchDB.begin(), patchDB.end(), ProcessFinder(procename));

	if ( pos==patchDB.end() )
		return false;

	tcreator = pos->tCreator;
	pcreator = pos->pCreator;
	patchname = pos->patchName;

	return true;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void
	VPFactory::registerPatch(const KeyType& processname, 
							 ProcessCreator pcreator, 
							 const KeyType& patchname, 
							 TerminalCreator tcreator, 
							 Category catg, 
							 const KeyType& groupname)
{
	PatchInfo data;

	if (PROCESSOR==catg)
	{
		data.patchName = "";
		data.tCreator = NULL;
	}
	else
	{
		data.patchName = patchname;
		data.tCreator = tcreator;
	}

	data.catg = catg;
	data.groupName = groupname;
	data.pCreator = pcreator;
	data.processName = processname;

	patchDB.push_back( data );
}

void	
	VPFactory::registerFlow(const KeyType& flowname, FlowCreator creator)
{
	FlowInfo data;

	data.flowName = flowname;
	data.fCreator = creator;

	flowDB.push_back( data );
}

void
	VPFactory::unRegisterPatchByProcessName(const KeyType& procname)
{
	// find_if
	PatchDBIter pos = 
		std::find_if(patchDB.begin(), patchDB.end(), ProcessFinder(procname));

	if (pos==patchDB.end())
		return;

	patchDB.erase(pos);
}

void
	VPFactory::unRegisterFlow(const KeyType& flowname)
{
	// find_if

	FlowDBIter pos = 
		std::find_if(flowDB.begin(), flowDB.end(), FlowFinder(flowname));

	if (pos==flowDB.end())
		return;

	flowDB.erase(pos);
}

//////////////////////////////////////////////////////////////////////////
//	processName 为主键
//		改变getProviderKeys，getConsumerKeys返回所有的processName
//		改变queryTerminal(processName,...)
//////////////////////////////////////////////////////////////////////////