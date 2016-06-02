#pragma once
#include "LineObject.h"

#define INPUT_SIZE 2

#define UNIT 10

class LogicObject
{
protected:
	POINT top, bottom; // left top / bottom
	POINT labelPos;
	int width, height;

	bool input[2];
	int direct;				//����Ʈ ����
				

public:
	CString label, facing;
	int objectType, objectName;

	int inputNum, outputNum;			//�Է¼� ����, ��¼� ����
	bool output, isSelected;

	BOOL chk;			//�˰��� ���� �湮�ߴ°�?

	//�׸��� �׸���
	virtual void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p) = 0;	
	//������ �׸���
	virtual void draw_main(Gdiplus::Graphics* gp) = 0;		
	//��ü ���� �����Ͽ� ������
	virtual void turn() = 0;						
	//��� ��ǥ�� �̿��Ͽ� ���� ��ü ��ü(top/bottom)�� ��ǥ�� ����.
	virtual void set_Coord_From_outC(int x, int y) = 0;	

	//��ü�� ���É�ٸ� �� �� �𼭸��� �׸� ǥ����.
	void showSelected(Gdiplus::Graphics* gp);
	//�� ǥ���ϱ�
	void showLabel(Gdiplus::Graphics* gp);
	//�� �̸� �����ϱ�
	void setLabel(CString input);
	void setFacing(CString input);
	void set_outputCoord(int x, int y);		//��� ��ǥ ����
	void set_inputCoord(int x, int y);		//�Է� ��ǥ ����
	
	void toggleOutput();		//��°� ġȯ 0->1 or 1->0
	POINT get_top() const;
	POINT get_bottm() const;
	BOOL getOutput() const { return output; } ;

	//���� ����� ������ ��� �湮�� �����ΰ��� üũ��
	int isInputSet() const;

	LineObject* input_line[2], *output_line;
	POINT inputCoord[2], outputCoord;

	LogicObject();
	virtual ~LogicObject();
};

