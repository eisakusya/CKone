#ifndef SAMPLECUSTENT_H
#define SAMPLECUSTENT_H
#pragma once

class CSampleCustEnt : public AcDbEntity
{
public:
	ACRX_DECLARE_MEMBERS(CSampleCustEnt);
	enum PartialUndoCode
	{
		kNoOp = 1,
		kCenter = 2,
		kRadius = 3
	};

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

private:
	AcGePoint3d m_center;
	double m_radius;
	AcDbObjectId m_srcId;

public:
	CSampleCustEnt();
	CSampleCustEnt(const AcDbObjectId& id);
	virtual ~CSampleCustEnt();

	//----- AcDbObject protocols
	//---- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler *pFiler);

	Acad::ErrorStatus dxfOutFields(AcDbDxfFiler * pFiler) const;
	Acad::ErrorStatus dxfInFields(AcDbDxfFiler * pFiler);

	//----- AcDbEntity protocols
	//----- Graphics protocol
protected:
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);
	virtual Adesk::UInt32 subSetAttributes(AcGiDrawableTraits *traits);

	Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray & gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds) const;
	Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d & offset);
	Acad::ErrorStatus subGetOsnapPoints(AcDb::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const AcGePoint3d & pickPoint, const AcGePoint3d & lastPoint, const AcGeMatrix3d & viewXform, AcGePoint3dArray & snapPoints, AcDbIntArray & geomIds) const;

	Acad::ErrorStatus subDeepClone(AcDbObject * pOwner, AcDbObject *& pClonedObject, AcDbIdMapping & idMap, Adesk::Boolean isPrimary) const;
	Acad::ErrorStatus subWblockClone(AcRxObject * pOwner, AcDbObject *& pClonedObject, AcDbIdMapping & idMap, Adesk::Boolean isPrimary) const;

public:
	Acad::ErrorStatus setRadius(double r = 0);
	double getRadius();
	Acad::ErrorStatus setCenter(AcGePoint3d center);
	AcGePoint3d getCenter();
	Acad::ErrorStatus setSourceId(AcDbObjectId id);
	Acad::ErrorStatus applyPartialUndo(AcDbDwgFiler * undoFiler, AcRxClass * classObj);
};

#endif

#ifdef ARXPROJECT1_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SampleCustEnt)
#endif // ARXPROJECT1_MODULE
