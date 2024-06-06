#include "pch.h"
#include "MyDBReactor.h"
#include "dbproxy.h"
#include "dbmain.h"

ACRX_DXF_DEFINE_MEMBERS(MyDBReactor, AcEditorReactor, AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, MYDBREACTOR, MYDBREACTORAPP)

Adesk::UInt32 MyDBReactor::kCurrentVersionNumber = 1;

MyDBReactor::MyDBReactor()
{
}

MyDBReactor::~MyDBReactor()
{
}

void MyDBReactor::objectAppended(const ZcDbDatabase * dwg, const ZcDbObject * dbObj)
{
	acutPrintf(_T("\nObject appended."));
}
