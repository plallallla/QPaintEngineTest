// GdiPaintDLL.h: GdiPaintDLL DLL 的主标头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 主符号


// CGdiPaintDLLApp
// 有关此类实现的信息，请参阅 GdiPaintDLL.cpp
//

class CGdiPaintDLLApp : public CWinApp
{
public:
	CGdiPaintDLLApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
