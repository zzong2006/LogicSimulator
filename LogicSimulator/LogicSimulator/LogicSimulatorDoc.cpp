
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
	int line_num, gate_num, pin_num, out_num, clock_num;

	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
		/*/////////////////////////////////////////////////////////저장 순서//////////////////////////////////////////////////////////
		1. 선 ( 개수 -> 정보)
		2. 게이트 (개수 -> 정보)
		3. 입력핀 (개수 -> 정보)
		4. 출력핀 (개수 -> 정보)
		5. 클럭 (개수 -> 정보)
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		line_num = lines.size();
		gate_num = gateInfo.size();
		pin_num = pinInfo.size();
		out_num = outInfo.size();
		clock_num = clockInfo.size();

		ar << line_num;
		ar << gate_num;
		ar << pin_num;
		ar << out_num;
		ar << clock_num;

		//선 위치 1번 -> 2번
		for (int i = 0; i < line_num; i++)
		{
			ar << lines.at(i)->line[0] << lines.at(i)->line[1];
		}
		//타입 -> 이릅 -> 위치
		for (int i = 0; i < gate_num; i++)
		{
			Gate* tempgate = gateInfo.at(i);
			CPoint find_pos;
			find_pos.x = tempgate->get_bottm().x;
			find_pos.y = (tempgate->get_top().y + tempgate->get_bottm().y)/2;
			ar << tempgate->objectName << find_pos;
		}

		for (int i = 0; i < pin_num; i++)
		{
			Pin * tempin = pinInfo.at(i);
			CPoint find_pos;
			find_pos.x = tempin->get_bottm().x;
			find_pos.y = (tempin->get_top().y + tempin->get_bottm().y) / 2;
			ar<< find_pos;
		}

		for (int i = 0; i < out_num; i++)
		{
			Out * temout = outInfo.at(i);
			CPoint find_pos;
			find_pos.x = temout->get_bottm().x;
			find_pos.y = (temout->get_top().y + temout->get_bottm().y) / 2;
			ar<< find_pos;
		}

		for (int i = 0; i < clock_num; i++)
		{
			Clock * tempclock = clockInfo.at(i);
			CPoint find_pos;
			find_pos.x = tempclock->get_bottm().x;
			find_pos.y = (tempclock->get_top().y + tempclock->get_bottm().y) / 2;
			ar << find_pos;
		}

	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		/*/////////////////////////////////////////////////////////로딩 순서//////////////////////////////////////////////////////////
		1. 선 ( 개수 -> 정보)
		2. 게이트 (개수 -> 정보)
		3. 입력핀 (개수 -> 정보)
		4. 출력핀 (개수 -> 정보)
		5. 클럭 (개수 -> 정보)
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		ar >> line_num;
		ar >> gate_num;
		ar >> pin_num;
		ar >> out_num;
		ar >> clock_num;

		for (int i = 0; i < line_num; i++)
		{
			CPoint pos1, pos2;
			ar >> pos1 >> pos2;
			LineObject* templine = new LineObject(pos1, pos2);
			lines.push_back(templine);
		}

		for (int i = 0; i < gate_num; i++)
		{
			int oName;
			CPoint find_pos;
			Gate *Gtemp;
			Gtemp = NULL;
			ar >> oName >> find_pos;
			switch (oName)
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
				gateInfo.push_back(Gtemp);
			}
		}

		for (int i = 0; i < pin_num; i++)
		{
			CPoint find_pos;
			Pin *Ptemp;
			Ptemp = NULL;
			ar >> find_pos;
			Ptemp = new Pin(find_pos.x, find_pos.y);
			logicInfo.push_back(Ptemp);
			Ptemp->set_outputCoord(find_pos.x, find_pos.y);
			Ptemp->set_Coord_From_outC(find_pos.x, find_pos.y);
			pinInfo.push_back(Ptemp);
		}

		for (int i = 0; i < out_num; i++)
		{
			CPoint find_pos;
			Out *Otemp;
			Otemp = NULL;
			ar >> find_pos;
			Otemp = new Out(find_pos.x, find_pos.y);
			Otemp->set_outputCoord(find_pos.x, find_pos.y);
			Otemp->set_Coord_From_outC(find_pos.x, find_pos.y);
			logicInfo.push_back(Otemp);
			outInfo.push_back(Otemp);
		}

		for (int i = 0; i < clock_num; i++)
		{
			CPoint find_pos;
			Clock *Ctemp;
			Ctemp = NULL;
			ar >> find_pos;
			Ctemp = new Clock(find_pos.x, find_pos.y);
			Ctemp->set_outputCoord(find_pos.x, find_pos.y);
			Ctemp->set_Coord_From_outC(find_pos.x, find_pos.y);
			logicInfo.push_back(Ctemp);
			clockInfo.push_back(Ctemp);
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
	for (int i = 0; i < gateInfo.size(); i++)
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

	for (int i = 0; i < outInfo.size(); i++)
	{
		LogicObject* curLogic = outInfo.at(i);

		curLogic->chk = 0;

		int out = curLogic->outputNum, in = curLogic->inputNum;

		//출력핀 초기화
		for (int i = 0; i < out; i++)
			curLogic->outputCoord[i].second = -1;
		//입력핀 초기화
		for (int i = 0; i < in; i++)
			curLogic->inputCoord[i].second = -1;			//input chk 초기화
	}

	////////////////////출력 시작/////////////////////
	//입력 Pin/Clock 과 관련된 값만 받기
	//출력 Pin 같은 경우는 제외해야 한다.
	for (int i = 0; i < pinInfo.size(); i++)
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
	////////////////////돌고 돌아/////////////////////////
	while (!searchLine.empty())
	{
		//(초반) Pin에 연결된 선을 다 돌면서 검사한다.
		while (!searchLine.empty())
		{
			LineObject* temp_line = searchLine.front();
			searchLine.pop();
			

			//게이트 ,라이브러리 박스, 출력핀, 플립플롭

			//////////////////////////연결된 선 체크////////////////////////
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

			for (int i = 0; i < gateInfo.size(); i++)
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
					
			for (int i = 0; i < outInfo.size(); i++)
			{
				Out* curout = outInfo.at(i);
				if (curout->chk == 0)
				{
					int ip = curout->inputNum;
					for (int j = 0; j < ip; j++)
					{
						TRACE(L"일치한다\n");
						if (curout->inputCoord[j].first == temp_line->line[0] || curout->inputCoord[j].first == temp_line->line[1])
						{
							curout->inputCoord[j].second = temp_line->state;
						}
					}
				}
			}

			temp_line->chk = 1;
		}

		/////////////////게이트 출력선 체크//////////////////////////
		//순회 해야될 LogicObject-> gate, flipflop, Library Box 세가지. 로직 오브젝트의 입력 선이 모두 방문되었나? && 이미 방문하였는가?
		for (int i = 0; i < gateInfo.size(); i++)
		{
			Gate* temp_gate = gateInfo.at(i);
			//Gate 방문
			if (temp_gate->isInputSet() && temp_gate->chk == 0)
			{
				temp_gate->chk = 1;
				temp_gate->setOutput();
				CPoint gatpos = temp_gate->outputCoord[0].first;

				int lin = (int)lines.size();
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
		
		//Outpin 방문 (제일 마지막)
		for (int i = 0; i < outInfo.size(); i++)
		{
			Out* temp_out = outInfo.at(i);
			//Gate 방문
			if (temp_out->isInputSet() && temp_out->chk == 0)
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
