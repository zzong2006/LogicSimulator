
// LogicSimulatorDoc.h : CLogicSimulatorDoc 클래스의 인터페이스
//

#pragma once
#include <vector>
#include <queue>
#include "andGate.h"
#include "orGate.h"
#include "Pin.h"
#include "Clock.h"

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
	std::vector <Gate *> gateInfo;
	std::vector <Pin *> pinInfo;
	int objectType;
	int objectName;

	int gateNum;
	LogicObject* temp;
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
