
#include "stdafx.h"
#include "Matrix.h"
#include "Utily.h"
#include "Random.h"
#include "SubSet.h"
//#include "Arrays.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyMatrix - 
	
MyMatrix::MyMatrix (int nDimX, int nDimY) {
	InitMemory(nDimX,nDimY);
}

MyMatrix::MyMatrix (int nDimX, int nDimY, int kolGrid) {
	InitMemory(nDimX,nDimY);
	for (int i=0; i<kolGrid; i++) AddRandomGrid();
}

MyMatrix::MyMatrix (MyMatrix& copy) {
	InitMemory(copy.GetDimX(),copy.GetDimY());
	for (int i=0; i<nDimX; i++) for (int j=0; j<nDimY; j++) 
		if (copy.Get(i,j)) SetTrue(i,j);
	// про "nSumma" см. комментарии в h-файле
}

MyMatrix::~MyMatrix () {
	delete[] Array;
	delete[] costX;
	delete[] costY;
}
	
void MyMatrix::InitMemory (int nDimX, int nDimY) {
	this->nDimX = nDimX;  BoundsInt(this->nDimX,1,maxDim);
	this->nDimY = nDimY;  BoundsInt(this->nDimY,1,maxDim);
	Array = new bool[nDimX*nDimY];
	for (int i=0; i<nDimX*nDimY; i++)  Array[i] = false;
	nSumma = 0;
	costX = new int[nDimX];		for (i=0; i<nDimX; i++)  costX[i] = 0;
	costY = new int[nDimY];		for (i=0; i<nDimY; i++)  costY[i] = 0;
}

int MyMatrix::MakeIndex (int nX, int nY) {
	if (nX<0 || nX>=nDimX || nY<0 || nY>=nDimY) MyError(1);
	return nX*nDimY + nY;
}

void MyMatrix::SetTrue (int nX, int nY) {
	int nIndex = MakeIndex(nX,nY);
	if (Array[nIndex]) return;
	Array[nIndex] = true;
	nSumma++;
	costX[nX]++;
	costY[nY]++;
}
	
bool MyMatrix::Get (int nX, int nY) {
	return Array[MakeIndex(nX,nY)];
}

void MyMatrix::AddGrid (MyGrid& grid) {
	for (int i=0; i<nDimX; i++) if (grid.GetXXX(i)) 
		for (int j=0; j<nDimY; j++) if (grid.GetYYY(j)) 
			SetTrue(i,j);
}
	
void MyMatrix::AddRandomGrid () {
	int nDimGridX = nDimX, nDimGridY = nDimY;
	MyRndDimPairGrid(nDimGridX,nDimGridY);
	MyGrid gridRnd(nDimX,nDimGridX,nDimY,nDimGridY);
	AddGrid(gridRnd);
}

/* old version
void MyMatrix::InitRandom (int kolGrid) {
	for (int k=0; k<kolGrid; k++) {
		MyXY XX(nDimX);  //XX.InitRandom(MyRndGrid(nDimX));
		MyXY YY(nDimY);  //YY.InitRandom(MyRndGrid(nDimY));
		for (int i=0; i<XX.GetDimGrid(); i++) {
			int ii = XX.GetByNumber(i);
			for (int j=0; j<YY.GetDimGrid(); j++)
				SetTrue(ii,YY.GetByNumber(j));
		}
	}
}

const int kolIter = 1000;	//количество итераций - используется ниже

MyGrid* MyMatrix::MakeGridRnd () {
	MyXY xYes(nDimX);	
		// пока инициализация всеми false;
		// будем заполнять true в том случае, когда решим, что включим в грид
	MyXY xNot(nDimX);	
		// пока инициализация всеми false;
		// будем заполнять true в том случае, когда решим, что НЕ включим в грид	
	MyXY yYes(nDimY);	
	MyXY yNot(nDimY);	
		// аналогично
	for (;;) {
		bool bLine = false, bColumn = false;	// стоит ли ещё искать новую строку и столбец
		if		(xYes.GetDimGrid()>0  && yYes.GetDimGrid()==0) bColumn = true;
		else if	(xYes.GetDimGrid()==0 && yYes.GetDimGrid()>0)  bLine = true;
		else {
			for (int i=0; i<nDimX; i++)  if (!xYes.Get(i) && !xNot.Get(i))  bLine = true;
			for (int j=0; j<nDimY; j++)  if (!yYes.Get(j) && !yNot.Get(j))  bColumn = true;
		}
		if (!bLine && !bColumn) return new MyGrid(xYes,yYes);  
		bool bNowLine = !bColumn ? true : !bLine ? false : RndBool();
			// выбрали, что бум искать: строку или столбец
		if (bNowLine) {	// ищем строку
			// сначала выберем номер - а потом определим, можно ли включать в грид
			int nnn = Rnd(kolIter) + 1;
			for (int iii=0; ; iii++) {	// iii - тот самый выбираемый № строки
				if (iii>=nDimX) iii = 0;
				if (!xYes.Get(iii) && !xNot.Get(iii)) {
					if (--nnn>0) continue;
					// а теперь строку iii либо включаем в грид, либо отмечаем, что этого нельзя
					bool bExistsTrue = false;	// есть ли хоть одна 1 в строке
					for (int j=0; j<nDimY; j++) {
						if (Get(iii,j)) bExistsTrue = true;	// есть
						if (!yYes.Get(j)) continue;	// столбец j (ещё) не включён в грид - 
													// незачем его и рассматривать
						if (!Get(iii,j)) {
							xNot.SetNewTrue(iii);
							goto Continue;
						}
					}
					if (!bExistsTrue) {
						xNot.SetNewTrue(iii);
						goto Continue;
					}
					xYes.SetNewTrue(iii);
					goto Continue;	// снова выбирать либо строку, либо столбец
				}
			}
		}
		else {	// ищем столбeц
			// сначала выберем номер - а потом определим, можно ли включать в грид
			int nnn = Rnd(kolIter) + 1;
			for (int jjj=0; ; jjj++) {	// jjj - тот самый выбираемый № столбца
				if (jjj>=nDimY) jjj = 0;
				if (!yYes.Get(jjj) && !yNot.Get(jjj)) {
					if (--nnn>0) continue;
					// а теперь строку jjj либо включаем в грид, либо отмечаем, что этого нельзя
					bool bExistsTrue = false;	// есть ли хоть одна 1 в строке
					for (int i=0; i<nDimX; i++) {
						if (Get(i,jjj)) bExistsTrue = true;	// есть
						if (!xYes.Get(i)) continue;	// строка i (ещё) не включена в грид - 
													// незачем её и рассматривать
						if (!Get(i,jjj)) {
							yNot.SetNewTrue(jjj);
							goto Continue;
						}
					}
					if (!bExistsTrue) {
						yNot.SetNewTrue(jjj);
						goto Continue;
					}
					yYes.SetNewTrue(jjj);
					goto Continue;	// снова выбирать либо строку, либо столбец
				}
			}
		}
		Continue:;
	}
}
*/
		
MyGrid* MyMatrix::MakeGridRnd (int nX, int nY) {
	if (!Get(nX,nY)) return NULL;
	int nDimX = GetDimX(), nDimY = GetDimY();
	MyGrid* pGrid = new MyGrid(nDimX,nDimY);
	pGrid->SetNewTrueXXX(nX); pGrid->SetNewTrueYYY(nY); 
	for (;;) {
		if (::RndBool()) {	// сначала пытаемся добавить строку
			int nXX = ExtandXRnd(pGrid);
			if (nXX>=0) {
				pGrid->SetNewTrueXXX(nXX);
				continue;
			}
			int nYY = ExtandYRnd(pGrid);
			if (nYY<0) break;
			pGrid->SetNewTrueYYY(nYY);
		} else {	// сначала пытаемся добавить столбец
			int nYY = ExtandYRnd(pGrid);
			if (nYY>=0) {
				pGrid->SetNewTrueYYY(nYY);
				continue;
			}
			int nXX = ExtandXRnd(pGrid);
			if (nXX<0) break;
			pGrid->SetNewTrueXXX(nXX);
		}
	}
	return pGrid;
}
	
int MyMatrix::ExtandXRnd (MyGrid* pGrid) {
	int nDimX = GetDimX();
	int nAddX = ::Rnd(nDimX);
	for (int i=0; i<nDimX; i++) {
		int nX = (i+nAddX) % nDimX;
		// вот на эту nX-ю строку и пытаемся расширить, 
		// причём начинаем не с 0-й строки, а с nAddX-й
		if (pGrid->GetXXX(nX)) continue;		// не надо проверять
		for (int nY=0; nY<GetDimY(); nY++) {
			if (!pGrid->GetYYY(nY)) continue;	// не надо проверять
			if (!Get(nX,nY)) goto L;			// строка не подходит
		}
		return nX;
		L:;
	}
	return -1;
}
	
int MyMatrix::ExtandYRnd (MyGrid* pGrid) {
	int nDimY = GetDimY();
	int nAddY = ::Rnd(nDimY);
	for (int j=0; j<nDimY; j++) {
		int nY = (j+nAddY) % nDimY;
		if (pGrid->GetYYY(nY)) continue;		// не надо проверять
		for (int nX=0; nX<GetDimX(); nX++) {
			if (!pGrid->GetXXX(nX)) continue;
			if (!Get(nX,nY)) goto L;
		}
		return nY;
		L:;
	}
	return -1;
}

int MyMatrix::HowManyNew (MyGrid& grid) {
	int nRet = 0;
	for (int i=0; i<GetDimX(); i++) {
		if (!grid.GetXXX(i)) continue;
		for (int j=0; j<GetDimY(); j++) {
			if (!grid.GetYYY(i)) continue;
			if (!Get(i,j)) nRet++;
		}
	}
	return nRet;
}

ostream& operator<< (ostream& os, MyMatrix& matrix) {
	for (int i=0; i<matrix.nDimX; i++) {
		for (int j=0; j<matrix.nDimY; j++)  {
			bool b = matrix.Get(i,j);
			os << (b ? '1' : '0');
		}
		os << endl;
	}
	return os;
}

int operator- (MyMatrix& matr1, MyMatrix& matr2) {
	int nDimX = matr1.GetDimX();  if (nDimX!=matr2.GetDimX()) ::MyError(8);
	int nDimY = matr1.GetDimY();  if (nDimY!=matr2.GetDimY()) ::MyError(8);
	int nRet = 0;
	for (int i=0; i<nDimX; i++)  for (int j=0; j<nDimX; j++)  {
		if (!matr1.Get(i,j)) continue;
		if (!matr2.Get(i,j)) nRet++;
	}
	return nRet;
}
