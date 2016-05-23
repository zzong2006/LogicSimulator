#pragma once
class LogicObject
{
protected :
	POINT top, bottom; // left top / bottom
	POINT labelPos;
	int width, height;
	CString label, type;

	POINT inputCoord[2], outputCoord;
	bool input[2], output;
	int inputNum;			//�Է¼� ����
	int direct;				//����Ʈ ����

public:
	virtual void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p) = 0;
	virtual void draw_main(Gdiplus::Graphics* gp) = 0;
	virtual void turn() = 0;
	virtual void set_Coord_From_outC(int x, int y) = 0;
	void setLabel(CString input);				//�� �̸�
	void set_outputCoord(int x, int y);
	void set_inputCoord(int x, int y);
	void toggleOutput();
	POINT get_top() const;
	POINT get_bottm() const;

	LogicObject();
	virtual ~LogicObject();
};

