// Graph.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogicSimulator.h"
#include "Graph.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "LogicSimulatorDoc.h"

// Graph 대화 상자입니다.

IMPLEMENT_DYNAMIC(Graph, CDialogEx)

Graph::Graph(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	

}

Graph::~Graph()
{
}

void Graph::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Graph, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &Graph::OnBnClickedOk)
END_MESSAGE_MAP()


// Graph 메시지 처리기입니다.


void Graph::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CLogicSimulatorDoc* pDoc = (CLogicSimulatorDoc *)pFrame->GetActiveDocument();
	dc.SetBkMode(TRANSPARENT);

	if (previousInput < 0)
		previousInput = pDoc->currBox->NumInput;
	else if (previousInput != pDoc->currBox->NumInput) {
		UpdateGraph = 1;
		previousInput = pDoc->currBox->NumInput;
	}
		
	dc.DrawText(_T("입력핀"), CRect(0, 0, 50, 50), DT_LEFT);
	int input = 0 , curr =20;
	for (int i = 0; i < pDoc->currBox->logicInfo.size(); i++)
	{
		CString Fornumber;

		if (pDoc->currBox->logicInfo.at(i)->objectName == PIN) {
			Pin* PT = (Pin*)pDoc->currBox->logicInfo.at(i);
			Fornumber.Format(_T("ID : %d, Label : %s"), PT->getConNum(), PT->label);
			if (PT->getConNum() >= 0)
				inputValue[PT->getConNum()] = PT->getOutput();

			dc.TextOutW(0, curr, Fornumber);
			curr += 50;
		}
	}
	
	dc.TextOutW(0, curr, _T("출력핀"));
	curr += 20;
	for (int i = 0; i < pDoc->currBox->logicInfo.size(); i++)
	{
		CString Fornumber;

		if (pDoc->currBox->logicInfo.at(i)->objectName == OUTPIN) {
			Out* PT = (Out*)pDoc->currBox->logicInfo.at(i);
			Fornumber.Format(_T("ID : %d, Label : %s"), PT->getConNum(), PT->label);
			if (PT->getConNum() >= 0)
				outputValue[PT->getConNum()] = PT->getOutput();

			dc.TextOutW(0, curr, Fornumber);
			curr += 50;
		}
	}

	int i ,final_i;
	CPen pen;
	pen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));    // 빨간색 펜 생성
	dc.SelectObject(&pen);

	for (i = 0; i < pDoc->currBox->NumInput; i++)
	{
		for (int j = 1; j < (count < 200 ? count : 200); j++)
		{
			inputline[i][j - 1].x += 5;
		}
		for (int j = (count < 200 ? count : 200); j >= 1; j--)
		{
			inputline[i][j].x = inputline[i][j - 1].x;
			inputline[i][j].y = inputline[i][j - 1].y;
		}
		//그리기
		if (inputValue[i])
		{
			inputline[i][0].x = 0;
			inputline[i][0].y = 45 + 50 * i;
		}
		else {
			inputline[i][0].x = 0;
			inputline[i][0].y = 60 + 50 * i;
		}

		dc.Polyline(inputline[i], (count < 200 ? count : 200));
		
	}

	final_i = i;

	if (UpdateGraph) {
		for (i = 0; i < 10; i++)
		{
			for (int j = 0; j < 250; j++)
			{
				outputline[i][j].y = 80 + 50 * (i + final_i);
			}
		}
		UpdateGraph = false;
	}
	pen.DeleteObject();


	pen.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));  // 파란색 펜 생성
	dc.SelectObject(&pen);

	for (i = 0; i < pDoc->currBox->NumOuput; i++)
	{
		for (int j = 1; j < (count < 200 ? count : 200); j++)
		{
			outputline[i][j - 1].x += 5;
		}
		for (int j = (count < 200 ? count : 200); j >= 1; j--)
		{
			outputline[i][j].x = outputline[i][j - 1].x;
			outputline[i][j].y = outputline[i][j - 1].y;
		}
		//그리기

		if (outputValue[i])
		{
			outputline[i][0].x = 0;
			outputline[i][0].y = 65 + 50 * (i + final_i);
		}
		else {
			outputline[i][0].x = 0;
			outputline[i][0].y = 80 + 50 * (i + final_i);
		}
		dc.Polyline(outputline[i], (count < 200 ? count : 200));
		
	}
	//dc.Rectangle(10, 10, 10, 100);
}


void Graph::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	count++;
	Invalidate();
	CDialogEx::OnTimer(nIDEvent);
}


BOOL Graph::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int i = 0;
	for (i = 0; i < 10; i++)
	{
		for (int j = 0; j < 250; j++)
		{
			inputline[i][j].x = 0;
			inputline[i][j].y = 60 + 50 * i;
		}
	}
	for (i = 0; i < 10; i++)
	{
		for (int j = 0; j < 250; j++)
		{
			outputline[i][j].x = 0;
			outputline[i][j].y = 60 + 50 * (i + 9);
		}
	}
	this->SetTimer(0, 1000, NULL);

	previousInput = -1;
	UpdateGraph = 1;

	return TRUE;  // return TRUE unless you set the focus to a control
					  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void Graph::OnBnClickedOk()
{
	KillTimer(0);
	CDialogEx::OnOK();
}
