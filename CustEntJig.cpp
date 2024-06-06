//-----------------------------------------------------------------------------
#include "pch.h"
#include "CustEntJig.h"
#include "gepnt3d.h"
#include "SampleCustEnt.h"
#include "dbjig.h"

//-----------------------------------------------------------------------------
CCustEntJig::CCustEntJig () : AcEdJig (),
	mCurrentInputLevel(0), mpEntity(nullptr),mRadius(0.0) { }

CCustEntJig::~CCustEntJig () { }

//-----------------------------------------------------------------------------
AcEdJig::DragStatus CCustEntJig::startJig (CSampleCustEnt *pEntity) {
	//- Store the new entity pointer
	mpEntity = pEntity ;
	//- Setup each input prompt
	AcString inputPrompts[2] ={
		"\nPick point",
		"\nRadius"
	};
	//- Setup kwords for each input
	AcString kwords[2] ={
		"",
		""
	};

	bool appendOk =true ;
	AcEdJig::DragStatus status =AcEdJig::kNull ;
	//- Loop the number of inputs
	for ( mCurrentInputLevel = 0; mCurrentInputLevel < 2; mCurrentInputLevel++ ) {
		//- Add a new input point to the list of input points
		mInputPoints.append (AcGePoint3d ()) ;
		//- Set the input prompt
		setDispPrompt (inputPrompts [mCurrentInputLevel]) ;
		//- Setup the keywords required
		setKeywordList (kwords [mCurrentInputLevel]) ;

		bool quit =false ;
		//- Lets now do the input
		status =drag () ;
		if ( status != kNormal ) {
			//- If it's a keyword
			switch ( status ) {
				case kCancel: 
				case kNull:
					quit =true ;
					break ;

				case kKW1:
				case kKW2:
				case kKW3:
				case kKW4:
				case kKW5:
				case kKW6:
				case kKW7:
				case kKW8:
				case kKW9:
					//- Do something

					break ;
		  }
		} else {
			appendOk =true ;
		}

		//- If to finish
		if ( quit )
			break ;
	}

	//- If the input went well
	if ( appendOk )
		//- Append to the database
		append () ;
	else
		//- Clean up
		delete mpEntity  ;

	return (status) ;
}

//-----------------------------------------------------------------------------
//- Input sampler
AcEdJig::DragStatus CCustEntJig::sampler () {
	//- Setup the user input controls for each input
	AcEdJig::UserInputControls userInputControls[2] ={
		/*AcEdJig::UserInputControls::*/(AcEdJig::UserInputControls)0,
		/*AcEdJig::UserInputControls::*/(AcEdJig::UserInputControls)0
	};
	//- Setup the cursor type for each input
	AcEdJig::CursorType cursorType[2] ={
		/*AcEdJig::CursorType::*/(AcEdJig::CursorType)0,
		/*AcEdJig::CursorType::*/(AcEdJig::CursorType)0
	};
	//- Setup the user input controls for each sample
	setUserInputControls (userInputControls [mCurrentInputLevel]) ;
	setSpecialCursorType (cursorType [mCurrentInputLevel]) ;

	AcEdJig::DragStatus status =AcEdJig::kCancel ;
	//- Check the current input number to see which input to do
	switch (mCurrentInputLevel + 1) {
		case 1:
			// TODO : get an input here
			status =GetStartPoint () ;
			break;
		case 2:
			// TODO : get an input here
			status = GetNextPoint();
			break;
		default:
			break;
		}
	return (status) ;
}

//-----------------------------------------------------------------------------
//- Jigged entity update
Adesk::Boolean CCustEntJig::update () {
	//- Check the current input number to see which update to do
	switch (mCurrentInputLevel + 1) {
		case 1:
			// TODO : update your entity for this input
			//mpEntity->setCenter (mInputPoints [mCurrentInputLevel]) ;
			mpEntity->setCenter(mInputPoints[mCurrentInputLevel]);
			break;
		case 2:
			// TODO : update your entity for this input
			//mpEntity->setCenter (mInputPoints [mCurrentInputLevel]) ;
			mpEntity->setRadius(mInputPoints[mCurrentInputLevel].distanceTo(mpEntity->getCenter()));
			break;
		default:
			break;
		}

	return (updateDimData ()) ;
}

//-----------------------------------------------------------------------------
//- Jigged entity pointer return
AcDbEntity * CCustEntJig::entity () const {
	return ((AcDbEntity *)mpEntity) ;
}

//-----------------------------------------------------------------------------
//- Dynamic dimension data setup
AcDbDimDataPtrArray * CCustEntJig::dimData (const double dimScale) {
	mDimData.removeAll();
	//- SAMPLE CODE:
	if (mInputPoints.length() < 2) {
		return nullptr;
	}
	AcDbAlignedDimension *dim =new AcDbAlignedDimension () ;
	dim->setDatabaseDefaults () ;
	dim->setNormal (AcGeVector3d::kZAxis) ;
	dim->setElevation (0.0) ;
	dim->setHorizontalRotation (0.0) ;
	dim->setXLine1Point (mInputPoints[0]) ;
	dim->setXLine2Point (mInputPoints[1]) ;
	//- Get the dimPoint, first the midpoint
	AcGePoint3d dimPoint;
	dimPoint.set(mInputPoints[0].x + mInputPoints[1].x, mInputPoints[0].y + mInputPoints[1].y, mInputPoints[0].z + mInputPoints[1].z);
	// mInputPoints[0] + ((mInputPoints[1] - mInputPoints[0])/ 2.0) 
	//- Then the offset
	dim->setDimLinePoint (dimPoint) ;
	dim->setDimtad (1) ;

	AcDbDimData *dimData = new AcDbDimData (dim) ;
	//AppData *appData =new AppData (1, dimScale) ;
	//dimData.setAppData (appData) ;
	dimData->setDimFocal (true) ;
	dimData->setDimHideIfValueIsZero (true) ;
	dimData->setDimInvisible(false);

	//- Make sure it is editable TODO: 
	dimData->setDimEditable (true) ;
	mDimData.append (dimData) ;

	return (&mDimData) ;
}

//-----------------------------------------------------------------------------
//- Dynamic dimension data update
Acad::ErrorStatus CCustEntJig::setDimValue (const AcDbDimData *pDimData, const double dimValue) {
	Acad::ErrorStatus es =Acad::eOk ;

	//- SAMPLE CODE:
	//- Convert the const pointer to non const
	AcDbDimData *dimDataNC =const_cast<AcDbDimData *>(pDimData) ;
	int inputNumber =-1 ;
	//- Find the dim data being passed so we can determine the input number
	if ( mDimData.find (dimDataNC, inputNumber) ) {
		//- Now get the dimension
		AcDbDimension *pDim =(AcDbDimension *)dimDataNC->dimension () ;
		//- Check it's the type of dimension we want
		AcDbAlignedDimension *pAlnDim =AcDbAlignedDimension::cast (pDim) ;
		//- If ok
		if ( pAlnDim ) {
			//- Extract the dimensions as they are now
			AcGePoint3d dimStart =pAlnDim->xLine1Point () ;
			AcGePoint3d dimEnd =pAlnDim->xLine2Point () ;
			//- Lets get the new point entered by the user 
			AcGePoint3d dimEndNew =dimStart + (dimEnd - dimStart).normalize () * dimValue ;
			//- Finally set the end dim point
			pAlnDim->setXLine2Point (dimEndNew) ;
			//- Now update the jig data to reflect the dynamic dimension input
			mInputPoints [mCurrentInputLevel] =dimEndNew ;
			update();
		}
	}
	return (es) ;
}

//-----------------------------------------------------------------------------
//- Various helper functions
//- Dynamic dimdata update function
Adesk::Boolean CCustEntJig::updateDimData () {
	//- Check the dim data store for validity
	if ( mDimData.length () <= 0 )
		return (true) ;

	//- SAMPLE CODE :
	//- Extract the individual dimData
	AcDbDimData *dimData =mDimData [0] ;
	//- Now get the dimension
	AcDbDimension *pDim =(AcDbDimension *)dimData->dimension () ;
	//- Check it's the type of dimension we want
	AcDbAlignedDimension *pAlnDim =AcDbAlignedDimension::cast (pDim) ;
	//- If ok
	if ( pAlnDim ) {
		//- Check to see if it is required
		dimData->setDimInvisible(false);
		pAlnDim->setXLine1Point (mInputPoints[0]) ;
		pAlnDim->setXLine2Point (mInputPoints[1]) ;
		//- Get the dimPoint, first the midpoint
		AcGePoint3d dimPoint = mInputPoints[0] + ((mInputPoints[1] - mInputPoints[0]) / 2.0) ;
		//- Then the offset
		pAlnDim->setDimLinePoint (dimPoint) ;
	} 
	return (true) ;
}

//-----------------------------------------------------------------------------
//- Std input to get a point with no rubber band
AcEdJig::DragStatus CCustEntJig::GetStartPoint () {
	AcGePoint3d newPnt ;
	//- Get the point 
	AcEdJig::DragStatus status =acquirePoint (newPnt) ;
	//- If valid input
	if ( status == AcEdJig::kNormal ) {
		//- If there is no difference
		if ( newPnt.isEqualTo (mInputPoints [mCurrentInputLevel]) )
			return (AcEdJig::kNoChange) ;
		//- Otherwise update the point
		mInputPoints [mCurrentInputLevel] =newPnt ;
	}
	return (status) ;
}

//-----------------------------------------------------------------------------
//- Std input to get a point with rubber band from point
AcEdJig::DragStatus CCustEntJig::GetNextPoint () {
	AcGePoint3d oldPnt = mInputPoints[mCurrentInputLevel] ;
	AcGePoint3d newPnt ;
	//- Get the point 
	AcEdJig::DragStatus status = acquirePoint (newPnt, oldPnt) ;
	//- If valid input
	if ( status == AcEdJig::kNormal ) {
		//- If there is no difference
		if ( newPnt.isEqualTo (mInputPoints [mCurrentInputLevel]) )
			return (AcEdJig::kNoChange) ;
		//- Otherwise update the point
		mInputPoints [mCurrentInputLevel] =newPnt ;
	}
	return (status) ;
}

AcEdJig::DragStatus CCustEntJig::GetRadius()
{
	AcGePoint3d oldPnt = mInputPoints[mCurrentInputLevel];
	double r = 0.0;
	AcEdJig::DragStatus status = acquireDist(r, oldPnt);
	if (status == AcEdJig::kNormal) {
		mRadius = r;
	}
	return (status);
}

