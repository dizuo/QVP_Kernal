#ifndef VP_PLUGIN_H
#define VP_PLUGIN_H

#include <iostream>
#include <string>
#include <map>

#include "vpterminal.h"
#include "..\factory\aflow.hh"
#include "..\factory\aprocess.hh"

//////////////////////////////////////////////////////////////////////////
// function pointer definition
//////////////////////////////////////////////////////////////////////////
typedef AFlow* (*FLOW_CREATOR)();
typedef AProcess* (*PROCESSOR_CREATOR)();
typedef VPProvider* (*PROVIDER_CREATOR)(AGraphExec*);
typedef VPConsumer* (*CONSUMER_CREATOR)(AGraphExec*);

//////////////////////////////////////////////////////////////////////////
// plugin map definition
//////////////////////////////////////////////////////////////////////////
typedef std::map
<
	std::string, 
	PROVIDER_CREATOR
>
PROVIDER_PLUGIN_MAP;

typedef std::map
<
	std::string, 
	PROCESSOR_CREATOR
>
PROCESSOR_PLUGIN_MAP;

typedef std::map
<
	std::string, 
	CONSUMER_CREATOR
>
CONSUMER_PLUGIN_MAP;

typedef std::map
<
	std::string, 
	FLOW_CREATOR
>
FLOW_PLUGIN_MAP;

//////////////////////////////////////////////////////////////////////////
// 9/1/2010 RYF
//////////////////////////////////////////////////////////////////////////

#endif
