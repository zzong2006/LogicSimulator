
// LogicSimulatorDoc.h : CLogicSimulatorDoc 클래스의 인터페이스
//

#pragma once
#include <vector>
#include <queue>
#include "Out.h"
#include "DFlipFlop.h"
#include "JKFlipFlop.h"
#include "TFlipFlop.h"
#include "Sevenseg.h"
#include "LibraryBox.h"
#include "Box.h"
#include "MainFrm.h"
#include "CircuitView.h"
#include "undo.h"
#include "Graph.h"

class CLogicSimulatorDoc : public CDocument,public undo
{
protected: // serialization에서만 만들어집니다.
	CLogicSimulatorDoc();
	DECLARE_DYNCREATE(CLogicSimulatorDoc)

// 특성입니다.
public:
	CString selectedType;
	BOOL isSelected;
	BOOL clickMode;
	BOOL selectMode;
	BOOL simulateMode;
	BOOL moved;
	BOOL locked;			//초기에 움직일때 움직일수 없게 만들음..

	int objectType;
	int objectName;

	LibraryBox logicBox[2];		//logicBox[0] 메인 박스
	LibraryBox* currBox;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents() ;
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CLogicSimulatorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// 생성된 메시지 맵 함수
protected:
	void clearAll();
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	afx_msg void OnOutputGraph();
};
