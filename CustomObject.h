#ifndef CUSTOMOBJECT_H
#define CUSTOMOBJECT_H
#pragma once

class CCustomObject : public AcDbObject
{
public:
	ACRX_DECLARE_MEMBERS(CCustomObject);

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	CCustomObject();
	virtual ~CCustomObject();

	//----- AcDbObject protocols
	//---- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

};

#endif

#ifdef ARXPROJECT1_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SampleObject)
#endif
