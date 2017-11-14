
#pragma once
#ifndef _DIALOGBOXES_H_
#define _DIALOGBOXES_H_

#include "Resource.h"
#include <string>
#include <vector>
#include "vec.h"

using namespace std;

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
private:
	float x;
	float y;
	float z;
	CEdit xEdit;
	CEdit yEdit;
	CEdit zEdit;

	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	SetXYZDialog(CString title = "Set X Y Z") : CommonInputDialog(title), x(0.0), y(0.0), z(0.0) {}
	virtual ~SetXYZDialog() {}

	vec3 getInput();
};

#endif