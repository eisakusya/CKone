#ifndef MYEDITORREACTOR_H
#define MYEDITORREACTOR_H
#pragma once
#include "aced.h"
class MyEditorReactor :public AcEditorReactor
{
public:
	ACRX_DECLARE_MEMBERS(MyEditorReactor);

	MyEditorReactor();
	~MyEditorReactor();

	virtual void commandWillStart(const ZTCHAR* cmdStr);

protected:
	static Adesk::UInt32 kCurrentVersionNumber;
};

#endif 

#ifdef ARXPROJECT1_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SampleObject)
#endif