#include "pch.h"
#include "MyCustomReactor.h"
#include "dbproxy.h"
#include "SampleCustEnt.h"

//----------------------------------------------------------------------------
Adesk::UInt32 MyCustomReactor::kCurrentVersionNumber = 1 ;

//----------------------------------------------------------------------------
//---- runtime definition
ACRX_DXF_DEFINE_MEMBERS (
	MyCustomReactor, AcDbObject,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, MYCUSTOMREACTOR, MYCUSTOMREACTORAPP
)

//----------------------------------------------------------------------------
//---- construct & destruct

MyCustomReactor::MyCustomReactor(){}

MyCustomReactor::~MyCustomReactor(){}

//----------------------------------------------------------------------------
//----- AcDbObject protocols
//---- Dwg Filing protocol
Acad::ErrorStatus MyCustomReactor::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled ();
	Acad::ErrorStatus es = AcDbObject::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	if ((es = pFiler->writeUInt32(MyCustomReactor::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	pFiler->writeItem((AcDbSoftPointerId&)entId);

	return (pFiler->filerStatus());
}

Acad::ErrorStatus MyCustomReactor::dwgInFields(AcDbDwgFiler * pFiler) {
	assertWriteEnabled();
	Acad::ErrorStatus es = AcDbObject::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version > MyCustomReactor::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	if ( version < MyCustomReactor::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	pFiler->readItem((AcDbSoftPointerId*)&entId);

	return (pFiler->filerStatus());
}

//----- Persistent reactor callbacks
void MyCustomReactor::openedForModify(const AcDbObject* pDbObj) {
	assertReadEnabled();
	AcDbObject::openedForModify(pDbObj);
}

void MyCustomReactor::cancelled(const AcDbObject* pDbObj) {
	assertReadEnabled();
	AcDbObject::cancelled(pDbObj);
}

void MyCustomReactor::objectClosed(const AcDbObjectId objId) {
	assertReadEnabled();
	AcDbObject::objectClosed(objId);
}

void MyCustomReactor::goodbye(const AcDbObject* pDbObj) {
	assertReadEnabled();
	AcDbObject::goodbye(pDbObj);
}

void MyCustomReactor::copied(const AcDbObject* pDbObj, const AcDbObject* pNewObj) {
	assertReadEnabled();
	AcDbObject::copied(pDbObj, pNewObj);
}

void MyCustomReactor::erased(const AcDbObject* pDbObj, Adesk::Boolean bErasing) {
	assertReadEnabled();
	AcDbObject::erased(pDbObj, bErasing);
}

void MyCustomReactor::modified(const AcDbObject* pDbObj) {
	AcDbCircle *pCircle = AcDbCircle::cast(pDbObj);
	if (!pCircle) {
		return;
	}

	CSampleCustEnt *pCir2 = nullptr;
	if (acdbOpenObject((AcDbObject*&)pCir2, entId, AcDb::kForWrite) == Acad::eOk) {
		AcGePoint3d c1 = pCircle->center();
		AcGeVector3d v;
		v = c1 - last;
		AcGePoint3d c2 = pCir2->getCenter();
		c2.operator+=(v);
		pCir2->setCenter(c2);
		pCir2->recordGraphicsModified(true);
		pCir2->close();
		last = c1;
	}
}

void MyCustomReactor::modifiedGraphics(const AcDbEntity* pDbEnt) {
	assertReadEnabled();
	AcDbObject::modifiedGraphics(pDbEnt);
}

void MyCustomReactor::modifiedXData(const AcDbObject* pDbObj) {
	assertReadEnabled();
	AcDbObject::modifiedXData(pDbObj);
}

void MyCustomReactor::subObjModified(const AcDbObject* pMainbObj, const AcDbObject* pSubObj) {
	assertReadEnabled();
	AcDbObject::subObjModified(pMainbObj, pSubObj);
}

void MyCustomReactor::modifyUndone(const AcDbObject* pDbObj) {
	assertReadEnabled();
	AcDbObject::modifyUndone(pDbObj);
}

void MyCustomReactor::reappended(const AcDbObject* pDbObj) {
	assertReadEnabled();
	AcDbObject::reappended(pDbObj);
}

void MyCustomReactor::unappended(const AcDbObject* pDbObj) {
	assertReadEnabled();
	AcDbObject::unappended(pDbObj);
}

void MyCustomReactor::setCenter(AcGePoint3d lastCenter)
{
	last = lastCenter;
}

void MyCustomReactor::link(AcDbObjectId objId)
{	
	entId = objId;
}

