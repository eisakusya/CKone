#include "pch.h"
#include"dbproxy.h"
#include "SampleCustEnt.h"
#include"acarray.h"
#include "CustEntJig.h"
#include "dbidmap.h"
#include "dbcfilrs.h"
#define _USE_MATH_DEFINES
#include <math.h>

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
	setHasSaveVersionOverride(true);
}

CSampleCustEnt::CSampleCustEnt(const AcDbObjectId & id)
{
	m_radius = 0.0;
	m_srcId = id;
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

	//- custom entity output subject id
	AcDbHardPointerId id(m_srcId);
	pFiler->writeHardPointerId(id);
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
	if (version < CSampleCustEnt::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//----- Read params
	//.....
	pFiler->readPoint3d(&m_center);
	pFiler->readDouble(&m_radius);

	//- custom entity read subject id
	AcDbHardPointerId id;
	pFiler->readHardPointerId(&id);
	m_srcId = id;
	return (pFiler->filerStatus());
}

//- Dxf Filing protocol
Acad::ErrorStatus CSampleCustEnt::dxfOutFields(AcDbDxfFiler *pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dxfOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	es = pFiler->writeItem(AcDb::kDxfSubclass, _T("SampleCustEnt"));
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(AcDb::kDxfInt32, CSampleCustEnt::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....

	return (pFiler->filerStatus());
}

Acad::ErrorStatus CSampleCustEnt::dxfInFields(AcDbDxfFiler *pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dxfInFields(pFiler);
	if (es != Acad::eOk || !pFiler->atSubclassData(_T("SampleCustEnt")))
		return (pFiler->filerStatus());
	//----- Object version number needs to be read first
	struct resbuf rb;
	pFiler->readItem(&rb);
	if (rb.restype != AcDb::kDxfInt32) {
		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode, _T("\nError: expected group code %d (version #)"), AcDb::kDxfInt32);
		return (pFiler->filerStatus());
	}
	Adesk::UInt32 version = (Adesk::UInt32)rb.resval.rlong;
	if (version > CSampleCustEnt::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < SampleCustEnt::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params in non order dependant manner
	while (es == Acad::eOk && (es = pFiler->readResBuf(&rb)) == Acad::eOk) {
		switch (rb.restype) {
			//----- Read params by looking at their DXF code (example below)
		case AcDb::kDxfXCoord:
			break;

		default:
			//----- An unrecognized group. Push it back so that the subclass can read it again.
			pFiler->pushBackItem();
			es = Acad::eEndOfFile;
			break;
		}
	}
	//----- At this point the es variable must contain eEndOfFile
	//----- - either from readResBuf() or from pushback. If not,
	//----- it indicates that an error happened and we should
	//----- return immediately.
	if (es != Acad::eEndOfFile)
		return (Acad::eInvalidResBuf);

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
	
	gripPoints.append(m_center);
	
	for (int i = 0; i < 8; ++i) {
		double angle = (i * 45.0) * (M_PI / 180.0); // 45度等分
		double x = m_center.x + m_radius * cos(angle);
		double y = m_center.y + m_radius * sin(angle);
		gripPoints.append(AcGePoint3d(x, y, m_center.z));
	}
	return Acad::eOk;
}

Acad::ErrorStatus CSampleCustEnt::subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d& offset) {
	assertWriteEnabled(false);
	if (indices.isEmpty()) {
		return Acad::eInvalidInput;
	}

	bool movedCenter = false;
	bool radiusChanged = false;
	double newRadius = m_radius;
	AcGePoint3d newCenter = m_center;

	for (int i = 0; i < indices.length(); ++i) {
		int index = indices[i];
		if (index == 0) {
			// Move the entire entity if the center grip point is moved
			newCenter += offset;
			movedCenter = true;
		}
		else if (index > 0 && index <= 8) {
			// Adjust the radius if any of the 8 points are moved
			AcGePoint3d gripPoint = m_center + AcGeVector3d(m_radius * cos((index - 1) * M_PI / 4.0),
				m_radius * sin((index - 1) * M_PI / 4.0),
				0.0);
			gripPoint += offset;
			newRadius = (gripPoint - m_center).length();
			radiusChanged = true;
		}
	}

	if (movedCenter) {
		setCenter(newCenter);
	}

	if (radiusChanged) {
		setRadius(newRadius);
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
	if (osnapMode == AcDb::kOsModeCen) {
		// Center point osnap
		snapPoints.append(m_center);
	}
	else if (osnapMode == AcDb::kOsModeQuad) {
		// Quadrant points osnap
		for (int i = 0; i < 8; ++i) {
			double angle = (i * 45.0) * (M_PI / 180.0); // 45度等分
			double x = m_center.x + m_radius * cos(angle);
			double y = m_center.y + m_radius * sin(angle);
			snapPoints.append(AcGePoint3d(x, y, m_center.z));
		}
	}
	acutPrintf(_T("Snap points"));
	return Acad::eOk;
	
}


Acad::ErrorStatus CSampleCustEnt::setRadius(double r)
{
	assertWriteEnabled(Adesk::kFalse, Adesk::kFalse);	//false for banning auto undo
	if (r <= 0) {
		acutPrintf(_T("\nUnavailable Radius!"));
		return Acad::eInvalidInput;
	}
	else {
		AcDbDwgFiler* pFiler = NULL;
		if ((pFiler = undoFiler()) != NULL) {
			undoFiler()->writeAddress(CSampleCustEnt::desc());
			undoFiler()->writeItem((Adesk::Int16)PartialUndoCode::kRadius);
			undoFiler()->writeDouble(m_radius);
		}
		m_radius = r;
		recordGraphicsModified(true);
		return Acad::eOk;
	}
}

double CSampleCustEnt::getRadius()
{
	assertReadEnabled();
	return m_radius;
}

Acad::ErrorStatus CSampleCustEnt::setCenter(AcGePoint3d center)
{
	assertWriteEnabled(Adesk::kFalse,Adesk::kFalse);
	AcDbDwgFiler* pFiler = NULL;
	if ((pFiler = undoFiler()) != NULL) {
		undoFiler()->writeAddress(CSampleCustEnt::desc());
		undoFiler()->writeItem((Adesk::Int16)PartialUndoCode::kCenter);
		undoFiler()->writeItem(m_center);
	}
	m_center = center;
	recordGraphicsModified(true);
	return Acad::eOk;
}

AcGePoint3d CSampleCustEnt::getCenter()
{
	assertReadEnabled();

	return m_center;
}

Acad::ErrorStatus CSampleCustEnt::setSourceId(AcDbObjectId id)
{
	assertWriteEnabled();
	m_srcId = id;
	return Acad::eOk;
}

Acad::ErrorStatus CSampleCustEnt::subDeepClone(AcDbObject* pOwner,
	AcDbObject*&   pClonedObject,
	AcDbIdMapping& idMap,
	Adesk::Boolean isPrimary) const
{
	//- initialization
	pClonedObject = NULL;

	//- whether has been copied
	bool isPrim = false;
	if (isPrimary)//isPrimary表示当前被复制的对象是被主动复制的还是因为所有关系而被复制的
		isPrim = true;
	AcDbIdPair idPair(objectId(), (AcDbObjectId)NULL, false, isPrim);
	if (idMap.compute(idPair) && (idPair.value() != NULL))
		return Acad::eOk;

	//create same entity
	auto *pClone = (CSampleCustEnt*)isA()->create();
	if (pClone != NULL)
		pClonedObject = pClone;
	else
		return Acad::eOutOfMemory;

	//copy entity data to cloned entity
	AcDbDeepCloneFiler filer;
	dwgOut(&filer);
	filer.seek(0L, AcDb::kSeekFromStart);
	pClone->dwgIn(&filer);

	//owner setting conditions
	bool bOwnerXlated = false;	//indicate whether owner has been translated
	if (isPrimary)
	{
		AcDbBlockTableRecord *pBTR = AcDbBlockTableRecord::cast(pOwner);
		if (pBTR != NULL)
		{
			//owner is record
			pBTR->appendAcDbEntity(pClone);
			bOwnerXlated = true;
		}
		else
		{
			AcDbDictionary *pD = AcDbDictionary::cast(pOwner);
			if (pD != NULL) {
				//owner is dictionary
				pD->setAt(_T("Cloned_Entity"), pClone, pClone->objectId());
				bOwnerXlated = true;
			}
		}
	}
	else {
		//subject is copy by reference
		//add cloned entity to database
		pOwner->database()->addAcDbObject(pClone);
		pClone->setOwnerId(pOwner->objectId());
		bOwnerXlated = true;
	}

	//setAcDbObjectIdsInFlux() before added into ID Map during deepclone method
	pClone->setAcDbObjectIdsInFlux();
	pClone->disableUndoRecording(true);

	//pair subject entity and clone entity and add into id Map
	//key: subject entity
	idPair.setValue(pClonedObject->objectId());
	idPair.setIsCloned(Adesk::kTrue);
	idPair.setIsOwnerXlated(bOwnerXlated);
	idMap.assign(idPair);

	//查找源对象的所有关系，调用它们的deepClone方法
	AcDbObjectId id;
	while (filer.getNextOwnedObject(id)) {

		AcDbObject *pSubObject;
		AcDbObject *pClonedSubObject;

		if (id == NULL)
			continue;

		//注意因为这些对象都是由于所有关系而被复制的，所以deepClone的最后一个参数(isPrimary)需要设为false
		acdbOpenAcDbObject(pSubObject, id, AcDb::kForRead);
		pClonedSubObject = NULL;
		pSubObject->deepClone(pClonedObject, pClonedSubObject, idMap, Adesk::kFalse);

		//在某特殊情况下(比如某些情况下的预览)，复制实际上只是数据“移动”了
		//pSubObject和pClonedSubObject会指向同一个对象，这个时候就不能close pSubObject了
		if (pSubObject != pClonedSubObject)
			pSubObject->close();

		//最后如果复制失败pClonedSubObject就会是空指针，需要检查
		if (pClonedSubObject != NULL)
			pClonedSubObject->close();
	}

	return Acad::eOk;
}

Acad::ErrorStatus CSampleCustEnt::subWblockClone(
	AcRxObject*    pOwner,
	AcDbObject*&   pClonedObject,
	AcDbIdMapping& idMap,
	Adesk::Boolean isPrimary) const
{
	pClonedObject = NULL;

	//if destinate dwg is original dwg, involke based wblockclone
	AcDbDatabase *pDest, *pOrig;
	idMap.destDb(pDest);
	idMap.origDb(pOrig);
	if (pDest == pOrig)
		return AcDbEntity::wblockClone(pOwner, pClonedObject,
			idMap, isPrimary);

	//如果这是在进行xbind操作，由于xbind只处理模型空间的对象，如果源对象在图纸空间，就直接返回
	AcDbObjectId pspace;
	AcDbBlockTable *pTable;
	database()->getSymbolTable(pTable, AcDb::kForRead);
	pTable->getAt(ACDB_PAPER_SPACE, pspace);
	pTable->close();
	if (idMap.deepCloneContext() == AcDb::kDcXrefBind
		&& ownerId() == pspace)
		return Acad::eOk;

	//if has been copied
	bool isPrim = false;
	if (isPrimary)
		isPrim = true;
	AcDbIdPair idPair(objectId(), (AcDbObjectId)NULL,
		false, isPrim);
	if (idMap.compute(idPair) && (idPair.value() != NULL))
		return Acad::eOk;

	//源对象的owner有可能是数据库，也有可能是一个AcDbObject，
	//如果owner是数据库，表示这个对象可能是由于硬指针关系而被复制的，它的真实的owner会在转译阶段被正确设置
	//此时pOwn的值会是NULL，作为一个标志
	AcDbObject   *pOwn = AcDbObject::cast(pOwner);
	AcDbDatabase *pDb = AcDbDatabase::cast(pOwner);
	if (pDb == NULL)
		pDb = pOwn->database();

	//create entity 
	auto *pClone = (CSampleCustEnt*)isA()->create();
	if (pClone != NULL)
		pClonedObject = pClone;
	else
		return Acad::eOutOfMemory;

	AcDbBlockTableRecord *pBTR = NULL;
	//if owner is record or something we know
	//otherwise original owner need to be copied to the cloned one
	if (pOwn != NULL)
		pBTR = AcDbBlockTableRecord::cast(pOwn);
	if (pBTR != NULL && isPrimary) {
		pBTR->appendAcDbEntity(pClone);
	}
	else {
		pDb->addAcDbObject(pClonedObject);
	}

	//data copy
	AcDbWblockCloneFiler filer;
	dwgOut(&filer);
	filer.seek(0L, AcDb::kSeekFromStart);
	pClone->dwgIn(&filer);

	//pair subject and cloned and add into id Map
	//isOwnerXlated indicates whether owner need translation, decided by whether owner is null
	idMap.assign(AcDbIdPair(objectId(), pClonedObject->objectId(), Adesk::kTrue, isPrim, (Adesk::Boolean)(pOwn != NULL)));

	//original owner need to be copied to the cloned one
	pClonedObject->setOwnerId((pOwn != NULL) ? pOwn->objectId() : ownerId());

	//setAcDbObjectIdsInFlux() before wblockclone
	pClone->setAcDbObjectIdsInFlux();

	//递归复制所有的硬所有和硬指针
	AcDbObjectId id;
	while (filer.getNextHardObject(id)) {
		AcDbObject *pSubObject;
		AcDbObject *pClonedSubObject;

		//某些情况下会出现引用的对象为空，需要跳过
		if (id == NULL)
			continue;

		//如果引用对象来自于另一个数据库，不需要复制
		acdbOpenAcDbObject(pSubObject, id, AcDb::kForRead);
		if (pSubObject->database() != database()) {
			pSubObject->close();
			continue;
		}

		//如上面所述，如果这个对象不是引用对象的owner，就把数据库作为owner传进去
		//另外跟deepclone一样，这里isPrimary参数要设为false
		pClonedSubObject = NULL;
		if (pSubObject->ownerId() == objectId()) {
			pSubObject->wblockClone(pClone,
				pClonedSubObject,
				idMap, Adesk::kFalse);
		}
		else {
			pSubObject->wblockClone(
				pClone->database(),
				pClonedSubObject,
				idMap, Adesk::kFalse);
		}
		pSubObject->close();

		//如果复制失败，pClonedSubObject会是空指针，需要检查
		if (pClonedSubObject != NULL)
			pClonedSubObject->close();
	}

	return Acad::eOk;
}

Acad::ErrorStatus CSampleCustEnt::applyPartialUndo(AcDbDwgFiler* undoFiler, AcRxClass* classObj)
{
	if (classObj != CSampleCustEnt::desc())
		return AcDbEntity::applyPartialUndo(undoFiler, classObj);
	Adesk::Int16 shortCode;
	undoFiler->readItem(&shortCode);
	PartialUndoCode code = (PartialUndoCode)shortCode;
	double rad = 0;
	AcGePoint3d center;
	switch (code) {
	case kRadius:
		//sequce as dwg out when multiple data
		undoFiler->readDouble(&rad);
		setRadius(rad);
		acutPrintf(_T("Partial undo radius"));
		break;
	case kCenter:
		undoFiler->readItem(&center);
		setCenter(center);
		acutPrintf(_T("Partial undo center"));
		break;
	default:
		assert(Adesk::kFalse);
		break;
	}
	return Acad::eOk;
}
