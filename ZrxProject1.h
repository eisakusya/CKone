// ZrxProject1.h : main header file for the ZrxProject1 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CZrxProject1App
// See ZrxProject1.cpp for the implementation of this class
//

class CZrxProject1App : public CWinApp
{
public:
	CZrxProject1App();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
