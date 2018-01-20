
#include "stdafx.h"
#include "SubSet.h"

#include "Utily.h"
#include "Random.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyXY - 

MyXY::MyXY (int nDim) {
	InitMemory(nDim);
}

MyXY::MyXY (int nDim, int nDimGrid) {
	InitMemory(nDim);
	InitRandom(nDimGrid);
}
	
MyXY::MyXY (MyXY& copy) {
	InitMemory(copy.GetDim());
	for (int i=0; i<nDim; i++)  if (copy.Get(i))  SetNewTrue(i);
}

MyXY::~MyXY () {
	delete[] Array;
}
	
void MyXY::InitMemory (int nDim) {
	this->nDim = nDim;  BoundsInt(this->nDim,1,maxDim);
	Array = new bool[nDim];
	for (int i=0; i<nDim; i++)  Array[i] = false;
	nDimGrid = 0;
}
	
void MyXY::InitRandom (int nDimGrid) {
	int nTmp=nDimGrid;  BoundsInt(nTmp,1,nDim);
	if (nTmp<=nDim/2) {	// небольшое число true
		for (int i=0; i<nTmp; i++) {
			for (;;) {
				int nom = Rnd(nDim);
				if (Array[nom]) continue;
				Array[nom] = true;
				break;
			}
		}
	}
	else {	// большое число true
		for (int i=0; i<nDim; i++) Array[i] = true;
		int nTmp1 = nDim - nTmp;
		for (i=0; i<nTmp1; i++) {
			for (;;) {
				int nom = Rnd(nDim);
				if (!Array[nom]) continue;
				Array[nom] = false;
				break;
			}
		}
	}
	MakeDimGrid();	// здесь обязательно!
	if (nTmp!=this->nDimGrid) MyError(2);
}

int MyXY::GetByNumber (int nom) {
	int j = -1;
	for (int i=0; i<nDim; i++) {
		if (!Array[i]) continue;
		if (++j>=nom) return i;
	}
	return -1;
}
	
int MyXY::MakeDimGrid () {
	nDimGrid = 0;
	for (int i=0; i<nDim; i++)  if (Array[i])  nDimGrid++;
	return nDimGrid;
}

bool operator<= (MyXY& xy1, MyXY& xy2) {
	int nDim = xy1.GetDim();  if (nDim != xy2.GetDim()) return false;
	if (xy1.GetDimGrid() > xy2.GetDimGrid()) return false;
	for (int i=0; i<nDim; i++)
		if (xy1.Get(i) && !xy2.Get(i)) return false;
	return true;
}

bool operator== (MyXY& xy1, MyXY& xy2) {
	int nDim = xy1.GetDim();  if (nDim != xy2.GetDim()) return false;
	if (xy1.GetDimGrid() != xy2.GetDimGrid()) return false;
	for (int i=0; i<nDim; i++)
		if (xy1.Get(i) != xy2.Get(i)) return false;
	return true;
}

ostream& operator<< (ostream& os, MyXY& xy) {
	for (int i=0; i<xy.nDim; i++) {
		bool b = xy.Get(i);
		os << (b ? '1' : '0');
	}
	return os;
}






/*	
MyXY::MyXY (int nDim) {
	this->nDim = nDim;  BoundsInt(this->nDim,1,maxDim);
	nDimGrid = 0;
	Array = new bool[nDim];
	for (int i=0; i<nDim; i++)  SetFalse(i);
}
	
MyXY::MyXY (MyXY& copy) {
	this->nDim = copy.nDim;  BoundsInt(this->nDim,1,maxDim);
	nDimGrid = copy.nDimGrid;
	Array = new bool[nDim];
	for (int i=0; i<nDim; i++)  if (copy.Get(i)) SetTrue(i); else SetFalse(i);
}
	
int MyXY::MakeDimGrid () {
	nDimGrid = 0;
	for (int i=0; i<nDim; i++)  if (Array[i])  nDimGrid++;
	return nDimGrid;
}

void MyXY::InitRandom () {
	int nTmp = MyDimGridRnd();  BoundsInt(nTmp,1,nDim);
	for (int i=0; i<nTmp; i++) {
		for (;;) {
			int nom = Rnd(nDim);
			if (Array[nom]) continue;
			SetTrue(nom);
			break;
		}
	}
	MakeDimGrid();
	if (nTmp!=nDimGrid) MyError(2);
}
	
void MyXY::InitTrueFalse (bool bInit) {
	nDimGrid = nDim;
	for (int i=0; i<nDim; i++)  
		if (bInit) SetTrue(i); else SetFalse(i);
}
		
bool MyXY::operator== (MyXY& other) {
	if (nDim!=other.nDim) return false;
	if (nDimGrid!=other.nDimGrid) return false;
	for (int i=0; i<nDim; i++)  
		if (Array[i]!=other.Array[i]) return false;
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSubSet -	
	
MySubSet::MySubSet (int nDim) : MyXY(nDim) {
	Inited = false;
}
	
bool* MySubSet::MakeNext () {
	if (!Inited) {
		Inited = true;
		nDimGrid = nDim;
		Init(nDim,nDim);
		return Array;
	}	// далее уже проинициализировано
	// сначала пытаемся сделать следующий вариант той же размерности
	for (int i=0; i<nDim-1; i++) {
		if (!Array[i] || Array[i+1]) continue;
		Array[i] = false;  Array[i+1] = true;
		int K = 0;
		for (int j=0; j<i; j++)  if (Array[j]) K++;
		Init(i,K);
		return Array;
	}	// если вышли из цикла, значит требуемую пару не нашли
	if (nDimGrid--<=0) return NULL;	
		// sic! 0-ю разм. по X "убииваем" в др.месте, в MAG::FormingGridY()
	Init(nDim,nDimGrid);
	return Array;
}
	
void MySubSet::Init (int n, int m) {
	for (int i=0; i<n; i++)  Array[i] = i<m;
}
	
bool MySubSet::operator== (MySubSet& other) {
	if (!Inited || !other.Inited || nDim!=other.nDim || nDimGrid!=other.nDimGrid) return false;
	for (int i=0; i<nDim; i++)  if (Array[i]!=other.Array[i]) return false;
	return true;
}

ostream& operator<< (ostream& os, MySubSet& ss) {
	if (!ss.Inited) os << "subset not initialized";
	else for (int i=0; i<ss.nDim; i++) os << (ss.Array[i] ? "1" : "0");
	return os;
}
*/