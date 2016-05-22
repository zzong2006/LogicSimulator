#pragma once
#include <gdiplus.h>

class Gate
{

protected:
	POINT inputCoord[2],outputCoord;	
	bool input[2], output;
	int inputNum;			//�Է¼� ����
	int direct;				//����Ʈ ����
	CString label;

public:
	Gate();
	virtual void draw(Graphics* gp, Pen* p) = 0;
	virtual void turn() = 0;
	virtual bool output() = 0;	//��� ����

	void addLabel (CString input);				//�� �̸�
	void set_outputCoord(int x, int y);
	virtual ~Gate();

};

