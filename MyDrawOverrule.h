#ifndef MYDRAWOVERRULE_H
#define MYDRAWOVERRULE_H
#pragma once

#include "drawable.h"

class MyDrawOverrule :
	public AcGiDrawableOverrule
{
public:
	MyDrawOverrule();
	~MyDrawOverrule();
	virtual bool isApplicable(
		const AcRxObject* pOverruledSubject
	) const;
	virtual ACDB_PORT Adesk::Boolean worldDraw(
		AcGiDrawable* pSubject,
		AcGiWorldDraw * wd
	);

};

#endif