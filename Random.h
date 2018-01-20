
#if !defined(_Random)
#define _Random

#include <fstream.h>
#include <iostream.h>
#include <iomanip.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Rnd (int nMax);
double RndReal ();
bool RndLT (double r);
bool RndBool ();

int MyRndDimGrid (int nDim);
void MyRndDimPairGrid (int& xDim, int& yDim);
	// ���������� ������ ��� ������������ ��������� ������������ ����� �������� ��������� �����

/*
int MyBigRndInt (int nSredn, int nMin, int nMax, double rProbaSredn, double rKoef);
	// rProbaSredn - ��������� ����������� ��� �������� nSredn;
	// rKoef - ����������� ��� ���������� ����������� ��� �������� � ����.����.
int MyDimGridRnd ();
	// ����� ����������� � ������������ �������������� ��� ����������� �����
*/

class RndPairs {
private:
	CWordArray XXX, YYY;
public:
	void AddPair (int x, int y) { XXX.Add(x); YYY.Add(y); }
	bool GetAnswer (int& x, int& y);
};

#endif
