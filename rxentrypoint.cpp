#include "pch.h"
#include "adsmigr.h"
#include "adsdef.h"
#include "adscodes.h"
#include "acestext.h"
#include "acedads.h"
#include "helloworld.h"
#include "DWGDatabase.h"	//homework operate cpp
#include "SampleCustEnt.h"		//4th,6th homework
#include "dbxEntryPoint.h"
#include "MyCustomReactor.h"	//6th homework

void initapp()
{
	acedRegCmds->addCommand(cmd_group_name, _T("helloworld"), _T("helloworld"), ACRX_CMD_MODAL, helloworld);
	//- second homework
	acedRegCmds->addCommand(cmd_group_name, _T("draw255"), _T("draw255"), ACRX_CMD_MODAL, draw255Circle);
	//- third homework
	acedRegCmds->addCommand(cmd_group_name, _T("myline"), _T("myline"), ACRX_CMD_MODAL, myLine);
	//- 4th and 6th homework
	acedRegCmds->addCommand(cmd_group_name, _T("customentity"), _T("customentity"), ACRX_CMD_MODAL, drawCunstomEntity);
	//- 5th homwork
	acedRegCmds->addCommand(cmd_group_name, _T("mycircle"), _T("mycircle"), ACRX_CMD_MODAL, myCircle);

	CSampleCustEnt::rxInit();
	MyCustomReactor::rxInit();
	acrxBuildClassHierarchy();
}

void unloadapp()
{
	acedRegCmds->removeGroup(cmd_group_name);
	deleteZcRxClass(CSampleCustEnt::desc());
	deleteZcRxClass(MyCustomReactor::desc());
}


#define ELEMENTS(array) (sizeof(array)/sizeof((array)[0]))

struct func_entry { TCHAR *func_name; int(*func) (struct resbuf *); };

struct func_entry func_table[] = {
	{_T("showhello"), showhello},
	{_T("c:showhello"), showhellocmd},
};

int funcload()
{
	for (int i = 0; i < ELEMENTS(func_table); i++)
	{
		if (!acedDefun(func_table[i].func_name, i))
			return RTERROR;
	}

	return RTNORM;
}

int dofunc()
{
	struct resbuf* rb = NULL;
	int val = 0;

	if ((val = acedGetFunCode()) < 0 || val >= ELEMENTS(func_table))
	{
		acdbFail(_T("Received nonexistent function code."));
		return RTERROR;
	}
	rb = acedGetArgs();
	val = (*func_table[val].func)(rb);
	acutRelRb(rb);
	return val;
}



extern "C" AcRx::AppRetCode zcrxEntryPoint(AcRx::AppMsgCode msg, void* appId)
{
	switch (msg)
	{
		case AcRx::kInitAppMsg:
		{
			acrxDynamicLinker->unlockApplication(appId);
			acrxDynamicLinker->registerAppMDIAware(appId);
			initapp();
		}
		break;
		case AcRx::kUnloadAppMsg:
		{
			unloadapp();
		}
		break;
		
		case AcRx::kLoadDwgMsg:
		{
			funcload();
		}
		break;
		case AcRx::kUnloadDwgMsg:
		{
			// Add your code here.
		}
		break;
		case AcRx::kInvkSubrMsg:
		{
			dofunc();
		}
		break;
		
		default:
			break;
	}
	return AcRx::kRetOK;
}



#ifdef _WIN64
#pragma comment(linker, "/export:zcrxEntryPoint,PRIVATE")
#pragma comment(linker, "/export:zcrxGetApiVersion,PRIVATE")
#else // WIN32
#pragma comment(linker, "/export:_zcrxEntryPoint,PRIVATE")
#pragma comment(linker, "/export:_zcrxGetApiVersion,PRIVATE")
#endif

