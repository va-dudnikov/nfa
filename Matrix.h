
#if !defined(_Matrix)
#define _Matrix

#include <fstream>
#include <iostream>
#include <iomanip>

#include "Grid.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyMatrix - 

class MyMatrix {
private:
	int nDimX, nDimY;
	bool* Array;
	int nSumma;	
		// ��������� ����� ����������� ������; 
		// ��������� ���� ���������� ������ � InitMemory(), 
		// � ���������� - ������ � SetTrue() !!!
	int* costX;
	int* costY;	
		// ������ ����� ����� true � ������� � ��������;
		// ���������� ����������� - 
		// ��������� ���� ���������� ������ � InitMemory(), 
		// � ���������� - ������ � SetTrue() !!!
	// !!! ����� �����!!!
	// ��. ����� ������ "��������� �������������" -
	// �.�. ����� ���������� ��������, �������������� 1 � ����� ������;
	// ��� ����� ������������ ��� ������ ������������ �������� Extand -
	// �� ��� ���� �� ����� ������������ � ����������� ������������ ��������������
	// ��� �������� ���� "�������������"
public:
	MyMatrix (int nDimX, int nDimY);				// ������ ������������� ����� false
	MyMatrix (int nDimX, int nDimY, int kolGrid);	// ������ ������������� ���������� ����������
	MyMatrix (MyMatrix& copy);
	virtual ~MyMatrix ();
private:
	void InitMemory (int nDimX, int nDimY);
	int MakeIndex (int nX, int nY);
public:
	void SetTrue (int nX, int nY);
	bool Get (int nX, int nY);
	int GetDimX () { return nDimX; }
	int GetDimY () { return nDimY; }
	int GetSumma () { return nSumma; }
	int GetCostX (int nX) { return costX[nX]; }
	int GetCostY (int nY) { return costY[nY]; }
	//
	void AddGrid (MyGrid& grid);
	void AddRandomGrid ();
	//void InitRandom (int kolGrid);
 	//MyGrid* MakeGridRnd ();
		// ���������� ����� ���� - ��� NULL, ���� �� �������;
		// � ��� �������� ��. � ������, 
		// ��, �������, ���������� �� ��������� �� MyTwoMatrix::MakeGrid()
	MyGrid* MakeGridRnd (int nX, int nY);
		// � ����� ������ �� �� ����� �������� ��� ������������
		// ��� ����������� ����� �������� ��������� ������������� ���������� �����;
		// ��: ������ ��������� ������
private:
	// ��� ��������� ���������� ��������� ��� ���������� ����� ���������� - ������ ����������;
	// ��� -1, ���� ���������� �� ���� ���������� ���������
	int ExtandXRnd (MyGrid* pGrid);
	int ExtandYRnd (MyGrid* pGrid);
public:
	int HowManyNew (MyGrid& grid);
		// ������� � ���� ����� 1, �� �������� � ���� �������
	friend ostream& operator<< (ostream& os, MyMatrix& matrix);
};

int operator- (MyMatrix& matr1, MyMatrix& matr2);
	// ������� ����� 1 � matr1, �� �������� � matr2

#endif
