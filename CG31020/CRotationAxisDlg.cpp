// CRotationAxisDialog.cpp: 实现文件
//

#include "pch.h"
#include "CG31020.h"
#include "afxdialogex.h"
#include "CRotationAxisDlg.h"


// CRotationAxisDialog 对话框

IMPLEMENT_DYNAMIC(CRotationAxisDlg, CDialogEx)

CRotationAxisDlg::CRotationAxisDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ROTATION_AXIS, pParent)
	, x(0)
	, y(0)
	, z(0)
{

}

CRotationAxisDlg::~CRotationAxisDlg()
{
}

void CRotationAxisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, x);
	DDX_Text(pDX, IDC_EDIT_Y, y);
	DDX_Text(pDX, IDC_EDIT_Z, z);
}


BEGIN_MESSAGE_MAP(CRotationAxisDlg, CDialogEx)
END_MESSAGE_MAP()


// CRotationAxisDialog 消息处理程序
