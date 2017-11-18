
#pragma once
#ifndef _DIALOGBOXES_H_
#define _DIALOGBOXES_H_

#include "Resource.h"
#include <string>
#include <vector>
#include "vec.h"

using namespace std;
//params
#define DX_PARAM_1 101
#define DX_PARAM_2 102
#define DX_PARAM_3 103
#define DX_PARAM_4 104
#define DX_PARAM_5 105
#define DX_PARAM_6 106

/*
	CommonInputDialog - base DialogBox class to inherit
 */

class CommonInputDialog : public CDialog
{
	DECLARE_DYNAMIC(CommonInputDialog)
	CString title;

public:
	enum { IDD = IDD_DIALOG2 };

	CommonInputDialog(CString title = "") : CDialog(IDD), title(title) {}
	virtual ~CommonInputDialog() {}
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);
};

/*
	SetXYZDialog
*/

class SetXYZDialog : public CommonInputDialog
{
	CEdit xEdit, yEdit, zEdit;

	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	SetXYZDialog(CString title = "Set X Y Z", int initialValue = 0) : CommonInputDialog(title), x(initialValue), y(initialValue), z(initialValue) {}
	virtual ~SetXYZDialog() {}

	float x, y, z;
};
/*
	OneInputDialog
*/
class OneInputDialog : public CommonInputDialog
{
	CEdit xEdit;
	CString name;

	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	OneInputDialog(CString name, CString title = "Set X") : CommonInputDialog(title), name(name), x(1.0) {}
	virtual ~OneInputDialog() {}

	float x;
};
/*

	SetViewVolumeDialog

*/
class SetViewVolumeDialog : public CommonInputDialog
{
private:
	CEdit lEdit, rEdit, bEdit, tEdit, nEdit, fEdit;

	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	SetViewVolumeDialog(CString title = "Set View Volume") : CommonInputDialog(title), left(-1), right(1), bottom(-1), top(1), zNear(-1), zFar(1) {}
	virtual ~SetViewVolumeDialog() {}

	float left, right, bottom, top, zNear, zFar;
};
/*

	SetFovyAspectDialog

*/
class SetFovyAspectDialog : public CommonInputDialog
{
private:
	CEdit fovyEdit, aEdit, nEdit, fEdit;

	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	SetFovyAspectDialog(CString title = "Set View Volume") : CommonInputDialog(title), fovy(30), aspect(1), zNear(1), zFar(100) {}
	virtual ~SetFovyAspectDialog() {}

	float fovy, aspect, zNear, zFar;
};

#endif