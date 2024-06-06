#include "pch.h"
#include"dbproxy.h"
#include "SampleCustEnt.h"
#include"acarray.h"
#include "CustEntJig.h"

//----------------------------------------------------------------------------
Adesk::UInt32 CSampleCustEnt::kCurrentVersionNumber = 1;

//----------------------------------------------------------------------------
//---- runtime definition
ACRX_DXF_DEFINE_MEMBERS(
	CSampleCustEnt, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, SAMPLECUSTENT, SAMPLECUSTENTAPP,
	ARXPROJECT1APP
)

//----------------------------------------------------------------------------
//---- construct & destruct

CSampleCustEnt::CSampleCustEnt() {
	/*m_center = AcGePoint3d(0, 0, 0);
	m_radius = 100;*/
}

CSampleCustEnt::~CSampleCustEnt() {}

//----------------------------------------------------------------------------
//----- AcDbObject protocols
//---- Dwg Filing protocol
Acad::ErrorStatus CSampleCustEnt::dwgOutFields(AcDbDwgFiler *pFiler) const {
	assertReadEnabled();
	Acad::ErrorStatus es = AcDbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	if ((es = pFiler->writeUInt32(CSampleCustEnt::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....
	pFiler->writePoint3d(m_center);
	pFiler->writeDouble(m_radius);
	return (pFiler->filerStatus());
}

Acad::ErrorStatus CSampleCustEnt::dwgInFields(AcDbDwgFiler * pFiler) {
	assertWriteEnabled();
	Acad::ErrorStatus es = AcDbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version > CSampleCustEnt::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	if ( version < CSampleCustEnt::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	pFiler->readPoint3d(&m_center);
	pFiler->readDouble(&m_radius);

	return (pFiler->filerStatus());
}

//----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean CSampleCustEnt::subWorldDraw(AcGiWorldDraw * mode) {
	assertReadEnabled();
	mode->geometry().circle(m_center, m_radius, AcGeVector3d::kZAxis);
	return (AcDbEntity::subWorldDraw(mode));
}

Adesk::UInt32 CSampleCustEnt::subSetAttributes(AcGiDrawableTraits * traits) {
	assertReadEnabled();
	return (AcDbEntity::subSetAttributes(traits));
}

Acad::ErrorStatus CSampleCustEnt::subGetGripPoints(AcGePoint3dArray& gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds) const {
	assertReadEnabled();
	AcGePoint3d upPt, dnPt, lePt, riPt;
	upPt.set(m_center.x, m_center.y + m_radius, m_center.z);
	dnPt.set(m_center.x, m_center.y - m_radius, m_center.z);
	lePt.set(m_center.x - m_radius, m_center.y, m_center.z);
	riPt.set(m_center.x + m_radius, m_center.y, m_center.z);
	gripPoints.append(m_center);
	gripPoints.append(upPt);
	gripPoints.append(dnPt);
	gripPoints.append(lePt);
	gripPoints.append(riPt);
	return Acad::eOk;
}
Acad::ErrorStatus CSampleCustEnt::subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d& offset) {
	assertWriteEnabled();
	AcGePoint3d newPt,upPt,dnPt,lePt,riPt;
	switch (indices[0]) {
	case(0):
		m_center += offset;
		break;
	case(1):
		upPt.set(m_center.x, m_center.y + m_radius, m_center.z);
		newPt = upPt + offset;
		m_radius = newPt.distanceTo(m_center);
		break;
	case(2):
		//down point
		dnPt.set(m_center.x, m_center.y - m_radius, m_center.z);
		newPt = dnPt + offset;
		m_radius = newPt.distanceTo(m_center);
		break;
	case(3):
		//left point
		lePt.set(m_center.x - m_radius, m_center.y, m_center.z);
		newPt = lePt + offset;
		m_radius = newPt.distanceTo(m_center);
		break;
	case(4):
		//right point
		riPt.set(m_center.x + m_radius, m_center.y, m_center.z);
		newPt = riPt + offset;
		m_radius = newPt.distanceTo(m_center);
		break;
	}
	return Acad::eOk;
}

Acad::ErrorStatus CSampleCustEnt::subGetOsnapPoints(
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d& pickPoint,
	const AcGePoint3d& lastPoint,
	const AcGeMatrix3d& viewXform,
	AcGePoint3dArray& snapPoints,
	AcDbIntArray & geomIds
) const {
	assertReadEnabled();
	AcDbCircle circle;
	circle.setCenter(m_center);
	circle.setRadius(m_radius);
	acutPrintf(_T("Snap points"));
	return circle.getOsnapPoints(osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds);
}


void CSampleCustEnt::setRadius(double r)
{
	if (r <= 0) {
		acutPrintf(_T("\nUnavailable Radius!"));
	}
	else {
		m_radius = r;
	}
}

double CSampleCustEnt::getRadius()
{
	return m_radius;
}

void CSampleCustEnt::setCenter(AcGePoint3d center)
{
	m_center = center;
}

AcGePoint3d CSampleCustEnt::getCenter()
{
	return m_center;
}

