
// LogicSimulator.h : LogicSimulator ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.



// CLogicSimulatorApp:
// �� Ŭ������ ������ ���ؼ��� LogicSimulator.cpp�� �����Ͻʽÿ�.
//

class CLogicSimulatorApp : public CWinApp
{
public:
	CLogicSimulatorApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	
private:
	ULONG_PTR gdiplusToken;
};

extern CLogicSimulatorApp theApp;
