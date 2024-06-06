#ifndef MYDBREACTOR_H
#define MYDBREACTOR_H
#pragma once
#include "dbmain.h"
class MyDBReactor :
	public AcDbDatabaseReactor
{
public:
	ACRX_DECLARE_MEMBERS(MyDBReactor);
	MyDBReactor();
	~MyDBReactor();

	virtual void objectAppended(const ZcDbDatabase* dwg, const ZcDbObject* dbObj);

protected:
	static Adesk::UInt32 kCurrentVersionNumber;
};

#endif 

#ifdef ARXPROJECT1_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SampleObject)
#endif