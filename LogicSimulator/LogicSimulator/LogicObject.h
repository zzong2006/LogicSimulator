#pragma once
#include "LineObject.h"

using namespace std;
#define INPUT_SIZE 2

#define UNIT 10

class LogicObject
{
protected:
	POINT top, bottom; // left top / bottom
	POINT labelPos;
	int width, height;

	//bool input[2];
	int direct;				//����Ʈ ����
				

public:
	CString label;
	int facing;
	int objectType, objectName;

	int inputNum, outputNum;			//�Է¼� ����, ��¼� ����
	bool output, isSelected;

	int state;
	int cycle;			//�ֱ� 1000�� 1����. (�ʱⰪ 1000)
	int oriCycle;		//������ �ֱ� ����Ŭ (cycle�� ��� ���鼭 üũ)

	int chk;			//�˰��� ���� �湮�ߴ°�?

	//�׸��� �׸���
	virtual void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p) = 0;	
	//������ �׸���
	virtual void draw_main(Gdiplus::Graphics* gp) = 0;				
	//��� ��ǥ�� �̿��Ͽ� ���� ��ü ��ü(top/bottom)�� ��ǥ�� ����.
	virtual void set_Coord_From_outC(int x, int y) = 0;	

	//����� ���� ǥ�����ش�.
	void drawLine(Gdiplus::Graphics* gp);
	//��ü�� ���É�ٸ� �� �� �𼭸��� �׸� ǥ����.
	void showSelected(Gdiplus::Graphics* gp);
	//�� ǥ���ϱ�
	void showLabel(Gdiplus::Graphics* gp);
	//�� �̸� �����ϱ�
	void setLabel(CString input);
	//�ٶ󺸴��� �����ϱ�
	void setFacing(CString input);
	//�ٶ󺸴��ʿ� ���� input/output��ǥ ����
	void set_Coord_ByFacing(CString input);
	//��ǥ �����Ҷ� ��ũ�� ��ǥ�迡 �°� ����
	void move_Coord(long & x, long & change);

	void setOutput();


	void set_outputCoord(int x, int y);		//��� ��ǥ ����
	void set_inputCoord(int x, int y);		//�Է� ��ǥ ����
	
	void toggleOutput();		//��°� ġȯ 0->1 or 1->0
	POINT get_top() const;
	POINT get_bottm() const;
	BOOL getOutput() const { return output; } ;

	//���� ����� ������ ��� �湮�� �����ΰ��� üũ��
	int isInputSet() const;
	//POINT inputCoord[10], outputCoord;
	pair<CPoint, int> inputCoord[10] , outputCoord[10];

	LogicObject();
	virtual ~LogicObject();
};

