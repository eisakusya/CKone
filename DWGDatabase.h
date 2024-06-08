#pragma once
//method for database
auto createDatabase();
auto readDatabase();
void saveDatabase(AcDbDatabase* pDb);
auto getDatabase();

//method for dictionary
AcDbDictionary* getNameDictionary(AcDbDatabase* pDb);
void printNamedDcitionary(AcDbDatabase* pDb);
void createDictionary(AcDbDatabase* pDb);
void addTextDictionaryItem(AcDbDictionary* pDict, const ACHAR* key, const ACHAR* item);
void addIntDictionaryItem(AcDbDictionary * pDict, const ACHAR * key, int item);

//method for block
AcDbBlockTable* getBlockTable(AcDbDatabase* pDb);
AcDbBlockTableRecord* getBlockTableRecord(AcDbBlockTable* pBT);
AcDbObjectId createAnonymousBlock(AcDbBlockTable* pBT);
void showBlock(AcDbBlockTable* pBT, AcDbObjectId blockID);
void appendEntity(AcDbBlockTableRecord* pBTR);

//interaction method
AcDbLine * createLine(AcGePoint3d& startPt, AcGePoint3d endPt);
AcDbLine * createLine(AcGePoint3d & startPt);
AcDbArc * createArc(AcGePoint3d& startPt);
AcDbPolyline * createPolyLine(AcGePoint3d & startPt, int half);
AcDbObjectId selectEntity();

//test method
void addCircle();
void myCircle();

//homework method
//- 2nd homework
void draw255Circle();
//- 3rd homework
void myLine();
//- 4th and 6th homework
void drawCunstomEntity();
