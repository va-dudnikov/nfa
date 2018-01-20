
#if !defined(_Little)
#define _Little

#include <fstream.h>
#include <iostream.h>
#include <iomanip.h>

#include "Matrix.h"
#include "Grid.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyLittleSubTask - 

class MyLittleSubTask {
private:	// ��������! ��� ��������� ���� ��������� ����
	MyMatrix* pMatrix;			// kein Destructor!
	MyXY yesX, yesY, noX, noY;	// �������� ������ � ������� ������� AddYesX � �.�.!
public:
	MyLittleSubTask (MyMatrix* pMatrix);
	MyLittleSubTask (MyMatrix* pMatrix, int x, int y);	
		// c ���������� �������� 1� ������ � yesX � yesY
	MyLittleSubTask (MyMatrix* pMatrix, int x, int y, int nScore);	
		// ��� � c �������������� �������������� ���������� nScore - ��. ��� ���� ����
	MyLittleSubTask (MyLittleSubTask& copy);
	virtual ~MyLittleSubTask () {}
	bool InitRandom (int& x, int& y);	// �������� �������� ��������� ������ � ������� � yesX � yesY
private:
	bool InitRandom () { int x,y; return InitRandom (x,y); }
	int CostX (int newX);	// ���������� ����������
	int CostY (int newY);	// "����" ��� �������, �������� ����������� � ����
		// ������� -1 - ����� �� ��������
public:
	bool ExtandX (int& newX, int& nScore);	// ���������� ����������
	bool ExtandY (int& newY, int& nScore);	// ��. ���. � ��������� �������
		// ������� ���������� - ����� ������� � "�������������"
		// ����� 2 ��. - ��������� 2 ����������; ������������� ������ �� �������� � yesX � yesY
private:
	bool CanExtandX ();
	bool CanExtandY ();
	// ��� ������ �� 28 ��� 08; �� ��������� ���� ����������
	/*
	void AddYesX (int newX) { yesX.SetNewTrue(newX); }
	void AddYesY (int newY) { yesY.SetNewTrue(newY); }
	int yesScore ()	{ return yesX.GetDim() * yesY.GetDim(); } // � CostX() � CostY() �� �������
	int yesScoreP() { return yesX.GetDim() + yesY.GetDim(); } // ����������
	int noScore  ()	{ return noX. GetDim() * noY. GetDim(); } // ����������
	MyLittleSubTask* MakeRight ();
		// ������ ������ ���������, ������� ���� ��� �����;
		// ������� NULL - ���� ��������� �� �������, �.�. ���� ��� �����;
		// � ���� ������ ���� ����� �������� ��������� �������:
	*/
	// ����� ��� ������ ������� � 29 ��� 08
private:	// ���o� ���e
	int nScore;
		// ����� �������, ��� ��� ������, ��� ����� -
		// � � ����� ������ ��� ����� ���������� ��� ����������� ������,
		// �.�. ������ ������������ ������������ yesX � yesY;
		// �� ����� �� �������� (��������, � ������������� 0.5) ����������� ��� noX � noY
private:
	void MakeScore ();	// ������ nScore �� ����� ���������
public:
	void SetScore (int nScore)  { this->nScore = nScore; }
		// ������������� ��� nScore ������������� 
		// (� ��� ������ ����������� � ����������-��������)
	int GetScore ()  { return nScore; }
	void AddYesX (int newX);
	void AddYesY (int newY);
	void AddNoX (int newX);
	void AddNoY (int newY);
	MyLittleSubTask* MakeRight ();
		// ������ ������ ���������, ������� ���� ��� �����;
		// ������� NULL - ���� ��������� �� �������, �.�. ���� ��� �����;
		// � ���� ������ ���� ����� �������� ��������� �������:
	MyGrid* MakeGrid ();	// �����������, ���� �������� �� ������ yesX � yesY
	friend ostream& operator<< (ostream& os, MyLittleSubTask& mlst);
};

bool operator< (MyLittleSubTask& mlst1, MyLittleSubTask& mlst2);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyArrayLittleSubTask - 

class MyArrayLittleSubTask : public CPtrArray {	// of MyLittleSubTask*
public:
	MyArrayLittleSubTask () : CPtrArray() {}
	virtual ~MyArrayLittleSubTask ();
	MyLittleSubTask* MyGet (int n) { return (MyLittleSubTask*)GetAt(n); }
	void MyAdd (MyLittleSubTask* pSTAdd); 
	//void MyAddPrim (MyLittleSubTask* pSTAdd); 
		// ��������� � ������� ���������� ������������ ����� ��-��� ("�������");
		// ������ - �������� ��������� > (��� MyLittleSubTask)
	MyLittleSubTask* ExtractFirst ();
		// ��������� ������; ���������� NULL, ���� ������� ���
	MyLittleSubTask* GetFirst ();
		// � ������� �� ����., ������ ���������� ������, �� �������� �; NULL, ���� ������� ���
	friend ostream& operator<< (ostream& os, MyArrayLittleSubTask& arrsubtask);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyLittleTask - 

class MyLittleTask : public MyArrayLittleSubTask {
private:
	MyMatrix* pMatrix;			// kein Destructor!
	MyArrayGrid* pArrGrid;     	// kein Destructor!
public:
	MyLittleTask (MyMatrix* pMatrix, MyArrayGrid* pArrGrid);
	virtual ~MyLittleTask () {}
	int Step ();
		// �������� ������� ��������� ���, ���� ������ ��������� �� ������;
		// �������� ��� ��� - �� �������� � ������� ���������� �������������;
		// �������:	0 - ��, ����� ����, �������� ��� � arrGrid;
		//			1 - ����� ���� ���, �� � arrGrid �� ��� ��� �������;
		//			2 - ������� ���������� (� ������� �� ������� ��������)
		//			3 - ������������� ������������ ���� ("��������")
		//			4 - ������ �������� ��� ����;
	void SpecialAddSubTask (int x, int y);
		// � �������� ����� ������ ��������� ��������� � ������������ (��������) �������
		// � ����������� ��������������
	friend ostream& operator<< (ostream& os, MyLittleTask& littletask);
};

#endif
