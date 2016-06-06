
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
	ON_COMMAND(ID_FILE_SAVE, &CLogicSimulatorDoc::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CLogicSimulatorDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, &CLogicSimulatorDoc::OnFileNew)
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
	int line_num, logic_num;

	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
		/*/////////////////////////////////////////////////////////저장 순서//////////////////////////////////////////////////////////
		1. 선 ( 개수 -> 정보)
		2. 오브젝트 (개수 -> 정보)
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		line_num = lines.size();
		logic_num = logicInfo.size();

		ar << line_num;
		ar << logic_num;

		//선 위치 1번 -> 2번
		for (int i = 0; i < line_num; i++)
		{
			ar << lines.at(i)->line[0] << lines.at(i)->line[1];
		}
		//타입 -> 이릅 -> 위치
		for (int i = 0; i < logic_num; i++)
		{
			LogicObject* tempLogic = logicInfo.at(i);
			CPoint find_pos;
			find_pos.x = tempLogic->get_bottm().x;
			find_pos.y = (tempLogic->get_top().y + tempLogic->get_bottm().y) / 2;
			ar << tempLogic->objectType << tempLogic->objectName << find_pos;
		}

	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		/*/////////////////////////////////////////////////////////로딩 순서//////////////////////////////////////////////////////////
		1. 선 ( 개수 -> 정보)
		2. 오브젝트 (개수 -> 정보)
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		ar >> line_num;
		ar >> logic_num;

		for (int i = 0; i < line_num; i++)
		{
			CPoint pos1, pos2;
			ar >> pos1 >> pos2;
			LineObject* templine = new LineObject(pos1, pos2);
			lines.push_back(templine);
		}

		for (int i = 0; i < logic_num; i++)
		{
			int objectType, objectName;
			CPoint find_pos;

			ar >> objectType >> objectName >> find_pos;

			Gate *Gtemp;
			FlipFlop *Ftemp;
			Pin *Ptemp;
			Clock *Ctemp;
			Out *Otemp;

			switch (objectType)
			{
			case GATE_TYPE:
				Gtemp = NULL;
				switch (objectName)
				{
				case AND_GATE:
					Gtemp = new andGate(find_pos.x, find_pos.y);
					break;
				case OR_GATE:
					Gtemp = new orGate(find_pos.x, find_pos.y);
					break;
				case NAND_GATE:
					Gtemp = new nandGate(find_pos.x, find_pos.y);
					break;
				case NOR_GATE:
					Gtemp = new norGate(find_pos.x, find_pos.y);
					break;
				case XOR_GATE:
					Gtemp = new xorGate(find_pos.x, find_pos.y);
					break;
				case NOT_GATE:
					Gtemp = new notGate(find_pos.x, find_pos.y);
					break;
				}

				if (Gtemp != NULL) {
					Gtemp->set_Coord_From_outC(find_pos.x, find_pos.y);
					logicInfo.push_back(Gtemp);
				}
				break;
			case FLIPFLOP_TYPE :
				Ftemp = NULL;
				switch (objectName)
				{
				case D_FF:
					Ftemp = new DFlipFlop(find_pos.x, find_pos.y);
					break;
				case JK_FF:
					break;
				case T_FF:
					break;
				}

				if (Ftemp != NULL) {
					Ftemp->set_Coord_From_outC(find_pos.x, find_pos.y);

					logicInfo.push_back(Ftemp);
				}
				break;
			case WIRING_TYPE :
				switch (objectName)
				{
				case PIN :
					Ptemp = NULL;
					Ptemp = new Pin(find_pos.x, find_pos.y);
					logicInfo.push_back(Ptemp);
					Ptemp->set_outputCoord(find_pos.x, find_pos.y);
					Ptemp->set_Coord_From_outC(find_pos.x, find_pos.y);
					break;
				case CLOCK :
					Ctemp = NULL;
					Ctemp = new Clock(find_pos.x, find_pos.y);
					Ctemp->set_outputCoord(find_pos.x, find_pos.y);
					Ctemp->set_Coord_From_outC(find_pos.x, find_pos.y);
					logicInfo.push_back(Ctemp);
					break;
				case OUTPIN :
					Otemp = NULL;
					Otemp = new Out(find_pos.x, find_pos.y);
					Otemp->set_outputCoord(find_pos.x, find_pos.y);
					Otemp->set_Coord_From_outC(find_pos.x, find_pos.y);
					logicInfo.push_back(Otemp);
					break;
				}
				break;
			}
		}
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
BOOL CLogicSimulatorDoc::IsInput(LogicObject* lo)
{
	return lo->objectName == PIN || lo->objectName == CLOCK;
}
BOOL CLogicSimulatorDoc::IsGate(LogicObject* lo)
{
	return lo->objectType == GATE_TYPE || lo->objectType == FLIPFLOP_TYPE;
}

BOOL CLogicSimulatorDoc::IsOutput(LogicObject* lo)
{
	return lo->objectType == OUTPIN;
}

void CLogicSimulatorDoc::CheckCircuit()
{
	std::queue <LineObject *> searchLine;
	LogicObject* curLogic;

	//초기화
	for (int i = 0; i < lines.size(); i++)
	{
		lines.at(i)->chk = 0;
		lines.at(i)->state = OFF_SIGNAL;
	}
	
	//게이트 ,라이브러리 박스, 출력핀, 플립플롭
	for (int i = 0; i < logicInfo.size(); i++)
	{
		curLogic = logicInfo.at(i);
		if (IsInput(curLogic))
		{
			CPoint pos = curLogic->outputCoord[0].first;
			int lin = lines.size();

			for (int j = 0; j < lin; j++)
			{
				LineObject* curline = lines.at(j);
				if (curline->line[0] == pos || curline->line[1] == pos)
				{
					curline->chk = 1;
					curline->state = curLogic->getOutput();
					searchLine.push(curline);
				}
			}
			curLogic->chk = 1;
		}
		else{
			curLogic->chk = 0;
			int out = curLogic->outputNum, in = curLogic->inputNum;

			//입력핀 초기화
			for (int i = 0; i < in; i++)
				curLogic->inputCoord[i].second = -1;			//input chk 초기화
		}
	}

	////////////////////출력 시작/////////////////////
	//입력 Pin/Clock 과 관련된 값만 받기
	//출력 Pin 같은 경우는 제외해야 한다.

	////////////////////돌고 돌아/////////////////////////
	while (!searchLine.empty())
	{
		//(초반) Pin에 연결된 선을 다 돌면서 검사한다.
		while (!searchLine.empty())
		{
			LineObject* temp_line = searchLine.front();
			searchLine.pop();
			
			/////////연결된 선 체크////////////////////////
			int lin = (int)lines.size();
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


			//게이트 ,라이브러리 박스, 출력핀, 플립플롭
			for (int i = 0; i < logicInfo.size(); i++)
			{
				LogicObject* curLogic = logicInfo.at(i);
				if (!IsInput(curLogic))
				{
					if (curLogic->chk == 0)
					{
						int ip = curLogic->inputNum;
						for (int j = 0; j < ip; j++)
						{
							if (curLogic->inputCoord[j].first == temp_line->line[0] || curLogic->inputCoord[j].first == temp_line->line[1])
							{
								curLogic->inputCoord[j].second = temp_line->state;
							}
						}

					}
				}
			}
			temp_line->chk = 1;
		}

		/////////////////게이트 출력선 체크//////////////////////////
		//순회 해야될 LogicObject-> gate, flipflop, Library Box 세가지. 로직 오브젝트의 입력 선이 모두 방문되었나? && 이미 방문하였는가?

		for (int i = 0; i < logicInfo.size(); i++)
		{
			LogicObject* curLogic = logicInfo.at(i);
			//Gate 방문
			if (IsGate(curLogic))
			{
				if (curLogic->chk == 0 && curLogic->isInputSet())
				{
					curLogic->chk = 1;
					curLogic->setOutput();

					//플립플롭 출력선은 두개
					for (int j = 0; j < curLogic->outputNum; j++)
					{
						CPoint gatpos = curLogic->outputCoord[j].first;

						int lin = (int)lines.size();
						for (int i = 0; i < lin; i++)
						{
							LineObject* curline = lines.at(i);
							if (curline->line[0] == gatpos || curline->line[1] == gatpos)
							{
								curline->state = curLogic->outputCoord[j].second;

								if (curline->chk != 1)
									searchLine.push(curline);
							}
						}
					}
				}
			}
		}
		
		//Outpin 방문 (제일 마지막)
		for (int i = 0; i < logicInfo.size(); i++)
		{
			LogicObject* curLogic = logicInfo.at(i);

			if (IsOutput(curLogic))
			{
				//Gate 방문
				if (curLogic->isInputSet() && curLogic->chk == 0)
				{
					curLogic->chk = 1;
					//curLogic->setOutput();
				}
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

	switch (temp.Act)
	{
	case NEW :
		if (temp.Type == LINE)
		{

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
		}
		else{
			temp.logicInfo.push_back(logicInfo.at(logicInfo.size() - 1));
			logicInfo.pop_back();
		}
		break;
	case DELETE :
		
		break;
	case MOVE :
		break;
	case COPY :
		break;
	case PASTE :
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

	switch (temp.Act)
	{
	case NEW:
		if (temp.Type == LINE)
		{
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
		}
		else{
			logicInfo.push_back(temp.logicInfo.at(0));
			temp.logicInfo.pop_back();
		}
		break;
	case DELETE:
		break;
	case MOVE:
		break;
	case COPY:
		break;
	case PASTE:
		break;
	}
	mUndo.AddHead(temp);
}

void CLogicSimulatorDoc::OnFileSave()
{
	CFileDialog dlg(FALSE, L"circ", L"default.circ", OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT, L"LogicSimulator Files (*.circ)|*.circ|All Files (*.*)|*.*|");

	dlg.m_ofn.lpstrTitle = L"파일을 저장하세요.";
	dlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

	if (dlg.DoModal() == IDOK) {
		CString Temp;
		Temp.Format(L"%s", dlg.GetPathName());

		HANDLE hFile;
		DWORD dwRead;
		DWORD dwWritten;

		//파일 저장 시작
		CString con;

		hFile = CreateFile(Temp, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		WriteFile(hFile, L"FWEFEW", sizeof(int), &dwRead, NULL);

		CloseHandle(hFile);

		//CString Temp;
		//Temp.Format(_T("%s"), dlg.GetPathName());

		CFile  file;
		// 파일을 쓰기모드로 연다.
		if (file.Open(Temp, CFile::modeCreate | CFile::modeWrite)) {
			// 정상적으로 열린 파일을 CArchive 에 저장하기 모드로 전달한다.
			CArchive ar(&file, CArchive::store);

			try {
				Serialize(ar);
			}
			catch (CFileException *fe) {
				// 예외가 발생하면 메세지박스를 통하여 사용자에게 알린다.
				fe->ReportError();
			}
			catch (CArchiveException *ae) {
				// 예외가 발생하면 메세지박스를 통하여 사용자에게 알린다.
				ae->ReportError();
			}
			// CArchive 를 닫는다.
			ar.Close();
			// 파일을 닫는다.
			file.Close();
		}
	}

}


void CLogicSimulatorDoc::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlg(FALSE, L"circ", L"default.circ", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, L"LogicSimulator Files (*.circ)|*.circ|All Files (*.*)|*.*|");
	dlg.m_ofn.lpstrTitle = L"파일을 불러오세요.";
	dlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

	if (dlg.DoModal() == IDOK)
	{
		CString Temp;
		Temp.Format(L"%s", dlg.GetPathName());
		clearAll();

		CFile  file;
		if (file.Open(Temp, CFile::modeRead | CFile::shareExclusive)) {
			// 정상적으로 열린 파일을 CArchive 에 저장하기 모드로 전달한다.
			CArchive ar(&file, CArchive::load | CArchive::bNoFlushOnDelete);

			try {
				Serialize(ar);
			}
			catch (CFileException *fe) {
				// 예외가 발생하면 메세지박스를 통하여 사용자에게 알린다.
				fe->ReportError();
			}
			catch (CArchiveException *ae) {
				// 예외가 발생하면 메세지박스를 통하여 사용자에게 알린다.
				ae->ReportError();
			}
			// CArchive 를 닫는다.
			ar.Close();
			// 파일을 닫는다.
			file.Close();
		}
	}

}


void CLogicSimulatorDoc::OnFileNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int Answer;
	Answer = AfxMessageBox(_T("저장 하시겠습니까?"), MB_YESNOCANCEL);
	switch (Answer)
	{
	case IDYES:
		OnFileSave();
		clearAll();
		break;
	case IDNO :
		clearAll();
		break;
	case IDCANCEL :
		break;
	}
}

void CLogicSimulatorDoc::clearAll()
{
	lines.clear();
	logicInfo.clear();
	mUndo.RemoveAll();
	mRedo.RemoveAll();
}