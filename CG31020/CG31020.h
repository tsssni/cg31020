
// CG31020.h: CG31020 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号


// CCG31020App:
// 有关此类的实现，请参阅 CG31020.cpp
//

class CCG31020App : public CWinApp
{
public:
	CCG31020App() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCG31020App theApp;
