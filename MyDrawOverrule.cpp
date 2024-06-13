#include "pch.h"
#include "MyDrawOverrule.h"


MyDrawOverrule::MyDrawOverrule()
{
}
MyDrawOverrule::~MyDrawOverrule()
{
}

bool MyDrawOverrule::isApplicable(const AcRxObject * pOverruledSubject) const
{
	if (pOverruledSubject->isKindOf(AcDbCircle::desc())) {
		return true;
	}
	return false;
}

ACDB_PORT Adesk::Boolean MyDrawOverrule::worldDraw(AcGiDrawable * pSubject, AcGiWorldDraw * wd)
{
	AcDbCircle* pCircle = AcDbCircle::cast(pSubject);
	if (!pCircle) {
		return AcGiDrawableOverrule::worldDraw(pSubject, wd);
	}

	//obtain center and radius
	AcGePoint3d center = pCircle->center();
	double radius = pCircle->radius();

	// plot outside circle
	wd->subEntityTraits().setColor(1);
	wd->geometry().circle(center, radius, AcGeVector3d::kZAxis);

	// caculate cross end point
	double halfDiag = radius * sqrt(2) / 2;
	AcGePoint3d pt1(center.x - halfDiag, center.y - halfDiag, center.z);
	AcGePoint3d pt2(center.x + halfDiag, center.y + halfDiag, center.z);
	AcGePoint3d pt3(center.x - halfDiag, center.y + halfDiag, center.z);
	AcGePoint3d pt4(center.x + halfDiag, center.y - halfDiag, center.z);

	// plot cross
	wd->geometry().polyline(2, new AcGePoint3d[2]{ pt1, pt2 });
	wd->geometry().polyline(2, new AcGePoint3d[2]{ pt3, pt4 });

	return Adesk::kTrue;
}
