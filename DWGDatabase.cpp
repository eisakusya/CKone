#include "pch.h"
#include "DWGDatabase.h"
#include "dbapserv.h"
#include "dbents.h"
#include "dbxrecrd.h"
#include "SampleCustEnt.h"
#include <atlstr.h>
#include<iostream>
#include<string>
#include<stack>
#include<math.h>
#include "CustEntJig.h"
#include "MyEditorReactor.h"
#include "MyDBReactor.h"
#include "ObjectToNotify.h"

auto createDatabase() {
	auto pDb = new AcDbDatabase();
	return pDb;
}

auto readDatabase() {
	auto pDb = new AcDbDatabase(false, true);
	pDb->readDwgFile(_T("D:\Study\experiment\ZWSOFT\resources\sample.dwg"));
	pDb->closeInput(true);
	return pDb;
}

void saveDatabase(AcDbDatabase* pDb) {
	pDb->saveAs(_T("D:\Study\experiment\ZWSOFT\resources\sample.dwg"));
}

auto getDatabase() {
	auto pDb = acdbHostApplicationServices()->workingDatabase();
	return pDb;
}

AcDbDictionary* getNameDictionary() {
	AcDbDatabase* pDb = getDatabase();
	//method to get the named object dictionary of a database, dictionary of dictionaries

	AcDbDictionary* pNOD = nullptr;
	acdbOpenAcDbObject((AcDbObject*&)pNOD, pDb->namedObjectsDictionaryId(), AcDb::kForWrite);
	//named dictionary id->pNOD
	return pNOD;
}

void printNamedDcitionary() {
	AcDbDatabase* pDb = getDatabase();
	AcDbDictionary* pNOD = getNameDictionary();
	if (pNOD) {
		auto pItr = pNOD->newIterator();
		while (!pItr->done()) {
			acutPrintf(_T("\n%s"), pItr->name());
			pItr->next();
		}
		delete pItr;
		pNOD->close();
	}
}

void createDictionary() {
	AcDbDictionary* pNOD = getNameDictionary();
	AcDbDictionary* pDict = new AcDbDictionary();
	AcDbObjectId dictID;
	pNOD->setAt(_T("MyDict"), pDict, dictID);
	pDict->close();
	pNOD->close();
}

void addTextDictionaryItem(AcDbDictionary* pDict, const ACHAR* key, const ACHAR* item) {
	if (!pDict->has(key)) {
		auto pXrec = new AcDbXrecord();
		resbuf* pRb = acutBuildList(AcDb::kDxfText, item, RTNONE);
		pXrec->setFromRbChain(*pRb);
		AcDbObjectId xrecID;
		pDict->setAt(key, pXrec, xrecID);
		pXrec->close();
		//
		acutRelRb(pRb);
		pRb = nullptr;
	}
}

void addIntDictionaryItem(AcDbDictionary* pDict, const ACHAR* key, int item) {
	if (!pDict->has(key)) {
		auto pXrec = new AcDbXrecord();
		resbuf* pRb = acutBuildList(AcDb::kDxfInt64, item, RTNONE);
		pXrec->setFromRbChain(*pRb);
		AcDbObjectId xrecID;
		pDict->setAt(key, pXrec, xrecID);
		pXrec->close();
		acutRelRb(pRb);
		pRb = nullptr;
	}
}

AcDbBlockTable* getBlockTable() {
	auto pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable* pBT = nullptr;
	pDb->getBlockTable(pBT, AcDb::kForWrite);
	if (!pBT) {
		return false;
	}

	//delete pDb;
	return pBT;
}

AcDbBlockTableRecord* getBlockTableRecord(AcDbBlockTable* pBT) {
	AcDbBlockTableRecord* pBTR = nullptr;
	pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite);

	if (!pBTR) {
		return false;
	}
	//content for using block table record
	return pBTR;
}

void appendEntity(AcDbBlockTableRecord* pBTR) {
	AcGePoint3d startPt(4.0, 2.0, 0.0);
	AcGePoint3d EndPt(10.0, 7.0, 0.0);

	AcDbLine *pLine = new AcDbLine(startPt, EndPt);
	AcDbObjectId lineID;

	pBTR->appendAcDbEntity(lineID, pLine);

	pLine->close();
}

AcDbObjectId createAnonymousBlock() {
	AcDbBlockTable* pBT = getBlockTable();
	auto pBTR = new AcDbBlockTableRecord();
	pBTR->setName(_T("*U"));
	AcDbObjectId btrID;
	pBT->add(btrID, pBTR);
	pBTR->close();
	pBT->close();
	return btrID;
}

void showBlock(AcDbBlockTable* pBT, AcDbObjectId blockID) {
	auto pModelSpace = new AcDbBlockTableRecord();
	pBT->getAt(ACDB_MODEL_SPACE, pModelSpace, AcDb::kForWrite);

	AcDbBlockReference* pRef = new AcDbBlockReference(AcGePoint3d(0, 0, 0), blockID);
	pModelSpace->appendAcDbEntity(pRef);
	pRef->close();
	pModelSpace->close();
}

//test program below

void addLine() {
	auto pDb = createDatabase();
	AcDbBlockTableRecord* pBTR = getBlockTableRecord(getBlockTable());
	appendEntity(pBTR);
	pBTR->close();
	delete pDb;
	acutPrintf(_T("Line Added!"));
}

void readSample() {
	auto pDb = readDatabase();
	delete pDb;
}

void saveSample() {
	auto pDb = acdbHostApplicationServices()->workingDatabase();
	saveDatabase(pDb);
	delete pDb;
}

void addCircle() {
	auto pDb = createDatabase();
	AcDbBlockTable* pBT = getBlockTable();
	auto pBTR = new AcDbBlockTableRecord();
	pBTR->setName(_T("Sample"));
	AcDbObjectId btrID;
	pBT->add(btrID, pBTR);	//create and save record

	//ZcDbCircle* pCir = new ZcDbCircle(AcGePoint3d::kOrigin, AcGeVector3d::kZAxis, 50);
	auto pCir = new AcDbCircle(AcGePoint3d::kOrigin, AcGeVector3d::kZAxis, 50);
	AcDbObjectId cirID;
	pBTR->appendAcDbEntity(cirID, pCir);
	pCir->close();
	pBTR->close();		//add an object to the record

	showBlock(pBT, btrID);

	pBT->close();
	delete pDb;
	acutPrintf(_T("Circle Added!"));
}

void draw255Circle()
{
	//get all figure
	AcDbDatabase* pDb = createDatabase();
	AcDbDictionary* pNOD = getNameDictionary();

	//create anonymous block
	AcDbObjectId blockID = createAnonymousBlock();
	AcDbBlockTable* pBT = getBlockTable();
	AcDbBlockTableRecord* pBTR = getBlockTableRecord(pBT);

	//plot circles
	for (int i = 1; i <= 255; ++i) {
		//for each circle
		auto pCir = new AcDbCircle(AcGePoint3d::kOrigin, AcGeVector3d::kZAxis, i);
		AcDbObjectId cirID;
		pCir->setColorIndex(i);
		pBTR->appendAcDbEntity(cirID, pCir);

		//add handle of circle to named dict
		AcDbHandle hd;
		pCir->getZcDbHandle(hd);
		ACHAR strHd[16];
		hd.getIntoAsciiBuffer(strHd, 16);
		addTextDictionaryItem(pNOD, strHd, strHd);

		//record circle's color figure
		if (pCir->extensionDictionary().isNull()) {
			pCir->createExtensionDictionary();
		}
		AcDbDictionary* pXDict = nullptr;
		AcDbObjectId cirDict = pCir->extensionDictionary();
		acdbOpenAcDbObject((AcDbObject*&)pXDict, cirDict, AcDb::kForWrite);
		if (pXDict) {
			std::wstring strKey = std::to_wstring(i);
			const wchar_t* wcharStr = strKey.c_str();
			CStringW cstr(wcharStr);
			ACHAR* key = nullptr;
			key = cstr.GetBuffer();
			addIntDictionaryItem(pXDict, key, i);
		}

		//close extension dict and circle
		pXDict->close();
		pCir->close();
	}
	//close other figuration
	pBTR->close();
	pBT->close();
	pNOD->close();
	delete pDb;

	acutPrintf(_T("Circle Drew!"));
}

void copyEntity()
{
	ads_name en;
	AcGePoint3d apt(100, 100, 0);
	AcDbObjectId enID;
	acdbGetAdsName(en, enID);
	acedCommand(RTSTR, _T("_.Copy"), RTENAME, en, RT3DPOINT, asDblArray(apt), RTNONE);
}

void pickPoint()
{
	AcGePoint3d pt1, pt2;
	acedGetPoint(NULL, _T("\nStart: "), asDblArray(pt1));
	acedGetPoint(asDblArray(pt1), _T("\nEnd: "), asDblArray(pt2));
	acedGetCorner(asDblArray(pt1), _T("\nCorner Point: "), asDblArray(pt2));
}

AcDbLine* createLine(AcGePoint3d& startPt, AcGePoint3d endPt) {
	AcDbLine* pLine = new AcDbLine(startPt, endPt);
	startPt = endPt;
	return pLine;
}

AcDbLine* createLine(AcGePoint3d& startPt) {
	ads_real length;
	acedGetReal(_T("\nLength: "), &length);
	acutPrintf( _T("%.2lf"),length);

	ads_real ang;
	acedGetAngle(NULL, _T("\nAngle: "), &ang);
	acutPrintf(_T("%.2lf"), ang);

	double angleRad = ang * (3.14 / 180.0);
	AcGePoint3d endPt;
	endPt.x = startPt.x + length * cos(angleRad);
	endPt.y = startPt.y + length * sin(angleRad);
	endPt.z = startPt.z;

	AcDbLine* pLine = new AcDbLine(startPt, endPt);

	startPt = endPt;
	return pLine;
}

AcDbArc* createArc(AcGePoint3d& startPt) {
	AcGePoint3d pt2, pt3;
	acedGetPoint(asDblArray(startPt), _T("\nSpecify the second point: "), asDblArray(pt2));
	acedGetPoint(asDblArray(pt2), _T("\nSpecify the third point: "), asDblArray(pt3));
	AcGeCircArc3d arcGeom(startPt, pt2, pt3);
	AcDbArc* pArc = new AcDbArc(arcGeom.center(), arcGeom.normal(), arcGeom.radius(), arcGeom.startAng(), arcGeom.endAng());
	startPt = pt3;
	return pArc;
}

AcDbPolyline* createPolyLine(AcGePoint3d& startPt, int half) {
	AcGePoint3d endPt;
	acedGetPoint(asDblArray(startPt), _T("\nSpecify the end point: "), asDblArray(endPt));

	AcGePoint2d startPt2d(startPt.x, startPt.y);
	AcGePoint2d endPt2d(endPt.x, endPt.y);

	AcDbPolyline* pPLine = new AcDbPolyline();
	pPLine->addVertexAt(0, startPt2d);
	pPLine->addVertexAt(1, endPt2d);

	if (half == 1) {
		//1 for half-width
		pPLine->setWidthsAt(0, 2, 10);
		pPLine->setWidthsAt(1, 2, 100);
	}
	else {
		//0 for equal-width
		ads_real width;
		acedGetReal(_T("\nWidth: "), &width);
		acutPrintf(_T("%.2lf"), width);
		pPLine->setWidthsAt(0, width, width);
		pPLine->setWidthsAt(1, width, width);
	}

	startPt = endPt;
	return pPLine;
}

AcDbObjectId selectEntity()
{
	ads_name ent;
	ads_point pt;
	int rc = acedEntSel(_T("\nSelect an entity: "), ent, pt);
	if (rc != RTNORM) {
		acutPrintf(_T("\nUnavailable Entity Selected!"));
		return nullptr;
	}
	acutPrintf(_T("\nEntity selected!"));
	AcDbObjectId entID;
	acdbGetObjectId(entID, ent);
	return entID;
}

void myLine()
{
	//initialize all figuration needed
	AcDbBlockTable* pBT = getBlockTable();
	AcDbBlockTableRecord* pBTR = getBlockTableRecord(pBT);
	typedef struct preNode {
		AcDbObjectId id;
		AcGePoint3d startPt;
	}preNode;
	std::stack<preNode> preID;
	bool flag = 0;	//flag for erase,1 for yes,0 for no

	//get start point
	AcGePoint3d pt1;
	acedGetPoint(NULL, _T("\nSpecify the Start Point: "), asDblArray(pt1));

	//get command for next step
	AcGePoint3d pt;
	acedInitGet(RSG_NONULL, _T("A,a H,h L,l U,u W,w"));	//initialize keyword
	auto rc = acedGetPoint(NULL, _T("\nSpecify Next Point or [A/H/L/U/W]: "), asDblArray(pt));

	while (rc != RTCAN) {	//Esc to abort
		flag = 0;
		preNode prenode;
		prenode.startPt = pt1;
		if (rc == RTNORM) {
			//get another point occasion
			//draw the line
			auto pline = createLine(pt1, pt);
			AcDbObjectId lineID;
			pBTR->appendAcDbEntity(lineID, pline);
			prenode.id = lineID;
			pline->close();
		}
		else if (rc == RTKWORD) {
			//key word occasion
			AcString kword;
			acedGetInput(kword);
			acutPrintf(kword);

			//switch keyword
			if (kword == _T("A"))
			{
				//draw the arc
				auto pArc = createArc(pt1);
				AcDbObjectId arcID;
				pBTR->appendAcDbEntity(arcID, pArc);
				prenode.id = arcID;
				pArc->close();
			}
			else if (kword == _T("H"))
			{
				//half width
				auto pPline = createPolyLine(pt1, 1);	//1 for half-width
				AcDbObjectId plineID;
				pBTR->appendZcDbEntity(plineID, pPline);
				prenode.id = plineID;
				pPline->close();
			}
			else if (kword == _T("L")) {
				//draw line with length and angle
				auto pLine = createLine(pt1);
				AcDbObjectId lineID;
				pBTR->appendZcDbEntity(lineID, pLine);
				prenode.id = lineID;
				pLine->close();

			}
			else if (kword == _T("U")) {
				//erase last entity
				if (preID.empty())
				{
					acutPrintf(_T("\nNo More Entity to Erased!"));
				}
				else {
					flag = 1;
					AcDbEntity* pEn;
					acdbOpenAcDbEntity(pEn, preID.top().id, AcDb::kForWrite);
					pt1 = preID.top().startPt;	//back to last origin
					preID.pop();

					pEn->erase();
					pEn->close();
				}

			}
			else if (kword == _T("W")) {
				auto pPLine = createPolyLine(pt1, 0);	//0 for width
				AcDbObjectId plineID;
				pBTR->appendZcDbEntity(plineID, pPLine);
				prenode.id = plineID;
				pPLine->close();
			}
		}

		if (!flag) {	//push to stack only when no erase op
			//reserve last entity's start point for erase op
			preID.push(prenode);
		}

		//receive peremeter input
		acedInitGet(RSG_NONULL, _T("A,a H,h L,l U,u W,w"));
		rc = acedGetPoint(NULL, _T("\nSpecify Next Point or [A/H/L/U/W]: "), asDblArray(pt));
	}
	pBT->close();
	pBTR->close();
}

void drawCunstomEntity() {
	//initialize all figuration needed
	AcDbBlockTable* pBT = getBlockTable();
	AcDbBlockTableRecord* pBTR = getBlockTableRecord(pBT);
	CSampleCustEnt* cusEnt = new CSampleCustEnt();

	double radius = 0.0;
	AcGePoint3d center;

	//select circle
	ads_name cir;
	ads_point pt;
	int rc = acedEntSel(_T("\nSelect a Circle: "), cir, pt);
	if (rc != RTNORM) {
		acutPrintf(_T("\nUnavailable Entity Selected!"));
		return;
	}
	AcDbObjectId cirID;
	acdbGetObjectId(cirID, cir);

	AcDbObject* pobj = nullptr;
	acdbOpenAcDbObject(pobj, cirID, AcDb::kForRead);

	if (pobj->isKindOf(AcDbCircle::desc())) {
		AcDbCircle* pCir = AcDbCircle::cast(pobj);
		if (pCir != nullptr) {
			radius = pCir->radius();
			acutPrintf(_T("\nRadius Get: %f"), radius);
		}
	}
	else {
		acutPrintf(_T("\nUnavailable Entity Selected!"));
	}
	pobj->close();

	//select center
	acedGetPoint(NULL, _T("\nSpecify the Center Point: "), asDblArray(center));
	acutPrintf(_T("\nCenter: (%f, %f, %f)"), center.x, center.y, center.z);

	//modify custom entity
	cusEnt->setCenter(center);
	cusEnt->setRadius(radius);
	pBTR->appendZcDbEntity(cusEnt);
	acutPrintf(_T("\nCustom Entity Printed"));

	//close
	cusEnt->close();
	pBTR->close();
	pBT->close();

}

void myCircle() {
	CSampleCustEnt* cusEnt = new CSampleCustEnt();
	CCustEntJig* myJig = new CCustEntJig();
	myJig->startJig(cusEnt);
}

void registerEditorReactor()
{
	MyEditorReactor* pReactor = new MyEditorReactor();
	acedEditor->addReactor(pReactor);
}

void registerDBReactor()
{
	AcDbDatabase* pDb = getDatabase();
	MyDBReactor* pReactor = new MyDBReactor();
	pDb->addReactor(pReactor);
}

void lineReactor()
{
	ObjectToNotify *pObj = new ObjectToNotify();
	pObj->setFactor(2.0);

	//attach b line to respond
	acutPrintf(_T("\nSelect a line to react: "));
	AcDbObjectId bID = selectEntity();
	pObj->link(bID);
	acutPrintf(_T("\nLine attached."));

	//store reactor to dict
	AcDbObjectId objID;
	AcDbDictionary* pNOD = getNameDictionary();
	Acad::ErrorStatus es = pNOD->setAt(_T("object_to_notify_A"), pObj, objID);
	if (es != Acad::eOk) {
		acutPrintf(_T("\nError storing reactor to dictionary."));
		pNOD->close();
		delete pObj;
		return;
	}
	acutPrintf(_T("\nPermanent reactor stored in dictionary."));

	//add reactor to a line
	acutPrintf(_T("\nSelect a line to regisetr the reactor: "));
	AcDbLine* pLine = nullptr;
	AcDbObjectId aID = selectEntity();
	acdbOpenObject((AcDbObject*&)pLine, aID, AcDb::kForWrite);
	pLine->addPersistentReactor(objID);
	acutPrintf(_T("\nReactor added."));

	//close
	pObj->close();
	pLine->close();
	pNOD->close();
}

