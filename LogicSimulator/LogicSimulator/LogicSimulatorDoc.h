
// LogicSimulatorDoc.h : CLogicSimulatorDoc Ŭ������ �������̽�
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
	std::vector <Clock *> clockInfo;
	std::vector <Gate *> gateInfo;
	std::vector <Pin *> pinInfo;
	int objectType;
	int objectName;

	int gateNum;
	LogicObject* temp;
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
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	void CheckCircuit();
};
