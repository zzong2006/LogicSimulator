
// LogicSimulatorDoc.h : CLogicSimulatorDoc 클래스의 인터페이스
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

class Action
{
public :
	Action(LineObject* line)
	{
		lines.push_back(line);
		Type = LINE;
	}
	Action(LineObject** line)
	{
		lines.push_back(line[0]);
		lines.push_back(line[1]);
		Type = LINE;
	}
	Action(LogicObject* clock)
	{
		logicInfo.push_back(clock);
		Type = OBJECT;
	}
	Action(int type)
	{
		if (type == LINE)
		{
			Type = type;
			line_num = 2;
		}
	}
	Action()
	{

	}
	enum{
		line_add
	};

	int Type;
	int line_num;
	std::vector <LineObject *> lineked_line;
	std::vector <LogicObject *> logicInfo;
	std::vector <LineObject *> lines;
};

class CLogicSimulatorDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CLogicSimulatorDoc();
	DECLARE_DYNCREATE(CLogicSimulatorDoc)

// 특성입니다.
public:
	CString selectedType;
	BOOL isSelected;
	BOOL CanBeDivided;		//분기 가능상태면 동그라미
	BOOL clickMode;
	BOOL selectMode;
	BOOL simulateMode;
	BOOL isOnFocus;			//선택 상태 모서리에 네모
	std::vector <LogicObject *> currObject;
	std::vector <LogicObject *> logicInfo;
	std::vector <LineObject *> lines;
	std::vector <Clock *> clockInfo;
	std::vector <FlipFlop *> FFInfo;
	std::vector <Gate *> gateInfo;
	std::vector <Pin *> pinInfo;
	std::vector <Out *> outInfo;
	
	int objectType;
	int objectName;

	int gateNum;

	LogicObject* temp;

	// undo/redo
	CList <Action> mUndo;
	CList <Action> mRedo;

	BOOL CanUndo();
	BOOL CanRedo();
	void Undo();
	void Redo();
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
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
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	void CheckCircuit();
};
