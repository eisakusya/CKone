#ifndef MYCUSTOMREACTOR_H
#define MYCUSTOMREACTOR_H
#pragma once

class MyCustomReactor : public AcDbObject
{
public:
	ACRX_DECLARE_MEMBERS(MyCustomReactor);

protected:
	static Adesk::UInt32 kCurrentVersionNumber;
private:
	AcDbObjectId entId;
	AcGePoint3d last;

public:
	MyCustomReactor();
	virtual ~MyCustomReactor();

	//----- AcDbObject protocols
	//---- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

	//----- Persistent reactor callbacks
	virtual void openedForModify(const AcDbObject* pDbObj);
	virtual void cancelled(const AcDbObject* pDbObj);
	virtual void objectClosed(const AcDbObjectId objId);
	virtual void goodbye(const AcDbObject* pDbObj);
	virtual void copied(const AcDbObject* pDbObj, const AcDbObject* pNewObj);
	virtual void erased(const AcDbObject* pDbObj, Adesk::Boolean bErasing = true);
	virtual void modified(const AcDbObject* pDbObj);
	virtual void modifiedGraphics(const AcDbEntity* pDbEnt);
	virtual void modifiedXData(const AcDbObject* pDbObj);
	virtual void subObjModified(const AcDbObject* pMainbObj, const AcDbObject* pSubObj);
	virtual void modifyUndone(const AcDbObject* pDbObj);
	virtual void reappended(const AcDbObject* pDbObj);
	virtual void unappended(const AcDbObject* pDbObj);

	void setCenter(AcGePoint3d lastCenter);
	void link(AcDbObjectId objId);

};

#endif

#ifdef ARXPROJECT1_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SampleObject)
#endif