
#include "stdafx.h"
#include "Random.h"
#include "Utily.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int nnn1 = 19217, nnn2 = 45213, nnn3 = 65293;	// старт для датчика

int nRnd;

int MyRandom () {
	nRnd *= nnn1;
	nRnd += nnn2;
	nRnd %= nnn3;
	return nRnd;
}

int Rnd (int nMax) {
	return MyRandom() % nMax;
}

double RndReal () { 
	return (MyRandom()+1.0) / double(nnn3); 
}

bool RndLT (double r) { 
	return RndReal() < r; 
}

bool RndBool () { 
	return RndLT(0.5); 
}

const double rOtnKol = 0.1;	
	// используется в следующих 2 функциях;
	// если относительная размерность меньше этого, то вероятность прдлж увеличиваться
	// а ещё используется для окончательного ответа - если нужное число шагов его не дали

int SimpleDimGrid (int nDim) {
	int nRet = int(rOtnKol*nDim);
	BoundsInt(nRet,1,nDim);
	return nRet;
}

int MyRndDimGrid (int nDim) {
	const int maxSteps = 100;			// то самое нужное число шагов (см. коммент. в SimpleDimGrid)
	double rStep = 1.0 / double(nDim);	// шаг увеличения вероятности
	double rProba = rStep;				// текущая вероятность (сейчас она же стартовая)
	int nSimpleDim = SimpleDimGrid(nDim);
	int nDimRet = 1;
	for (int nStep=0; nStep<maxSteps; nStep++) {
		if (RndLT(rProba)) return nDimRet;
		if (nDimRet<nSimpleDim) rProba += rStep; else rProba *= (1.0-2.0*rOtnKol);
		Bounds(rProba,0.01,0.5);
		if (++nDimRet<=nDim) continue;
		nDimRet = 1;
		rProba = rStep;
	}
	return nSimpleDim;
}

void MyRndDimPairGrid (int& xDim, int& yDim) {
	BoundsInt(xDim,1,100);  BoundsInt(yDim,1,100);
	if (RndBool()) {	// первой обрабатываем 1ю размерность
		int xDimMax = xDim;
		xDim = MyRndDimGrid(xDim);
		double r1 = double(xDim)/double(xDimMax);	
			// какую часть составляем от максимально возможной размерности
		double r2 = 0.5*(1.0-r1);
			// какую часть может составлять для другой размерности
		int yDimMax = int(r2*yDim);  BoundsInt(yDimMax,1,yDim);
		yDim = MyRndDimGrid(yDimMax);
	}
	else {	// первой обрабатываем 2ю размерность
		int yDimMax = yDim;
		yDim = MyRndDimGrid(yDim);
		double r1 = double(yDim)/double(yDimMax);	
		double r2 = 0.5*(1.0-r1);
		int xDimMax = int(r2*xDim);  BoundsInt(xDimMax,1,xDim);
		xDim = MyRndDimGrid(xDimMax);
	}
}

bool RndPairs::GetAnswer (int& x, int& y) {
	int nSize = XXX.GetSize();
	if (nSize<=0 || nSize!= YYY.GetSize()) return false;
	int i = Rnd(nSize);
	x = XXX[i];
	y = YYY[i];
	return true;
}

/* old version
int MyRndGrid (int nMax) {
	const double rProba = 0.25;	// см. алгоритм
	const double rDolya = 0.33;	// при умножении на  получается макс. возм. кол-во
	nMax *= rDolya;  if (nMax<=0) nMax = 1;
	int nRet = 1;
	for (;;) {
		if (RndLT(rProba)) return nRet;
		if (++nRet>nMax) nRet = 1;
	}
}
*/

/*
int MyBigRndInt (int nSredn, int nMin, int nMax, double rProbaSredn, double rKoef) {
	int nDelta = max(nSredn-nMin,nMax-nSredn);
	for (;;) {
		if (RndLT(rProbaSredn)) return nSredn;
		double rProba = rProbaSredn;
		for (int i=1; i<nDelta; i++) {
			rProba *= rKoef;
			int nSleva = nSredn-i;
			if (nSleva<nMin) continue;
			if (RndLT(rProba)) return nSleva;
			int nSprava = nSredn+i;
			if (nSprava>nMax) continue;
			if (RndLT(rProba)) return nSprava;
		}
	}
}

int MyDimGridRnd () {
	return MyBigRndInt(4,1,20,0.01,0.85);
}
*/
