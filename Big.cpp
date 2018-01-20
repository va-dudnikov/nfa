
#include "stdafx.h"
#include "Utily.h"
#include "Random.h"
#include "Big.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyWordArray - 

MyWordArray::MyWordArray (MyWordArray& copy) : CWordArray() {
	AddCopy(copy);
}
	
bool MyWordArray::ExistsNom (int nom) {
	for (int i=0; i<GetSize(); i++)  if (GetAt(i)==nom)  return true;
	return false;
}
	
MyWordArray::operator= (MyWordArray& copy) {
	RemoveAll();
	AddCopy(copy);
}
		
void MyWordArray::DeleteNom (int nom) {
	for (int i=0; i<GetSize(); i++)  if (GetAt(i)==nom)  RemoveAt(i);
}

void MyWordArray::AddCopy (MyWordArray& copy) {
	for (int i=0; i<copy.GetSize(); i++)  Add(copy.GetAt(i));
}

bool operator>= (MyWordArray& wa1, MyWordArray& wa2) {
	for (int i=0; i<wa2.GetSize(); i++) {
		int nnn = wa2.GetAt(i);
		if (!wa1.ExistsNom(nnn)) return false;
	}
	return true;
}

bool operator== (MyWordArray& wa1, MyWordArray& wa2) {
	return wa1>=wa2 && wa2>=wa1;
}

const double rNoEq = 2.0;	// значение метрики если нет совпадающих элементов
double operator&& (MyWordArray& wa1, MyWordArray& wa2) {
	int nKol = 0;	// число совпадающих элементов
	for (int i=0; i<wa1.GetSize(); i++)  if (wa2.ExistsNom(wa1.GetAt(i)))  nKol++;
	if (nKol==0) return rNoEq;
	return 1.0/double(nKol);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MySubTask (Big) - 

MySubTask::MySubTask (MyMatrix* pMatrix, MyArrayGrid* pArrayGrid) : 
	arrYes(), arrNo(), matrThis(pMatrix->GetDimX(),pMatrix->GetDimY()) 
{
	this->pMatrix = pMatrix;
	this->pArrayGrid = pArrayGrid;
	// всЄ так! параметры завод€тс€ в задаче (MyTask)
}

MySubTask::MySubTask (MySubTask& copy) : 
	arrYes(copy.arrYes), arrNo(copy.arrNo), matrThis(copy.matrThis) 
{
	this->pMatrix = copy.pMatrix;
	this->pArrayGrid = copy.pArrayGrid;
}
		
int MySubTask::SelectGridSimple () {
	int nBestAdd = 0;	// наибольшее кол-во добал€емых 1
	int nBestNom = -1;	// соответствующий є
	for (int i=0; i<pArrayGrid->GetSize(); i++) {
		if (arrYes.ExistsNom(i) || arrNo.ExistsNom(i)) continue;
		MyGrid* pGrid = (MyGrid*) pArrayGrid->GetAt(i);
		int nTmp = matrThis.HowManyNew(*pGrid);
		if (nTmp<=nBestAdd) continue;
		nBestAdd = nTmp;
		nBestNom = i;
	}
	return nBestNom;
}
		
int MySubTask::BoundSimple ()  { 
	return *pMatrix - matrThis; 
}
		
int MySubTask::BoundSecond ()  { 
	int n1 = BoundSimple();
	int n2 = arrYes.GetSize();
	int n3 = arrNo.GetSize();
	return 10*n1 + 4*n2 + n3; 
}

MySubTask* MySubTask::MakeRight () {
	int nom = SelectGridSimple();
	if (nom<0) return NULL;
	return MakeRight(nom);
}

MySubTask* MySubTask::MakeRight (int nom) {
	MySubTask*  stRight = new MySubTask(*this); // конструктор копии
	stRight->AddYes(nom);
	this->AddNo(nom);
	return stRight;
}
	
bool MySubTask::GetRandomSquare (int& x, int& y) {
	RndPairs pairs;
	for (int i=0; i<pMatrix->GetDimX(); i++)  for (int j=0; j<pMatrix->GetDimY(); j++) 
		if (pMatrix->Get(i,j) && !matrThis.Get(i,j))
			pairs.AddPair(i,j);
	return pairs.GetAnswer(x,y);
}

void MySubTask::AddYes (int nom) {
	arrYes.Add(nom);
	// теперь добавим в главную матрицу подзадачи (matrThis) новые 1
	MyGrid* pGridNew = (MyGrid*) pArrayGrid->GetAt(nom);
	/*
	MyXY xxx = pGridNew->GetXX();  MyXY yyy = pGridNew->GetYY();  
	for (int i=0; i<xxx.GetDim(); i++) {
		if (!xxx.Get(i)) continue;
		for (int j=0; j<yyy.GetDim(); j++)
			if (yyy.Get(j)) matrThis.SetTrue(i,j);
	}
	*/
	for (int i=0; i<pMatrix->GetDimX(); i++) {
		if (!pGridNew->GetXXX(i)) continue;
		for (int j=0; j<pMatrix->GetDimY(); j++)
			if (pGridNew->GetYYY(j)) matrThis.SetTrue(i,j);
	}
}

void MySubTask::AddNo (int nom) {
	arrNo.Add(nom);
}
	
ostream& operator<< (ostream& os, MySubTask& subtask) {
	os << subtask.BoundSimple() << "(" << subtask.BoundSecond() << ")";
	return os;
}

/*
bool operator<= (MySubTask& st1, MySubTask& st2) {
	//return st1.GetSumma() <= st2.GetSumma();
	//return st1.BoundSimple() <= st2.BoundSimple();
	return st1.BoundSecond() <= st2.BoundSecond();
}
*/

bool operator== (MySubTask& st1, MySubTask& st2) {
	return st1.GetYes()==st2.GetYes() && st1.GetNo()==st2.GetNo();
}

const double rKoef = 0.2;  
    // реально это значение подбираетс€ с помощью √ј - 
	// но здесь эта часть проекта не приводитс€
double operator&& (MySubTask& st1, MySubTask& st2) {
	return st1.GetYes() && st1.GetYes();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyArraySubTask (Big) - 

MyArraySubTask::~MyArraySubTask () {
	for (int i=0; i<GetSize(); i++) {
		MySubTask* pSubTask = (MySubTask*)GetAt(i);
		delete pSubTask;
	}
}

void MyArraySubTask::MyAdd (MySubTask* pAdd) {
	int nAdd = pAdd->BoundSecond();
	for (int i=0; i<GetSize(); i++) {
		MySubTask* pTmp = (MySubTask*)GetAt(i);
		int nTmp = pTmp->BoundSecond();
		if (nAdd>nTmp) continue;
		if (nAdd<nTmp) {	// уже дошли до подзадач с худшей колбасистостью
			InsertAt(i,pAdd);
			return;
		}
		// далее значени€ колбасистостей равны; смотрим, не равны ли случайно сами задачи
		if (*pAdd==*pTmp) return;
	}
	Add(pAdd);
}

/* стара€ вери€ - но работала неплохо
void MyArraySubTask::MyAdd (MySubTask* pAdd) {
	InsertAt(0,pSTAdd);
	int nBoundSTAdd = pSTAdd->BoundSecond();
	for (int i=0; ; i++) {
		if (i+1 > GetUpperBound()) return;
		MySubTask* pTmp = (MySubTask*)GetAt(i+1);
		if (nBoundSTAdd <= pTmp->BoundSecond()) return;
		SetAt(i,pTmp);
		SetAt(i+1,pSTAdd);
	}
}

/* стара€ вери€ - еЄ вспомнить только в сверх-крайнем случае
void MyArraySubTask::MyAdd (MySubTask* pSTAdd, int nMin) {
	Add(pSTAdd);
	int nUpperBound = GetUpperBound();
	for (int i=nUpperBound; i>=nMin+1; i--) {	// sic!
		MySubTask* p1 = (MySubTask*)GetAt(i-1);
		MySubTask* p2 = (MySubTask*)GetAt(i);
		if (*p1 <= *p2) return;
		SetAt(i-1,p2);
		SetAt(i,p1);
		// MyLittleSubTask* p = p1;  p1 = p2;  p2 = p; - так нельз€!
	}
}
*/
        
MySubTask* MyArraySubTask::ExtractFirst () {
	if (GetSize()<=0) return NULL;
	MySubTask* pSTReturn = (MySubTask*)GetAt(0);
	if (pSTReturn->Ready()) ::MyError(9);
	this->RemoveAt(0);
	return pSTReturn;
}
        
/*
MySubTask* MyArraySubTask::GetFirst () {
	if (GetSize()<=0) return NULL;
	return (MySubTask*)GetAt(0);
}
*/
	
/*
void MyArraySubTask::Clean () {
	for (int i=0; i<10; i++) {
		if (i>GetUpperBound()-1) return;
		MySubTask* pSTMain = (MySubTask*)GetAt(i);
		for (int j=i+1; j<GetSize(); j++) {
			MySubTask* pSTSlave = (MySubTask*)GetAt(j);
			if (!(*pSTMain==*pSTSlave)) continue;
			delete pSTSlave;
			RemoveAt(j);
		}
	}
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyTask (Big) - 
	
MyTask::MyTask (MyMatrix& matr) : MyArraySubTask(), matrMain(matr), ltMain(&matr,&arrGridMain) {
	nOpt = 999999;
	for (int i=0; i<1000; i++) {	// ну хоть один-то грид дл€ начала надо сделать
		int n = ltMain.Step();
		//if (n==0) break;
	}
	// да и хоть одну подзадачу иметь надо
	MySubTask* pSTFirst = new MySubTask(&matrMain,&arrGridMain);
	this->Add(pSTFirst);
}

int MyTask::Step () {
	MySubTask* stCurrent = ExtractFirst();
	if (stCurrent==NULL) return 4;	// массив задач пуст
	MySubTask* stR = stCurrent->MakeRight();
	MyAdd(stCurrent);			// старую задачу засовываем в любом случае
	if (stR==NULL) return 0;	
		// ничего смертельного - может, просто не хватает готовых гридов;
		// но очень важно: чтобы не зациклитьс€, теперь надо об€зательно вызвать
		// принудительное построение нового грида!
	return ProofReady(stR,false);
}

void MyTask::SubStep () {
	MySubTask* stFirst = ExtractFirst();
	if (stFirst==NULL) return;
	int x, y;
	if (!stFirst->GetRandomSquare(x,y)) MyError(12);
	int nom = arrGridMain.GridBySqare(x,y);
	if (nom<0) {
		// такого грида не нашли - ну и какого хрена лезть в подзадачу (как в старом варианте) - 
		// строим подход€щий грид просто по матрице
		MyGrid* pGrid = matrMain.MakeGridRnd(x,y);
		arrGridMain.Add(pGrid);
		nom = arrGridMain.ExistsGrid(pGrid);
		if (nom!=arrGridMain.GetUpperBound()) MyError(11);
	}
	MySubTask* stR = stFirst->MakeRight(nom);
	MyAdd(stFirst);
	ProofReady(stR,true);
}
		
int MyTask::ProofReady (MySubTask* stR, bool bFirst) {
	if (!stR->Ready()) {
		MyAdd(stR);
		//if (bFirst) InsertAt(0,stR); else MyAdd(stR);
		// если засовываем эту новую задачу самой первой - 
		// то с помощью такой "псевдорекурсии" делаем ѕѕ«
		return 0;
	}
	int n = stR->GetSizeYes();
	int nRet = 1;
	if (n<nOpt) { 
		nOpt = n; 
		arrYesOpt = stR->GetYes(); 
		nRet = 2; 
	}
	delete stR;
	return nRet;
}

/* старый вариант
void MyTask::SubStep () {
	MySubTask* stFirst = GetFirst();
	if (stFirst==NULL) return;
	int x, y;
	if (!stFirst->GetRandomSquare(x,y)) return;	// конечно, никакого деструктора
	int nom = arrGridMain.GridBySqare(x,y);
	if (nom<0) {	// такого грида не нашли
		ltMain.SpecialAddSubTask(x,y);
		// а ещЄ принудительно сделаем грид
		MyLittleSubTask* lstTmp = new MyLittleSubTask(&matrMain,x,y);
		// далее похоже на MyLittleSubTask::MakeRight()
		for (;;) {
			int newX, nScoreX, newY, nScoreY;
			bool bX = lstTmp->ExtandX(newX,nScoreX);
			bool bY = lstTmp->ExtandY(newY,nScoreY);
			if (!bX && !bY) break;
			if (bX && bY && nScoreX<nScoreY) bX = false;
			if (bX) lstTmp->AddYesX(newX); else lstTmp->AddYesY(newY);
		}
		MyGrid* gridTmp = lstTmp->MakeGrid();
		nom = arrGridMain.ExistsGrid(gridTmp);
		if (nom>=0) delete gridTmp;
		else {
			arrGridMain.Add(gridTmp);
			nom = arrGridMain.GetUpperBound();
		}
		delete lstTmp;
	}
	if (nom<0) return;	// in case - такого грида так и нет
	// подход€щий грид с номером nom
	this->RemoveAt(0);
	stFirst->GetYes().DeleteNom(nom);	// in case
	stFirst->GetNo().DeleteNom(nom);
	MySubTask* stR = stFirst->MakeRight(nom);
	// MyAdd(stFirst,GetUpperBound()/10);	// старую задачу засовываем в любом случае но не совсем с начала
	MyAdd(stFirst);
	if (stR->Ready()) {			// готово; но улучшили ли предыдущее решение?
		int n = stR->GetSizeYes();
		if (n<nOpt) {
			nOpt = n;
			arrYesOpt = stR->GetYes();
		}
		delete stR;
	}
	else MyAdd(stR);
}
*/

int MyTask::BigStep () {
	ltMain.Step();
	if (this->Step()==0 || ::RndLT(0.1))	// пытаемс€ принудительно сделать новый грид
		SubStep();
	//if (::RndLT(0.03)) Clean();	// in case
	return nOpt;
}
	
ostream& operator<< (ostream& os, MyTask& task) {
	os << task.arrGridMain.GetSize() << " " << task.GetSize() << " " << task.nOpt << " ***" << endl;
	for (int i=0; i<min(10,task.GetSize()); i++) {
		MySubTask* pST = (MySubTask*)task.GetAt(i);
		os << " " << *pST;
	}
	os << " ..." << endl;
	return os;
}
