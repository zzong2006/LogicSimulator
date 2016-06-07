
// LogicSimulatorDoc.h : CLogicSimulatorDoc Ŭ������ �������̽�
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
protected: // serialization������ ��������ϴ�.
	CLogicSimulatorDoc();
	DECLARE_DYNCREATE(CLogicSimulatorDoc)

// Ư���Դϴ�.
public:
	CString selectedType;
	BOOL isSelected;
	BOOL clickMode;
	BOOL selectMode;
	BOOL simulateMode;
	BOOL moved;
	BOOL locked;			//�ʱ⿡ �����϶� �����ϼ� ���� ������..

	int objectType;
	int objectName;

	LibraryBox logicBox[2];		//logicBox[0] ���� �ڽ�
	LibraryBox* currBox;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents() ;
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CLogicSimulatorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// ������ �޽��� �� �Լ�
protected:
	void clearAll();
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	afx_msg void OnOutputGraph();
};
