#ifndef SAMPLECUSTENT_H
#define SAMPLECUSTENT_H
#pragma once

class CSampleCustEnt : public AcDbEntity
{
public:
	ACRX_DECLARE_MEMBERS(CSampleCustEnt);

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

private:
	AcGePoint3d m_center;
	double m_radius;

public:
	CSampleCustEnt();
	virtual ~CSampleCustEnt();

	//----- AcDbObject protocols
	//---- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler *pFiler);

	//----- AcDbEntity protocols
	//----- Graphics protocol
protected:
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);
	virtual Adesk::UInt32 subSetAttributes(AcGiDrawableTraits *traits);

	Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray & gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds) const;
	Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d & offset);
	Acad::ErrorStatus subGetOsnapPoints(AcDb::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const AcGePoint3d & pickPoint, const AcGePoint3d & lastPoint, const AcGeMatrix3d & viewXform, AcGePoint3dArray & snapPoints, AcDbIntArray & geomIds) const;
public:
	void setRadius(double r = 0);
	double getRadius();
	void setCenter(AcGePoint3d center);
	AcGePoint3d getCenter();
};

#endif

#ifdef ARXPROJECT1_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SampleCustEnt)
#endif // ARXPROJECT1_MODULE
