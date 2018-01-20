
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
	// используем только для формирования случайных размерностей обоих множеств координат грида

/*
int MyBigRndInt (int nSredn, int nMin, int nMax, double rProbaSredn, double rKoef);
	// rProbaSredn - стартовая вероятность для значения nSredn;
	// rKoef - коэффициент для уменьшения вероятности при переходе к след.знач.
int MyDimGridRnd ();
	// вызов предыдущего с подобранными коэффициентами для размерности грида
*/

class RndPairs {
private:
	CWordArray XXX, YYY;
public:
	void AddPair (int x, int y) { XXX.Add(x); YYY.Add(y); }
	bool GetAnswer (int& x, int& y);
};

#endif
