#pragma once


// Graph ��ȭ �����Դϴ�.

class Graph : public CDialogEx
{
	DECLARE_DYNAMIC(Graph)

public:
	int inputValue[10], outputValue[10];
	int previousInput, UpdateGraph = 0;

	POINT inputline[10][250];
	POINT outputline[10][250];
	int count = 0;

	Graph(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Graph();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
