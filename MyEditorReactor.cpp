#include "pch.h"
#include "MyEditorReactor.h"
#include "aced.h"
#include "dbproxy.h"

ACRX_DXF_DEFINE_MEMBERS(MyEditorReactor, AcEditorReactor, AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, MYEDITORREACTOR, MYEDITORREACTORAPP)

Adesk::UInt32 MyEditorReactor::kCurrentVersionNumber = 1;

MyEditorReactor::MyEditorReactor()
{
}

MyEditorReactor::~MyEditorReactor()
{
}

void MyEditorReactor::commandWillStart(const ZTCHAR * cmdStr)
{
	acutPrintf(_T("\nCommand %s is about to start."), cmdStr);
}
