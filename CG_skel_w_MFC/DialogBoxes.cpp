
#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "DialogBoxes.h"

#define DX_SET_X 101
#define DX_SET_Y 102
#define DX_SET_Z 103

/*
	CommonInputDialog - base DialogBox class to inherit
*/

IMPLEMENT_DYNAMIC(CommonInputDialog, CDialog)

BOOL CommonInputDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(title);

	return TRUE;
}

void CommonInputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/*
	SetXYZDialog
*/

vec3 SetXYZDialog::getInput()
{
	return vec3(x, y, z);
}

void SetXYZDialog::DoDataExchange(CDataExchange* pDX)
{
	CommonInputDialog::DoDataExchange(pDX);

	DDX_Text(pDX, DX_SET_X, x);
	DDX_Text(pDX, DX_SET_Y, y);
	DDX_Text(pDX, DX_SET_Z, z);
}

int SetXYZDialog::OnCreate(LPCREATESTRUCT lpcs)
{
	xEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(130, 70, 340, 90), this, DX_SET_X);

	yEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(130, 140, 340, 160), this, DX_SET_Y);

	zEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(130, 210, 340, 230), this, DX_SET_Z);

	return 0;
}

void SetXYZDialog::OnPaint()
{
	CPaintDC dc(this);
	dc.SetBkMode(TRANSPARENT);

	CRect x_rect(100, 72, 450, 90);
	dc.DrawText(CString("X "), -1, &x_rect, DT_SINGLELINE);

	CRect y_rect(100, 142, 450, 160);
	dc.DrawText(CString("Y "), -1, &y_rect, DT_SINGLELINE);

	CRect z_rect(100, 212, 450, 230);
	dc.DrawText(CString("Z "), -1, &z_rect, DT_SINGLELINE);

	xEdit.SetFocus();
}

BEGIN_MESSAGE_MAP(SetXYZDialog, CommonInputDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()