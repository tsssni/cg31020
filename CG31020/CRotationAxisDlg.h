#pragma once
#include "afxdialogex.h"


// CRotationAxisDialog 对话框

class CRotationAxisDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRotationAxisDlg)

public:
	CRotationAxisDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRotationAxisDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROTATION_AXIS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double x;
	double y;
	double z;
};
