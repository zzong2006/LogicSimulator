
// LogicSimulatorDoc.h : CLogicSimulatorDoc Ŭ������ �������̽�
//

#pragma once
#include <vector>
#include <queue>
#include "andGate.h"
#include "orGate.h"
#include "notGate.h"
#include "xorGate.h"
#include "nandGate.h"
#include "norGate.h"
#include "Pin.h"
#include "Clock.h"
#include "Out.h"
#include "DFlipFlop.h"
#include "JKFlipFlop.h"
#include "TFlipFlop.h"
#include "Sevenseg.h"

class Action
{
public :
	Action(int type,int act)
	{
		Type = type;
		Act = act;
		if (type == LINE)
			line_num = 2;

	}
	Action()
	{

	}
	enum{
		line_add
	};
	int Act, Type;
	int line_num;


	std::vector <LineObject *> lineked_line;
	std::vector <LogicObject *> logicInfo;
	std::vector <Sevenseg *> segInfo;
	std::vector <LineObject *> lines;
};

class CLogicSimulatorDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CLogicSimulatorDoc();
	DECLARE_DYNCREATE(CLogicSimulatorDoc)

// Ư���Դϴ�.
public:
	CString selectedType;
	BOOL isSelected;
	BOOL CanBeDivided;		//�б� ���ɻ��¸� ���׶��
	BOOL clickMode;
	BOOL selectMode;
	BOOL simulateMode;
	BOOL isOnFocus;			//���� ���� �𼭸��� �׸�
	std::vector <LogicObject *> currObject;
	std::vector <LogicObject *> logicInfo;
	std::vector <LineObject *> lines;

	
	int objectType;
	int objectName;

	int gateNum;

	LogicObject* temp;

	// undo/redo
	CList <Action> mUndo;
	CList <Action> mRedo;

	BOOL IsInput(LogicObject* lo);
	BOOL IsGate(LogicObject* lo);
	BOOL IsOutput(LogicObject* lo);

	BOOL CanUndo();
	BOOL CanRedo();
	void Undo();
	void Redo();
// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
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
	void CheckCircuit();
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
};
