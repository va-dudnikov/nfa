
#if !defined(_Utily)
#define _Utily

#include <fstream.h>
#include <iostream.h>
#include <iomanip.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Obmen (int& n1, int& n2);
void Obmen (double& r1, double& r2);
bool ObmenIf (int& n1, int& n2);	// ���������� n1>=n2; ������� true ���� ��� �����
bool Nulla (double r, double eps);
bool Nulla (double r);
double GetInfty ();
double GetMinusInfty ();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BoundsInt (int& n, int nMin, int nMax);
void Bounds (double& r, double rMin, double rMax);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MyError (int nCode);


#endif
