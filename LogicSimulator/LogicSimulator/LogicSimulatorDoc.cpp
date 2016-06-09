
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
	ON_COMMAND(ID_OUTPUT_GRAPH, &CLogicSimulatorDoc::OnOutputGraph)
END_MESSAGE_MAP()


// CLogicSimulatorDoc 생성/소멸

CLogicSimulatorDoc::CLogicSimulatorDoc()
{
	currBox = &logicBox[0];
	currBox->ID = MAIN_LIB;
	logicBox[1].ID = SUB_LIB;

	clickMode = false;
	selectMode = true;
	simulateMode = false;
	isSelected = false;
}

CLogicSimulatorDoc::~CLogicSimulatorDoc()
{
	
}

BOOL CLogicSimulatorDoc::OnNewDocument()
{


	if (!CDocument::OnNewDocument())
		return FALSE;

	for (int i = 0; i < currBox->logicInfo.size(); i++)
		delete currBox->logicInfo.at(i);

	for (int i = 0; i < currBox->lines.size(); i++)
		delete currBox->lines.at(i);

	CheckPoint();
	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}

BOOL CLogicSimulatorDoc::OnOpenDocument(LPCTSTR lpszPathName) {
	if (!CLogicSimulatorDoc::OnOpenDocument(lpszPathName))
		return FALSE;

	CheckPoint();
	return TRUE;
}


// CLogicSimulatorDoc serialization
void CLogicSimulatorDoc::Serialize(CArchive& ar)
{
	int line_num, logic_num;

	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
		/*/////////////////////저장 순서///////////////////////
		1. 선 ( 개수 -> 정보)
		2. 오브젝트 (개수 -> 정보)
*/
		line_num = (int)currBox->lines.size();
		logic_num = (int)currBox->logicInfo.size();

		ar << line_num;
		ar << logic_num;

		//선 위치 1번 -> 2번
		for (int i = 0; i < line_num; i++)
		{
			ar << currBox->lines.at(i)->line[0] << currBox->lines.at(i)->line[1];
		}
		//타입 -> 이름 -> 위치 -> 어딜 향하는지 (int)
		for (int i = 0; i < logic_num; i++)
		{
			LogicObject* tempLogic = currBox->logicInfo.at(i);
			CPoint find_pos;
			find_pos.x = tempLogic->get_bottm().x;
			find_pos.y = (tempLogic->get_top().y + tempLogic->get_bottm().y) / 2;
			ar << tempLogic->objectType << tempLogic->objectName << find_pos << currBox->logicInfo.at(i)->facing;
		}

	}
	else
	{
		//일단 초기화
		for (int i = 0; i < currBox->logicInfo.size(); i++)
			delete currBox->logicInfo.at(i);

		for (int i = 0; i < currBox->lines.size(); i++)
			delete currBox->lines.at(i);


		currBox->lines.clear();
		currBox->currObject.clear();
		currBox->logicInfo.clear();

		currBox->CanBeDivided = false;
		currBox->isOnFocus = false;
		currBox->NumOuput = currBox->NumInput = 0;

		for (int i = 0; i < 10; i++)
		{
			currBox->ConnInput[i] = currBox->ConnOutput[i] = FALSE;
		}
		// 초기화 종료

		// TODO: 여기에 로딩 코드를 추가합니다.
		/*/////////////////////////////////////////////////////////로딩 순서//////////////////////////////////////////////////////////
		1. 선 ( 개수 -> 정보)
		2. 오브젝트 (개수 -> 정보)
		///////////////*/

		ar >> line_num;
		ar >> logic_num;

		for (int i = 0; i < line_num; i++)
		{
			CPoint pos1, pos2;
			ar >> pos1 >> pos2;
			LineObject* templine = new LineObject(pos1, pos2);
			currBox->lines.push_back(templine);
		}

		for (int i = 0; i < logic_num; i++)
		{
			int objectType, objectName, facing;
			CPoint find_pos;

			ar >> objectType >> objectName >> find_pos >> facing;

			Gate *Gtemp;
			FlipFlop *Ftemp;
			Pin *Ptemp;
			Clock *Ctemp;
			Out *Otemp;
			Box *Btemp;
			Sevenseg *Stemp;

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
					currBox->logicInfo.push_back(Gtemp);
					Gtemp->setFacing(facing);
					Gtemp->set_Coord_ByFacing(facing);

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
					Ftemp = new JKFlipFlop(find_pos.x,find_pos.y);
					break;
				case T_FF:
					Ftemp = new TFlipFlop(find_pos.x, find_pos.y);
					break;
				}

				if (Ftemp != NULL) {
					Ftemp->set_Coord_From_outC(find_pos.x, find_pos.y);

					currBox->logicInfo.push_back(Ftemp);
					Ftemp->setFacing(facing);
					Ftemp->set_Coord_ByFacing(facing);
				}
				break;
			case WIRING_TYPE :
				switch (objectName)
				{
				case PIN :
					Ptemp = NULL;
					Ptemp = new Pin(find_pos.x, find_pos.y);
					currBox->logicInfo.push_back(Ptemp);
					Ptemp->setFacing(facing);
					Ptemp->set_outputCoord(find_pos.x, find_pos.y);
					Ptemp->set_Coord_From_outC(find_pos.x, find_pos.y);
					Ptemp->set_Coord_ByFacing(facing);
					break;
				case CLOCK :
					Ctemp = NULL;
					Ctemp = new Clock(find_pos.x, find_pos.y);
					Ctemp->set_outputCoord(find_pos.x, find_pos.y);
					Ctemp->setFacing(facing);
					Ctemp->set_Coord_From_outC(find_pos.x, find_pos.y);
					currBox->logicInfo.push_back(Ctemp);
					Ctemp->set_Coord_ByFacing(facing);
					break;
				case OUTPIN :
					Otemp = NULL;
					Otemp = new Out(find_pos.x, find_pos.y);
					Otemp->setFacing(facing);
					
					Otemp->set_Coord_From_outC(find_pos.x, find_pos.y);
					currBox->logicInfo.push_back(Otemp);
					Otemp->set_Coord_ByFacing(facing);
					break;
				case SEG7:
					Stemp = NULL;
					Stemp = new Sevenseg(find_pos.x, find_pos.y);
					Stemp->set_inputCoord(find_pos.x, find_pos.y);
					Stemp->setFacing(facing);
					
					Stemp->set_Coord_From_outC(find_pos.x, find_pos.y);
					currBox->logicInfo.push_back(Stemp);
					Stemp->set_Coord_ByFacing(facing);
					break;
				}
				break;
			case LIB:
				if (this->currBox->ID == SUB_LIB) {
					AfxMessageBox(L"Cannot Load Library Box in Sub Library Field !");
					this->clearAll();
					return;
				}
				Btemp = new Box(find_pos.x, find_pos.y, &(logicBox[1]));
				Btemp->set_inputCoord(find_pos.x, find_pos.y);
				Btemp->set_outputCoord(find_pos.x, find_pos.y);
				Btemp->set_Coord_From_outC(find_pos.x, find_pos.y);
				Btemp->setFacing(facing);
				Btemp->set_Coord_ByFacing(facing);
				currBox->logicInfo.push_back(Btemp);
				
				break;
			}
		}
	}
}

void CLogicSimulatorDoc::DeleteContents()
{

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
	CFileDialog dlg(TRUE, L"circ", L"default.circ", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, L"LogicSimulator Files (*.circ)|*.circ|All Files (*.*)|*.*|");
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
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	CCircuitView *CVCtrl = (CCircuitView *)(pFrame->m_wndSplitterMain.GetPane(0, 1));

	currBox->lines.clear();
	currBox->logicInfo.clear();

	currBox->CanBeDivided = false;
	currBox->isOnFocus = false;
	currBox->NumOuput = currBox->NumInput = 0;

	for (int i = 0; i < 10; i++)
	{
		currBox->ConnInput[i] = currBox->ConnOutput[i] = FALSE;
	}

	CVCtrl->Invalidate();
}


void CLogicSimulatorDoc::OnOutputGraph()
{
	Graph dlg;
	Graph *pDlg = new Graph;
	pDlg->Create(IDD_DIALOG1);
	pDlg->ShowWindow(SW_SHOW);

}
