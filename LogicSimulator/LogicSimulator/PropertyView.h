#pragma once
#include "afxpropertygridctrl.h"
#include "LogicObject.h"


// CPropertyView 폼 뷰입니다.

class CPropertyView : public CFormView
{
	DECLARE_DYNCREATE(CPropertyView)

protected:
	CPropertyView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CPropertyView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPERTYVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	CMFCPropertyGridCtrl m_pGridInfo;
	void InitializePropGrid(LogicObject *);
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
};


