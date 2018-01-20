
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
	// ��� "nSumma" ��. ����������� � h-�����
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

const int kolIter = 1000;	//���������� �������� - ������������ ����

MyGrid* MyMatrix::MakeGridRnd () {
	MyXY xYes(nDimX);	
		// ���� ������������� ����� false;
		// ����� ��������� true � ��� ������, ����� �����, ��� ������� � ����
	MyXY xNot(nDimX);	
		// ���� ������������� ����� false;
		// ����� ��������� true � ��� ������, ����� �����, ��� �� ������� � ����	
	MyXY yYes(nDimY);	
	MyXY yNot(nDimY);	
		// ����������
	for (;;) {
		bool bLine = false, bColumn = false;	// ����� �� ��� ������ ����� ������ � �������
		if		(xYes.GetDimGrid()>0  && yYes.GetDimGrid()==0) bColumn = true;
		else if	(xYes.GetDimGrid()==0 && yYes.GetDimGrid()>0)  bLine = true;
		else {
			for (int i=0; i<nDimX; i++)  if (!xYes.Get(i) && !xNot.Get(i))  bLine = true;
			for (int j=0; j<nDimY; j++)  if (!yYes.Get(j) && !yNot.Get(j))  bColumn = true;
		}
		if (!bLine && !bColumn) return new MyGrid(xYes,yYes);  
		bool bNowLine = !bColumn ? true : !bLine ? false : RndBool();
			// �������, ��� ��� ������: ������ ��� �������
		if (bNowLine) {	// ���� ������
			// ������� ������� ����� - � ����� ���������, ����� �� �������� � ����
			int nnn = Rnd(kolIter) + 1;
			for (int iii=0; ; iii++) {	// iii - ��� ����� ���������� � ������
				if (iii>=nDimX) iii = 0;
				if (!xYes.Get(iii) && !xNot.Get(iii)) {
					if (--nnn>0) continue;
					// � ������ ������ iii ���� �������� � ����, ���� ��������, ��� ����� ������
					bool bExistsTrue = false;	// ���� �� ���� ���� 1 � ������
					for (int j=0; j<nDimY; j++) {
						if (Get(iii,j)) bExistsTrue = true;	// ����
						if (!yYes.Get(j)) continue;	// ������� j (���) �� ������� � ���� - 
													// ������� ��� � �������������
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
					goto Continue;	// ����� �������� ���� ������, ���� �������
				}
			}
		}
		else {	// ���� �����e�
			// ������� ������� ����� - � ����� ���������, ����� �� �������� � ����
			int nnn = Rnd(kolIter) + 1;
			for (int jjj=0; ; jjj++) {	// jjj - ��� ����� ���������� � �������
				if (jjj>=nDimY) jjj = 0;
				if (!yYes.Get(jjj) && !yNot.Get(jjj)) {
					if (--nnn>0) continue;
					// � ������ ������ jjj ���� �������� � ����, ���� ��������, ��� ����� ������
					bool bExistsTrue = false;	// ���� �� ���� ���� 1 � ������
					for (int i=0; i<nDimX; i++) {
						if (Get(i,jjj)) bExistsTrue = true;	// ����
						if (!xYes.Get(i)) continue;	// ������ i (���) �� �������� � ���� - 
													// ������� � � �������������
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
					goto Continue;	// ����� �������� ���� ������, ���� �������
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
		if (::RndBool()) {	// ������� �������� �������� ������
			int nXX = ExtandXRnd(pGrid);
			if (nXX>=0) {
				pGrid->SetNewTrueXXX(nXX);
				continue;
			}
			int nYY = ExtandYRnd(pGrid);
			if (nYY<0) break;
			pGrid->SetNewTrueYYY(nYY);
		} else {	// ������� �������� �������� �������
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
		// ��� �� ��� nX-� ������ � �������� ���������, 
		// ������ �������� �� � 0-� ������, � � nAddX-�
		if (pGrid->GetXXX(nX)) continue;		// �� ���� ���������
		for (int nY=0; nY<GetDimY(); nY++) {
			if (!pGrid->GetYYY(nY)) continue;	// �� ���� ���������
			if (!Get(nX,nY)) goto L;			// ������ �� ��������
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
		if (pGrid->GetYYY(nY)) continue;		// �� ���� ���������
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
