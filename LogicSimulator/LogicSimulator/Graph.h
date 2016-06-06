#pragma once


// Graph 대화 상자입니다.

class Graph : public CDialogEx
{
	DECLARE_DYNAMIC(Graph)

public:
	int inputValue[10], outputValue[10];
	int previousInput, UpdateGraph = 0;

	POINT inputline[10][250];
	POINT outputline[10][250];
	int count = 0;

	Graph(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Graph();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
