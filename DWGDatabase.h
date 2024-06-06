#pragma once
//method for database
auto createDatabase();
auto readDatabase();
void saveDatabase(AcDbDatabase* pDb);
auto getDatabase();

//method for dictionary
AcDbDictionary* getNameDictionary();
void printNamedDcitionary();
void createDictionary();
void addTextDictionaryItem(AcDbDictionary* pDict, const ACHAR* key, const ACHAR* item);
void addIntDictionaryItem(AcDbDictionary * pDict, const ACHAR * key, int item);

//method for block
AcDbBlockTable* getBlockTable();
AcDbBlockTableRecord* getBlockTableRecord(AcDbBlockTable* pBT);
AcDbObjectId createAnonymousBlock();
void showBlock(AcDbBlockTable* pBT, AcDbObjectId blockID);
void appendEntity(AcDbBlockTableRecord* pBTR);

//interaction method
AcDbLine * createLine(AcGePoint3d& startPt, AcGePoint3d endPt);
AcDbLine * createLine(AcGePoint3d & startPt);
AcDbArc * createArc(AcGePoint3d& startPt);
AcDbPolyline * createPolyLine(AcGePoint3d & startPt, int half);
AcDbObjectId selectEntity();

//test method
void addLine();
void readSample();
void saveSample();
void addCircle();
void draw255Circle();
void copyEntity();
void pickPoint();
void myLine();

void drawCunstomEntity();

void myCircle();

void registerEditorReactor();

void registerDBReactor();

void lineReactor();
