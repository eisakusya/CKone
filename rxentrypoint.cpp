#include "pch.h"
#include "adsmigr.h"
#include "adsdef.h"
#include "adscodes.h"
#include "acestext.h"
#include "acedads.h"
#include "helloworld.h"
#include "DWGDatabase.h"
#include "SampleCustEnt.h"
#include "dbxEntryPoint.h"
#include "CustomObject.h"
#include "MyEditorReactor.h"
#include "MyDBReactor.h"
#include "ObjectToNotify.h"
#include "MyCustomReactor.h"

void initapp()
{
	acedRegCmds->addCommand(cmd_group_name, _T("helloworld"), _T("helloworld"), ACRX_CMD_MODAL, helloworld);
	acedRegCmds->addCommand(cmd_group_name, _T("addline"), _T("addline"), ACRX_CMD_MODAL, addLine);
	acedRegCmds->addCommand(cmd_group_name, _T("readSample"), _T("readSample"), ACRX_CMD_MODAL, readSample);
	acedRegCmds->addCommand(cmd_group_name, _T("saveSample"), _T("saveSample"), ACRX_CMD_MODAL, saveSample);
	acedRegCmds->addCommand(cmd_group_name, _T("addCircle"), _T("addCircle"), ACRX_CMD_MODAL, addCircle);
	acedRegCmds->addCommand(cmd_group_name, _T("draw255"), _T("draw255"), ACRX_CMD_MODAL, draw255Circle);
	acedRegCmds->addCommand(cmd_group_name, _T("copyentity"), _T("copyentity"), ACRX_CMD_MODAL, copyEntity);
	acedRegCmds->addCommand(cmd_group_name, _T("pickpoint"), _T("pickpoint"), ACRX_CMD_MODAL, pickPoint);
	acedRegCmds->addCommand(cmd_group_name, _T("myline"), _T("myline"), ACRX_CMD_MODAL, myLine);
	acedRegCmds->addCommand(cmd_group_name, _T("customentity"), _T("customentity"), ACRX_CMD_MODAL, drawCunstomEntity);
	acedRegCmds->addCommand(cmd_group_name, _T("mycircle"), _T("mycircle"), ACRX_CMD_MODAL, myCircle);
	acedRegCmds->addCommand(cmd_group_name, _T("edreact"), _T("edreact"), ACRX_CMD_MODAL, registerEditorReactor);
	acedRegCmds->addCommand(cmd_group_name, _T("dbreact"), _T("dbreact"), ACRX_CMD_MODAL, registerDBReactor);
	acedRegCmds->addCommand(cmd_group_name, _T("linereact"), _T("linereact"), ACRX_CMD_MODAL, lineReactor);

	CSampleCustEnt::rxInit();
	acrxBuildClassHierarchy();

	CCustomObject::rxInit();
	acrxBuildClassHierarchy();

	MyEditorReactor::rxInit();
	acrxBuildClassHierarchy();

	MyDBReactor::rxInit();
	acrxBuildClassHierarchy();

	ObjectToNotify::rxInit();
	acrxBuildClassHierarchy();

	MyCustomReactor::rxInit();
	acrxBuildClassHierarchy();
}

void unloadapp()
{
	acedRegCmds->removeGroup(cmd_group_name);
	deleteZcRxClass(CSampleCustEnt::desc());
	deleteZcRxClass(CCustomObject::desc());
	deleteZcRxClass(MyEditorReactor::desc());
	deleteZcRxClass(MyDBReactor::desc());
	deleteZcRxClass(ObjectToNotify::desc());
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

