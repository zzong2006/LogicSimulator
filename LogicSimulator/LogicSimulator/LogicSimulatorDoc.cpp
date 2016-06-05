
// LogicSimulatorDoc.cpp : CLogicSimulatorDoc 클래스의 구현
//

#include "stdafx.h"

// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "LogicSimulator.h"
#endif

#include "LogicSimulatorDoc.h"


#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLogicSimulatorDoc

IMPLEMENT_DYNCREATE(CLogicSimulatorDoc, CDocument)

BEGIN_MESSAGE_MAP(CLogicSimulatorDoc, CDocument)
END_MESSAGE_MAP()


// CLogicSimulatorDoc 생성/소멸

CLogicSimulatorDoc::CLogicSimulatorDoc()
{
	temp = NULL;
	clickMode = false;
	selectMode = true;
	simulateMode = false;
	CanBeDivided = false;
	isOnFocus = false;
}

CLogicSimulatorDoc::~CLogicSimulatorDoc()
{
	for (int i = 0; i < logicInfo.size(); i++)
		delete logicInfo.at(i);
	for (int i = 0; i < lines.size(); i++)
		delete lines.at(i);
	
}

BOOL CLogicSimulatorDoc::OnNewDocument()
{
	for (int i = 0; i < logicInfo.size(); i++)
		delete logicInfo.at(i);
	
	for (int i = 0; i < lines.size(); i++)
		delete lines.at(i);
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CLogicSimulatorDoc serialization

void CLogicSimulatorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CLogicSimulatorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CLogicSimulatorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CLogicSimulatorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CLogicSimulatorDoc 진단

#ifdef _DEBUG
void CLogicSimulatorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLogicSimulatorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLogicSimulatorDoc 명령


void CLogicSimulatorDoc::CheckCircuit()
{
	std::queue <LineObject *> searchLine;

	//초기화
	for (int i = 0; i < lines.size(); i++)
	{
		lines.at(i)->chk = 0;
		lines.at(i)->state = OFF_SIGNAL;
	}
	
	//게이트 ,라이브러리 박스, 출력핀, 플립플롭
	int lgn = gateInfo.size();
	for (int i = 0; i < lgn; i++)
	{
		LogicObject* curLogic = gateInfo.at(i);
		curLogic->chk = 0;
		int out = curLogic->outputNum, in = curLogic->inputNum;

		//출력핀 초기화
		for (int i = 0; i < 1; i++)
			curLogic->outputCoord[i].second = -1;
		//입력핀 초기화
		for (int i = 0; i < in; i++)
			curLogic->inputCoord[i].second = -1;			//input chk 초기화

	}

	////////////////////////////////////////////////////////////////출력 시작////////////////////////////////////////////////////////////
	//입력 Pin/Clock 과 관련된 값만 받기
	//출력 Pin 같은 경우는 제외해야 한다.
	int pn = pinInfo.size();
	for (int i = 0; i < pn; i++)
	{
		CPoint pinos = pinInfo.at(i)->outputCoord[0].first;
		int lin = lines.size();

		for (int j = 0; j < lin; j++)
		{
			LineObject* curline = lines.at(j);
			if (curline->line[0] == pinos || curline->line[1] == pinos)
			{
				curline->chk = 1;
				curline->state = pinInfo.at(i)->getOutput();
				searchLine.push(curline);
			}
		}
		pinInfo.at(i)->chk = 1;
	}

	for (int i = 0; i < clockInfo.size(); i++)
	{
		CPoint pinos = clockInfo.at(i)->outputCoord[0].first;
		int lin = lines.size();

		for (int j = 0; j < lin; j++)
		{
			LineObject* curline = lines.at(j);
			if (curline->line[0] == pinos || curline->line[1] == pinos)
			{
				curline->chk = 1;
				curline->state = clockInfo.at(i)->getOutput();
				searchLine.push(curline);
			}
		}
		clockInfo.at(i)->chk = 1;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////돌고 돌아///////////////////////////////////////////////////////////
	while (!searchLine.empty())
	{
		//(초반) Pin에 연결된 선을 다 돌면서 검사한다.
		while (!searchLine.empty())
		{
			LineObject* temp_line = searchLine.front();
			searchLine.pop();
			int lin = lines.size();

			/////////////////////////////////////////////////////연결된 선 체크////////////////////////////////////////////////////
			for (int i = 0; i < lin; i++)
			{
				LineObject* curline = lines.at(i);
				if (curline->chk == 0)
				{
					if (curline->line[0] == temp_line->line[0] || curline->line[1] == temp_line->line[0]
						|| curline->line[0] == temp_line->line[1] || curline->line[1] == temp_line->line[1])
					{
						curline->chk = 1;
						curline->state = temp_line->state;
						searchLine.push(curline);
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////게이트 입력선 체크///////////////////////////////////////////////////////
			int gan = gateInfo.size();
			for (int i = 0; i < gan; i++)
			{
				Gate* curgate = gateInfo.at(i);
				if (curgate->chk == 0)
				{
					int ip = curgate->inputNum;
					for (int j = 0; j < ip; j++)
					{
						if (curgate->inputCoord[j].first == temp_line->line[0] || curgate->inputCoord[j].first == temp_line->line[1])
						{
							curgate->inputCoord[j].second = temp_line->state;
						}
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			temp_line->chk = 1;
		}

		//////////////////////////////////////////////게이트 출력선 체크///////////////////////////////////////////////////////
		//순회 해야될 LogicObject-> gate, flipflop, Library Box 세가지. 로직 오브젝트의 입력 선이 모두 방문되었나? && 이미 방문하였는가?
		int gan = gateInfo.size();
		for (int i = 0; i < gan; i++)
		{
			Gate* temp_gate = gateInfo.at(i);
			//Gate 방문
			if (temp_gate->isInputSet() && temp_gate->chk == 0)
			{
				temp_gate->chk = 1;
				temp_gate->setOutput();
				CPoint gatpos = temp_gate->outputCoord[0].first;

				int lin = lines.size();
				for (int i = 0; i < lin; i++)
				{
					LineObject* curline = lines.at(i);
					if (curline->line[0] == gatpos || curline->line[1] == gatpos)
					{
							curline->state = temp_gate->outputCoord[0].second;

						if (curline->chk != 1)
							searchLine.push(curline);
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Outpin 방문 (제일 마지막)
		for (int i = 0; i < outInfo.size(); i++)
		{
			Out* temp_out = outInfo.at(i);
		
			if (temp_out->isInputSet() && !temp_out->chk)
			{
				temp_out->chk = 1;
				temp_out->setOutput();
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

BOOL CLogicSimulatorDoc::CanUndo()
{
	while (mUndo.GetCount() > MAX_UNDO)
	{
		mUndo.RemoveTail();
	}
	return (mUndo.GetCount() > 0);
}

void CLogicSimulatorDoc::Undo()
{
	Action temp;
	CPoint stp, edp;

	temp = mUndo.RemoveHead();
	int lkedn = temp.lineked_line.size();

	switch (temp.Type)
	{
	case LINE :

		for (int i = lkedn - 1; i > 0; i -= 2)
		{
			temp.lineked_line.at(i - 1)->line[1] = temp.lineked_line.at(i)->line[1];
			lines.pop_back();
		}

		for (int i = 0; i < 2; i++)
		{
			temp.lines.push_back(lines.at(lines.size() - 1));
			lines.pop_back();
		}

		break;
	case OBJECT :
		logicInfo.pop_back();
		break;
	}
	mRedo.AddHead(temp);
}

BOOL CLogicSimulatorDoc::CanRedo()
{
	while (mRedo.GetCount() > MAX_UNDO)
	{
		mRedo.RemoveTail();
	}
	return (mRedo.GetCount() > 0);
}

void CLogicSimulatorDoc::Redo()
{
	Action temp;
	CPoint stp, edp;

	temp = mRedo.RemoveHead();
	int lkedn = temp.lineked_line.size();

	switch (temp.Type)
	{
	case LINE : 
		for (int i = 1; i > -1; i--)
		{
			lines.push_back(temp.lines.at(i));
			temp.lines.pop_back();
		}

		for (int i = 0; i < lkedn; i += 2)
		{
			temp.lineked_line.at(i)->line[1] = temp.lineked_line.at(i + 1)->line[0];
			lines.push_back(temp.lineked_line.at(i + 1));
		}
		break;
	case OBJECT:
		logicInfo.push_back(temp.logicInfo.at(0));
		break;
	}
	mUndo.AddHead(temp);
}