
#include "stdafx.h"
#include "Little.h"
#include "Utily.h"
#include "Random.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyLittleSubTask - 
	
MyLittleSubTask::MyLittleSubTask (MyMatrix* pMatrix) :
	yesX(pMatrix->GetDimX()),	noX(pMatrix->GetDimX()),
	yesY(pMatrix->GetDimY()),	noY(pMatrix->GetDimY())
{
	this->pMatrix = pMatrix;
	if (!InitRandom()) MyError(10);
	nScore = 2;	// sic!
}

MyLittleSubTask::MyLittleSubTask (MyMatrix* pMatrix, int x, int y) :
	yesX(pMatrix->GetDimX()),	noX(pMatrix->GetDimX()),
	yesY(pMatrix->GetDimY()),	noY(pMatrix->GetDimY())
{
	this->pMatrix = pMatrix;
	yesX.SetNewTrue(x);
	yesY.SetNewTrue(y);
	nScore = 2;	// sic!
}

MyLittleSubTask::MyLittleSubTask (MyMatrix* pMatrix, int x, int y, int nScore) :
	yesX(pMatrix->GetDimX()),	noX(pMatrix->GetDimX()),
	yesY(pMatrix->GetDimY()),	noY(pMatrix->GetDimY())
{
	this->pMatrix = pMatrix;
	yesX.SetNewTrue(x);
	yesY.SetNewTrue(y);
	this->nScore = nScore;	// ����� ����� � ��� �����
}
	
MyLittleSubTask::MyLittleSubTask (MyLittleSubTask& copy) :
	yesX(copy.yesX),	noX(copy.noX),	
	yesY(copy.yesY),	noY(copy.noY)
{
	this->pMatrix = copy.pMatrix;
	this->nScore = copy.nScore;
}
	
/*
bool MyLittleSubTask::InitFirstRandom () {
	for (int i=0; i<1000; i++) {
		int x = Rnd(pMatrix->GetDimX());
		for (int j=0; j<1000; j++) {
			int y = Rnd(pMatrix->GetDimY());
			if (!pMatrix->Get(x,y)) continue;
			yesX.SetTrue(x);  yesY.SetTrue(y);
			return true;
		}
	}
	return false;
}
*/

bool MyLittleSubTask::InitRandom (int& x, int& y) {
	// ���������� MySubTask::GetRandomSquare
	RndPairs pairs;
	for (int i=0; i<pMatrix->GetDimX(); i++)  for (int j=0; j<pMatrix->GetDimY(); j++) {
		if (!pMatrix->Get(i,j)) continue;			// �� ��� ����� (����� � ���� �����) ������
		if (yesX.Get(i) && yesY.Get(j)) continue;	// ������ ��� ��������
		if (noX.Get(i) || noY.Get(j)) continue;		// ������ �������� ������
		pairs.AddPair(i,j);
	}
	if (!pairs.GetAnswer(x,y)) return false;
	yesX.SetNewTrue(x); yesY.SetNewTrue(y); 
	return true;
}

/* ������ �������
int MyLittleSubTask::CostX (int newX) {	// ����������� ��. � ����. �-���
	int nScore = 0;
	for (int j=0; j<pMatrix->GetDimY(); j++) {
		if (yesY.Get(j)) {
			if (!pMatrix->Get(newX,j)) return -1;
			if (!noY.Get(j)) nScore += pMatrix->GetCostY(j);
		}
	}
	return nScore;
}
	
int MyLittleSubTask::CostY (int newY) {
	int nScore = 0;
	for (int i=0; i<pMatrix->GetDimX(); i++) {
		if (yesX.Get(i)) {	// ������ ��� �������� � ����
			if (!pMatrix->Get(i,newY))	// ...�� �� �������� - ������� ��� ����� ������
				return -1;
			if (!noX.Get(i))		 	// ������ �� ��������� ��� ����������� �����
				nScore += pMatrix->GetCostX(i);
		}
	}
	return nScore;
}
*/

int MyLittleSubTask::CostX (int newX) {
	int nScore = 0;
	for (int j=0; j<pMatrix->GetDimY(); j++) {
		if (noY.Get(j)) continue;
		if (yesY.Get(j)) {
			if (!pMatrix->Get(newX,j)) return -1;
			nScore += 2 * pMatrix->GetCostY(j);
		}
		else nScore += pMatrix->GetCostY(j);
	}
	return nScore;
}

int MyLittleSubTask::CostY (int newY) {
	int nScore = 0;
	for (int i=0; i<pMatrix->GetDimX(); i++) {
		if (noX.Get(i)) continue;
		if (yesX.Get(i)) {
			if (!pMatrix->Get(i,newY)) return -1;
			nScore += 2 * pMatrix->GetCostX(i);
		}
		else nScore += pMatrix->GetCostX(i);
	}
	return nScore;
}


bool MyLittleSubTask::ExtandX (int& newX, int& nScore) {
	nScore = -1;
	for (int i=0; i<pMatrix->GetDimX(); i++) {
		if (yesX.Get(i) || noX.Get(i)) continue;
		int n = CostX(i);
		if (n<=nScore) continue;
		nScore = n;
		newX = i;
	}
	return nScore>=0;
}

bool MyLittleSubTask::ExtandY (int& newY, int& nScore) {
	nScore = -1;
	for (int j=0; j<pMatrix->GetDimY(); j++) {
		if (yesY.Get(j) || noY.Get(j)) continue;
		int n = CostY(j);
		if (n<=nScore) continue;
		nScore = n;
		newY = j;
	}
	return nScore>=0;
}

bool MyLittleSubTask::CanExtandX () {
	for (int i=0; i<pMatrix->GetDimX(); i++) {
		if (yesX.Get(i)) continue;				// ��������� ���������� ������ �� ����
		for (int j=0; j<pMatrix->GetDimY(); j++)  {
			if (!yesY.Get(j)) continue;			// ������� �� ������� - ��������� �� ����
			if (!pMatrix->Get(i,j)) goto L;		// 0 ��� ��� �������� ����� 1 - ��� � ���� ���
		}
		return true;							// ��� ��� �������� �����
		L:;
	}
	return false;
}

bool MyLittleSubTask::CanExtandY () {
	for (int j=0; j<pMatrix->GetDimY(); j++) {
		if (yesY.Get(j)) continue;
		for (int i=0; i<pMatrix->GetDimX(); i++)  {
			if (!yesX.Get(i)) continue;
			if (!pMatrix->Get(i,j)) goto L;
		}
		return true;
		L:;
	}
	return false;
}
	
/* ��� ������ ������������� 28 ���
MyLittleSubTask* MyLittleSubTask::MakeRight () {
	int newX, nScoreX, newY, nScoreY;
	bool bX = ExtandX(newX,nScoreX);
	bool bY = ExtandY(newY,nScoreY);
	if (!bX && !bY) return NULL;
	if (bX && bY) {
		if (nScoreX<nScoreY) bX = false;
		else bY = false;
	}
	MyLittleSubTask* stRight = new MyLittleSubTask(*this); // ����������� �����
	if (bX) {
		stRight->yesX.SetTrue(newX);
		this->noX.SetTrue(newX);
	} else {
		stRight->yesY.SetTrue(newY);
		this->noY.SetTrue(newY);
	}
	return stRight;
}
	
bool operator> (MyLittleSubTask& lst1, MyLittleSubTask& lst2) {
	int yes1 = lst1.yesScore(); int yes2 = lst2.yesScore();
	if (yes1>yes2) return true;
	if (yes1<yes2) return false;
	yes1 = lst1.yesScoreP(); yes2 = lst2.yesScoreP();
	if (yes1>yes2) return true;
	if (yes1<yes2) return false;
	return lst1.noScore() < lst2.noScore();	// sic
}
*/

void MyLittleSubTask::MakeScore () {
	int yX = yesX.GetDimGrid(), yY = yesY.GetDimGrid(), nX = noX.GetDimGrid(), nY = noY.GetDimGrid();
	nScore = 2 * yX * yY - (nX*nY + nX + nY);
}
	
void MyLittleSubTask::AddYesX (int newX) { 
	yesX.SetNewTrue(newX); 
	if (nScore<999999) MakeScore();	// a ������� �������� ��������� � � �������-�����������
}
	
void MyLittleSubTask::AddYesY (int newY) { 
	yesY.SetNewTrue(newY); 
	if (nScore<999999) MakeScore();	// ����������
}
	
void MyLittleSubTask::AddNoX (int newX) { 
	noX.SetNewTrue(newX); 
	MakeScore();
}
	
void MyLittleSubTask::AddNoY (int newY) { 
	noY.SetNewTrue(newY); 
	MakeScore();
}

MyLittleSubTask* MyLittleSubTask::MakeRight () {
	MyLittleSubTask* stRight = new MyLittleSubTask(*this); // ����������� �����
	if (yesX.Empty() || yesY.Empty()) {	// ���� ������ �-� ������, ��:
		int newX,newY;
		if (!stRight->InitRandom(newX,newY)) goto Bad;
		this->AddNoX(newX);
		this->AddNoY(newY);
	} else {
		int newX, nScoreX, newY, nScoreY;
		bool bX = ExtandX(newX,nScoreX);
		bool bY = ExtandY(newY,nScoreY);
		if (!bX && !bY) goto Bad;
		if (bX && bY) {
			if (nScoreX<nScoreY) bX = false;
			else bY = false;
		}
		if (bX) {
			stRight->AddYesX(newX);
			this->AddNoX(newX);
		} else {
			stRight->AddYesY(newY);
			this->AddNoY(newY);
		}
	}
	return stRight;
	Bad:; delete stRight; return NULL;
}
		
MyGrid* MyLittleSubTask::MakeGrid () {
	// ������� ���������� ������������� �������� ����� ����������;
	// ������ ��� ����� - ��. ������ ������ � ~readme.rtf
	if (CanExtandX() || CanExtandY()) return NULL;
	return new MyGrid(yesX,yesY);
}
	
ostream& operator<< (ostream& os, MyLittleSubTask& mlst) {
	cout << "yes:" << mlst.yesX << "-" << mlst.yesY << endl;
	cout << "no :" << mlst.noX  << "-" << mlst.noY  << endl;
	cout << "score :" << mlst.nScore << endl;
	return os;
}

bool operator< (MyLittleSubTask& lst1, MyLittleSubTask& lst2) {
	return lst1.GetScore() < lst2.GetScore();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyArrayLittleSubTask - 

MyArrayLittleSubTask::~MyArrayLittleSubTask () {
	for (int i=0; i<GetSize(); i++) {
		MyLittleSubTask* pSubTask = (MyLittleSubTask*)GetAt(i);
		delete pSubTask;
	}
}
	
void MyArrayLittleSubTask::MyAdd (MyLittleSubTask* pSTAdd) {
	InsertAt(0,pSTAdd);
	for (int i=0; ; i++) {
		if (i+1 > GetUpperBound()) return;
		MyLittleSubTask* pTmp = (MyLittleSubTask*)GetAt(i+1);
		if (*pTmp < *pSTAdd) return;
		SetAt(i,pTmp);
		SetAt(i+1,pSTAdd);
	}
}

/*
void MyArrayLittleSubTask::MyAddPrim (MyLittleSubTask* pSTAdd) {
	Add(pSTAdd);
	for (int i=GetUpperBound(); i>=1; i--) {
		MyLittleSubTask* p1 = (MyLittleSubTask*)GetAt(i-1);
		MyLittleSubTask* p2 = (MyLittleSubTask*)GetAt(i);
		if (*p1 < *p2) return;
		SetAt(i-1,p2);
		SetAt(i,p1);
		// MyLittleSubTask* p = p1;  p1 = p2;  p2 = p; - ��� ������!
	}
}
*/
      
MyLittleSubTask* MyArrayLittleSubTask::ExtractFirst () {
	if (GetSize()<=0) return NULL;
	MyLittleSubTask* pSTReturn = (MyLittleSubTask*)GetAt(0);
	this->RemoveAt(0);
	return pSTReturn;
}
        
MyLittleSubTask* MyArrayLittleSubTask::GetFirst () {
	if (GetSize()<=0) return NULL;
	return (MyLittleSubTask*)GetAt(0);
}
	
ostream& operator<< (ostream& os, MyArrayLittleSubTask& arrsubtask) {
	int nSize = arrsubtask.GetSize();
	cout << "* number of subtasks: " << nSize;
	if (nSize>10) { 
		cout << " (we print first 10 ones)";
		nSize = 10;
	}
	cout << endl;
    for (int i=0; i<nSize; i++) {
        MyLittleSubTask* pSubTask = (MyLittleSubTask*)arrsubtask.GetAt(i);
        os << "*subtask # " << i << endl << *pSubTask;
    }
	return os;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyLittleTask - 

MyLittleTask::MyLittleTask (MyMatrix* pMatrix, MyArrayGrid* pArrGrid)
{
	this->pMatrix = pMatrix;
	this->pArrGrid = pArrGrid;
	Add(new MyLittleSubTask(pMatrix));	// MyAdd() �������� �������
	// ��� �� ������� ���� ��������� - �� ���� ������������ "������� ������ �����",
	// �.�. ������������ ��������� � �������� �������;
	// � ��� ����� ������ �� "��������" ���
}

/* ������ ������
		// �������� ������� ��������� ���, ���� ������ ��������� �� ������;
		// �� �������� ��� ���;
		// ���� ���-�� �� ������� - �������� ������� ����� ����;
		// �������:	0 - ��, ����� ����, �������� ��� � arrGrid;
		//			1 - ����� ���� ���, �� � arrGrid �� ��� ��� �������;
		//			2 - ��� �������� - �.�. ������� ������ ��� � ����� 1� ���������
		//				(����� ������ ����� ���� �� ��������������);
		//			3 - ������������� ������������ ���� ("��������")
		//			4 - ������ �������� ��� ����;
int MyLittleTask::Step () {
	MyLittleSubTask* stCurrent = ExtractFirst();
	if (stCurrent==NULL) return 4;
	// ������ ������ ���
	// for (int i=0;;i++) {
	for (;;) {
		MyLittleSubTask* stR = stCurrent->MakeRight();
		if (stR==NULL) {	// ���� ��� ����� - ���� �� ��� ����������� �������� � arrGrid
			MyGrid* gridNew = stCurrent->MakeGrid();
			delete stCurrent;
			if (gridNew==NULL) return 3;
			if (pArrGrid->ExistsGrid(gridNew)>=-1) {	// sic!!!
				delete gridNew;
				return 1;
			}
			pArrGrid->Add(gridNew);
			return 0;
		}
		// ����� ������ - (�����) � �����-��������, � � ������ ����� ��������
		// cout << "+++++" << i << endl << *stR;
		MyAdd2(stCurrent);
		stCurrent = stR;
	}
	// ��� 08 (� ����� ������� � ��� 07): 
	// � �����-�� ���� ���� ��� ������������ ����������,
	// ��������� ������ ������ ����� ������ � ������ � ����� ����� ���������;
	// ���� ��� ������� ��� MyTask
}
*/

int MyLittleTask::Step () {
	MyLittleSubTask* stCurrent = ExtractFirst();
	if (stCurrent==NULL) return 4;
	MyLittleSubTask* stR = stCurrent->MakeRight();
	if (stR==NULL) {	// ���� ��� ����� - ���� �� ��� ����������� �������� � arrGrid
		MyGrid* gridNew = stCurrent->MakeGrid();
		delete stCurrent;
		if (gridNew==NULL) return 3;
		if (pArrGrid->ExistsGrid(gridNew)>=-1) {	// sic!!!
			delete gridNew;
			return 1;
		}
		pArrGrid->Add(gridNew);
		return 0;
	}
	MyAdd(stCurrent);
	MyAdd(stR);
	return 2;
}
		
void MyLittleTask::SpecialAddSubTask (int x, int y) {
	MyLittleSubTask* pLST = new MyLittleSubTask(pMatrix,x,y,999999);	// sic!
	this->InsertAt(0,pLST);
}

ostream& operator<< (ostream& os, MyLittleTask& littletask) {
	cout << endl << "*** begin printing new little task";
	cout << endl << "** begin printing matrix" << endl << *littletask.pMatrix;
	cout << "** begin printing array of grids" << endl << *littletask.pArrGrid;
		// ��� ������� �������� ��� ����, ����� ������ "MyMatrix* pMatrix", � �� "MyMatrix *pMatrix"
	MyArrayLittleSubTask& alst = littletask;  
	cout << "** begin printing array of little SUBtask" << endl << alst;
	cout << "*** end printing new little task" << endl;
	return os;
}
