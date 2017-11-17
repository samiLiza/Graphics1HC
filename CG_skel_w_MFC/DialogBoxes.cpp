
#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "DialogBoxes.h"

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

void SetXYZDialog::DoDataExchange(CDataExchange* pDX)
{
	CommonInputDialog::DoDataExchange(pDX);

	DDX_Text(pDX, DX_PARAM_1, x);
	DDX_Text(pDX, DX_PARAM_2, y);
	DDX_Text(pDX, DX_PARAM_3, z);
}

int SetXYZDialog::OnCreate(LPCREATESTRUCT lpcs)
{
	xEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(130, 70, 340, 90), this, DX_PARAM_1);

	yEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(130, 140, 340, 160), this, DX_PARAM_2);

	zEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(130, 210, 340, 230), this, DX_PARAM_3);

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

/*

SetViewVolumeDialog

*/

void SetViewVolumeDialog::DoDataExchange(CDataExchange * pDX)
{
	CommonInputDialog::DoDataExchange(pDX);

	DDX_Text(pDX, DX_PARAM_1, left);
	DDX_Text(pDX, DX_PARAM_2, right);
	DDX_Text(pDX, DX_PARAM_3, bottom);
	DDX_Text(pDX, DX_PARAM_4, top);
	DDX_Text(pDX, DX_PARAM_5, zNear);
	DDX_Text(pDX, DX_PARAM_6, zFar);
}

int SetViewVolumeDialog::OnCreate(LPCREATESTRUCT lpcs)
{
	lEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(100, 30, 250, 50), this, DX_PARAM_1);

	rEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(100, 70, 250, 90), this, DX_PARAM_2);

	bEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(100, 110, 250, 130), this, DX_PARAM_3);

	tEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(100, 150, 250, 170), this, DX_PARAM_4);

	nEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(100, 190, 250, 210), this, DX_PARAM_5);

	fEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(100, 230, 250, 250), this, DX_PARAM_6);

	return 0;
}

void SetViewVolumeDialog::OnPaint()
{
	CPaintDC dc(this);
	dc.SetBkMode(TRANSPARENT);

	CRect l_rect(10, 32, 450, 50);
	dc.DrawText("Left ", -1, &l_rect, DT_SINGLELINE);

	CRect r_rect(10, 72, 450, 90);
	dc.DrawText("Right ", -1, &r_rect, DT_SINGLELINE);

	CRect b_rect(10, 112, 450, 130);
	dc.DrawText("Bottom ", -1, &b_rect, DT_SINGLELINE);

	CRect t_rect(10, 152, 450, 170);
	dc.DrawText("Top ", -1, &t_rect, DT_SINGLELINE);

	CRect n_rect(10, 192, 450, 210);
	dc.DrawText("zNear ", -1, &n_rect, DT_SINGLELINE);

	CRect f_rect(10, 232, 450, 250);
	dc.DrawText("zFar ", -1, &f_rect, DT_SINGLELINE);

	lEdit.SetFocus();
}

BEGIN_MESSAGE_MAP(SetViewVolumeDialog, CommonInputDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/*
	SetFovyAspectDialog
*/

void SetFovyAspectDialog::DoDataExchange(CDataExchange * pDX)
{
	CommonInputDialog::DoDataExchange(pDX);

	DDX_Text(pDX, DX_PARAM_1, fovy);
	DDX_Text(pDX, DX_PARAM_2, aspect);
	DDX_Text(pDX, DX_PARAM_3, zNear);
	DDX_Text(pDX, DX_PARAM_4, zFar);
}

int SetFovyAspectDialog::OnCreate(LPCREATESTRUCT lpcs)
{
	fovyEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(100, 30, 250, 50), this, DX_PARAM_1);

	aEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(100, 70, 250, 90), this, DX_PARAM_2);

	nEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(100, 110, 250, 130), this, DX_PARAM_3);

	fEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(100, 150, 250, 170), this, DX_PARAM_4);

	return 0;
}

void SetFovyAspectDialog::OnPaint()
{
	CPaintDC dc(this);
	dc.SetBkMode(TRANSPARENT);

	CRect l_rect(10, 32, 450, 50);
	dc.DrawText("FOVY ", -1, &l_rect, DT_SINGLELINE);

	CRect r_rect(10, 72, 450, 90);
	dc.DrawText("Aspect ", -1, &r_rect, DT_SINGLELINE);

	CRect b_rect(10, 112, 450, 130);
	dc.DrawText("zNear ", -1, &b_rect, DT_SINGLELINE);

	CRect t_rect(10, 152, 450, 170);
	dc.DrawText("zFar ", -1, &t_rect, DT_SINGLELINE);

	fovyEdit.SetFocus();
}

BEGIN_MESSAGE_MAP(SetFovyAspectDialog, CommonInputDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/*
	OneInputDialog
*/

void OneInputDialog::DoDataExchange(CDataExchange * pDX)
{
	CommonInputDialog::DoDataExchange(pDX);

	DDX_Text(pDX, DX_PARAM_1, x);
}

int OneInputDialog::OnCreate(LPCREATESTRUCT lpcs)
{
	xEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(100, 30, 250, 50), this, DX_PARAM_1);

	return 0;
}

void OneInputDialog::OnPaint()
{
	CPaintDC dc(this);
	dc.SetBkMode(TRANSPARENT);

	CRect l_rect(10, 32, 450, 100);
	dc.DrawText(name, -1, &l_rect, DT_SINGLELINE);

	xEdit.SetFocus();
}

BEGIN_MESSAGE_MAP(OneInputDialog, CommonInputDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()
