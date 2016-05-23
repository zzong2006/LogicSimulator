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
	virtual void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p) = 0;
	virtual void draw_main(Gdiplus::Graphics* gp) = 0;
	virtual void turn() = 0;
	virtual bool calOutput() = 0;	//��� ����

	void addLabel (CString input);				//�� �̸�
	void set_outputCoord(int x, int y);
	void set_inputCoord(int x, int y);
	virtual ~Gate();

};

