#include "pch.h"
#include "ObjectToNotify.h"
#include "dbproxy.h"


//----------------------------------------------------------------------------
Adesk::UInt32 ObjectToNotify::kCurrentVersionNumber = 1;

//----------------------------------------------------------------------------
//---- runtime definition
ACRX_DXF_DEFINE_MEMBERS(
	ObjectToNotify, AcDbObject,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, OBJECTTONOTIFY, OBJECTTONOTIFYAPP
)

//----------------------------------------------------------------------------
//---- construct & destruct

ObjectToNotify::ObjectToNotify() {}

ObjectToNotify::~ObjectToNotify() {}

//----------------------------------------------------------------------------
//----- AcDbObject protocols
//---- Dwg Filing protocol
Acad::ErrorStatus ObjectToNotify::dwgOutFields(AcDbDwgFiler *pFiler) const {
	assertReadEnabled();
	Acad::ErrorStatus es = AcDbObject::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	if ((es = pFiler->writeUInt32(ObjectToNotify::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	pFiler->writeItem(mFactor);
	pFiler->writeItem((AcDbSoftPointerId&)mId);
	return (pFiler->filerStatus());
}

Acad::ErrorStatus ObjectToNotify::dwgInFields(AcDbDwgFiler * pFiler) {
	assertWriteEnabled();
	Acad::ErrorStatus es = AcDbObject::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version > ObjectToNotify::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	if ( version < ObjectToNotify::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;

	pFiler->readItem(&mFactor);
	pFiler->readItem((AcDbSoftPointerId*)&mId);
	return (pFiler->filerStatus());
}

//----- Persistent reactor callbacks
void ObjectToNotify::openedForModify(const AcDbObject* pDbObj) {
	assertReadEnabled();
	AcDbObject::openedForModify(pDbObj);
}

void ObjectToNotify::cancelled(const AcDbObject* pDbObj) {
	assertReadEnabled();
	AcDbObject::cancelled(pDbObj);
}

void ObjectToNotify::objectClosed(const AcDbObjectId objId) {
	assertReadEnabled();
	AcDbObject::objectClosed(objId);
}

void ObjectToNotify::goodbye(const AcDbObject* pDbObj) {
	assertReadEnabled();
	AcDbObject::goodbye(pDbObj);
}

void ObjectToNotify::copied(const AcDbObject* pDbObj, const AcDbObject* pNewObj) {
	assertReadEnabled();
	AcDbObject::copied(pDbObj, pNewObj);
}

void ObjectToNotify::erased(const AcDbObject* pDbObj, Adesk::Boolean bErasing) {
	assertReadEnabled();
	AcDbObject::erased(pDbObj, bErasing);
}

void ObjectToNotify::modified(const AcDbObject* pDbObj) {
	AcDbLine *pLine = AcDbLine::cast(pDbObj);
	if (!pLine) {
		return;
	}
	AcDbLine *pLine2 = nullptr;
	if (acdbOpenObject((AcDbObject*&)pLine2, mId, AcDb::kForWrite) == Acad::eOk) {
		AcGePoint3d p = pLine->startPoint();
		AcGePoint3d q = pLine->endPoint();
		AcGeVector3d v = q - p;
		double len = v.length();
		p = pLine2->startPoint();
		q = pLine2->endPoint();
		v = q - p;
		v = len * mFactor * v.normal();
		pLine2->setEndPoint(p + v);
		pLine2->close();
	}
}

void ObjectToNotify::modifiedGraphics(const AcDbEntity* pDbEnt) {
	assertReadEnabled();
	AcDbObject::modifiedGraphics(pDbEnt);
}

void ObjectToNotify::modifiedXData(const AcDbObject* pDbObj) {
	assertReadEnabled();
	AcDbObject::modifiedXData(pDbObj);
}

void ObjectToNotify::subObjModified(const AcDbObject* pMainbObj, const AcDbObject* pSubObj) {
	assertReadEnabled();
	AcDbObject::subObjModified(pMainbObj, pSubObj);
}

void ObjectToNotify::modifyUndone(const AcDbObject* pDbObj) {
	assertReadEnabled();
	AcDbObject::modifyUndone(pDbObj);
}

void ObjectToNotify::reappended(const AcDbObject* pDbObj) {
	assertReadEnabled();
	AcDbObject::reappended(pDbObj);
}

void ObjectToNotify::unappended(const AcDbObject* pDbObj) {
	assertReadEnabled();
	AcDbObject::unappended(pDbObj);
}

void ObjectToNotify::link(AcDbObjectId lineID)
{
	mId = lineID;
}

void ObjectToNotify::setFactor(double factor)
{
	mFactor = factor;
}

