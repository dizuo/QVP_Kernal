#ifndef VP_FACTORY_H
#define VP_FACTORY_H

#include <iostream>
#include <algorithm>
#include <utility>
#include <string>
#include <sstream>

#include "vpterminal.h"
#include "..\factory\aflow.hh"
#include "..\factory\aprocess.hh"

using namespace std;

//////////////////////////////////////////////////////////////////////////
// 12/1/2010 RYF
//////////////////////////////////////////////////////////////////////////

// 宏定义
//////////////////////////////////////////////////////////////////////////
#ifndef NAME_TO_TYPE
	#define NAME_TO_TYPE(procClassName) #procClassName
#endif 

#ifndef PLUGIN_API
	#define PLUGIN_API __declspec(dllexport)
#endif
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//Category | Type | PatchName | PatchCreator | ProcessName | ProcessCreator
//////////////////////////////////////////////////////////////////////////
//	ProcessName: an unique index key
//	ProcessCreator: cannot be NULL
//	PatchName: may be empty
//	PatchCreator: may be NULL
//	Type: not unique
//	Category: not unique
//////////////////////////////////////////////////////////////////////////

class VPFactory
{
public:
	/////////////////////////////////////////////////////////////
	// type definition
	/////////////////////////////////////////////////////////////
	typedef std::string		KeyType;
	

	/////////////////////////////////////////////////////////////
	// creator definition
	/////////////////////////////////////////////////////////////
	typedef AProcess* (*ProcessCreator)(void);
	typedef VPTerminal* (*TerminalCreator)(void);
	typedef AFlow* (*FlowCreator)(void);

	/////////////////////////////////////////////////////////////
	// category definition
	/////////////////////////////////////////////////////////////
	enum Category {
		PROVIDER = 0,
		PROCESSOR = 1,
		CONSUMER = 2
	};

	/////////////////////////////////////////////////////////////
	//Category | GroupName  | PatchName | PatchCreator | ProcessName | ProcessCreator
	/////////////////////////////////////////////////////////////
	struct PatchInfo
	{
		Category	catg;
		KeyType		groupName;
		KeyType		patchName;
		KeyType		processName;

		TerminalCreator	tCreator;
		ProcessCreator	pCreator;

		PatchInfo()
			: tCreator(0), patchName("")
			, pCreator(0), processName("")
			, catg(PROCESSOR), groupName("")
		{}

		PatchInfo(Category c, const KeyType& groupname, 
			const KeyType& patchname, TerminalCreator pathcreator,
			const KeyType& processname, ProcessCreator proccreator)
			: catg(c), groupName(groupname)
			, patchName(patchname), tCreator(pathcreator)
			, processName(processname), pCreator(proccreator)
		{}
	};

	struct PatchFinder
	{
		PatchFinder(const KeyType& patchname)
			: patchName(patchname)
		{}

		bool operator() (const PatchInfo& data)
		{
			return data.patchName==patchName;
		}
	
	private:
		const KeyType& patchName;
	};	

	struct ProcessFinder
	{
		ProcessFinder(const KeyType& procname)
			: processName(procname)
		{
		}

		bool operator()(const PatchInfo& data)
		{
			return data.processName==processName;	
		}

	private:
		const KeyType& processName;
	};

	struct CategoryFinder
	{
		CategoryFinder(const Category& c)
			: catg(c)
		{
		}

		bool operator()(const PatchInfo& data)
		{
			return data.catg==catg;
		}

	private:
		const Category& catg;
	};

	/////////////////////////////////////////////////////////////
	// Flow 
	/////////////////////////////////////////////////////////////
	struct FlowInfo
	{
		KeyType flowName;
		FlowCreator	fCreator;

		FlowInfo()
			: flowName(""), fCreator(0)
		{}

		FlowInfo(const KeyType& flowname, FlowCreator flowcreator)
			: flowName(flowname), fCreator(flowcreator)
		{}
	};

	struct FlowFinder
	{
		FlowFinder(const KeyType& flowname)
			: flowName(flowname)
		{
		}

		bool operator()(const FlowInfo& data)
		{
			return data.flowName==flowName;	
		}

	private:
		const KeyType& flowName;
	};

	/////////////////////////////////////////////////////////////
	// Factory container type 
	/////////////////////////////////////////////////////////////
	typedef std::vector< KeyType > VectorType;
	typedef VectorType::iterator VectorIter;

	typedef std::vector<PatchInfo>	PatchDBType;
	typedef PatchDBType::iterator	PatchDBIter;
	typedef PatchDBType::const_iterator		PatchDBConstIter;

	typedef std::vector<FlowInfo>	FlowDBType;
	typedef FlowDBType::iterator	FlowDBIter;
	typedef FlowDBType::const_iterator		FlowDBConstIter;

	//////////////////////////////////////////////////////////////////////////
	// VPDB interfaces.
	//////////////////////////////////////////////////////////////////////////
	
	VPFactory();
	~VPFactory();

	// for PatchTreeView
	VectorType	getPatchKeys(const Category type) const;

	// for FlowTreeView
	VectorType  getFlowKeys() const;

	// for VPScene::addPatch:	processor
	ProcessCreator	queryProcessor(const KeyType& procname) const;

	FlowCreator		qureyFlow(const KeyType& flowname) const;

	// for VPScene::addPatch:	consumer/provider
	bool	queryTerminal(const KeyType& processname, TerminalCreator& tcreator, 
						  KeyType& patchname, ProcessCreator& pcreator) const;

	//Category | GroupName  | PatchName | PatchCreator | ProcessName | ProcessCreator
	void	registerPatch(const KeyType& processname, ProcessCreator pcreator,
						  const KeyType& patchname, TerminalCreator tcreator=NULL,
						  Category catg=PROCESSOR, const KeyType& groupname="Default");

	void	registerFlow(const KeyType& flowname, FlowCreator creator);


	void	unRegisterPatchByProcessName(const KeyType& procname);
	void	unRegisterFlow(const KeyType& flowname);

private:
	
	PatchDBType	patchDB;
	FlowDBType	flowDB;	
	
};	// VPFactory

//////////////////////////////////////////////////////////////////////////
// 12/1/2010 RYF
//////////////////////////////////////////////////////////////////////////

// patchType	processName		processCreator
// [patchType] = processMap
// [processName] = processCreator	

//	如何区分provider，processor，consumer？
//	为什么必须区分呢？有意义吗？

// 	void RegisterTerminal(const std::string& type, TerminalCreator creator);
// 	void RegisterProcess(const std::string& type, ProcessCreator creator);
// 	void RegisterFlow(const std::string& type, FlowCreator creator);
// 
// 	void UnRegisterTerminal(const std::string& type);
// 	void UnRegisterProcess(const std::string& type);
// 	void UnRegisterFlow(const std::string& type);


#endif
