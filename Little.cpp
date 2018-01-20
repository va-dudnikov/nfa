
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
	this->nScore = nScore;	// может можно и без этого
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
	// аналогично MySubTask::GetRandomSquare
	RndPairs pairs;
	for (int i=0; i<pMatrix->GetDimX(); i++)  for (int j=0; j<pMatrix->GetDimY(); j++) {
		if (!pMatrix->Get(i,j)) continue;			// ну нет такой (буквы в этом слове) клетки
		if (yesX.Get(i) && yesY.Get(j)) continue;	// клетка уже включена
		if (noX.Get(i) || noY.Get(j)) continue;		// клетку включать нельзя
		pairs.AddPair(i,j);
	}
	if (!pairs.GetAnswer(x,y)) return false;
	yesX.SetNewTrue(x); yesY.SetNewTrue(y); 
	return true;
}

/* старый вариант
int MyLittleSubTask::CostX (int newX) {	// комментарии см. в след. ф-ции
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
		if (yesX.Get(i)) {	// строка уже включена в грид
			if (!pMatrix->Get(i,newY))	// ...но не подходит - столбец для грида плохой
				return -1;
			if (!noX.Get(i))		 	// строка не запрещена для строящегося грида
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
		if (yesX.Get(i)) continue;				// проверять включённые строки не надо
		for (int j=0; j<pMatrix->GetDimY(); j++)  {
			if (!yesY.Get(j)) continue;			// столбец НЕ включён - проверять не надо
			if (!pMatrix->Get(i,j)) goto L;		// 0 там где пытались найти 1 - идём к след стр
		}
		return true;							// эту стр включить можно
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
	
/* это версия закончившаяся 28 янв
MyLittleSubTask* MyLittleSubTask::MakeRight () {
	int newX, nScoreX, newY, nScoreY;
	bool bX = ExtandX(newX,nScoreX);
	bool bY = ExtandY(newY,nScoreY);
	if (!bX && !bY) return NULL;
	if (bX && bY) {
		if (nScoreX<nScoreY) bX = false;
		else bY = false;
	}
	MyLittleSubTask* stRight = new MyLittleSubTask(*this); // конструктор копии
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
	if (nScore<999999) MakeScore();	// a большое значение сохраняем и в задачах-наследниках
}
	
void MyLittleSubTask::AddYesY (int newY) { 
	yesY.SetNewTrue(newY); 
	if (nScore<999999) MakeScore();	// аналогично
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
	MyLittleSubTask* stRight = new MyLittleSubTask(*this); // конструктор копии
	if (yesX.Empty() || yesY.Empty()) {	// если пустой к-л массив, то:
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
	// сначала необходима окончательная проверка перед включением;
	// почему это нужно - см. плохой пример в ~readme.rtf
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
		// MyLittleSubTask* p = p1;  p1 = p2;  p2 = p; - так нельзя!
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
	Add(new MyLittleSubTask(pMatrix));	// MyAdd() вызывать незачем
	// это мы сделали ОДНУ подзадачу - но надо периодически "вливать свежую кровь",
	// т.е. генерировать подзадачи с заданным началом;
	// а это легко делать из "большого" МВГ
}

/* старая версия
		// пытаемся сделать очередной шаг, беря первую подзадачу из списка;
		// не забываем про ППЗ;
		// если что-то не удалось - пытаемся сделать новый грид;
		// возврат:	0 - ОК, новый грид, включили его в arrGrid;
		//			1 - новый грид был, но в arrGrid он уже был включён;
		//			2 - ППЗ прервали - т.к. граница больше чем у новой 1й подзадачи
		//				(такой случай вроде пока не рассматривался);
		//			3 - сгенерировали неправильный грид ("неполный")
		//			4 - список подзадач был пуст;
int MyLittleTask::Step () {
	MyLittleSubTask* stCurrent = ExtractFirst();
	if (stCurrent==NULL) return 4;
	// теперь делаем ППЗ
	// for (int i=0;;i++) {
	for (;;) {
		MyLittleSubTask* stR = stCurrent->MakeRight();
		if (stR==NULL) {	// грид уже готов - надо бы его попробовать засунуть в arrGrid
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
		// левую задачу - (назад) в класс-родитель, а с правой прдлж работать
		// cout << "+++++" << i << endl << *stR;
		MyAdd2(stCurrent);
		stCurrent = stR;
	}
	// янв 08 (а метод писался в окт 07): 
	// а вааще-то надо было это организовать рекурсивно,
	// засовывая правую задачу самой первой в массив и далее делая самовызов;
	// надо так сделать для MyTask
}
*/

int MyLittleTask::Step () {
	MyLittleSubTask* stCurrent = ExtractFirst();
	if (stCurrent==NULL) return 4;
	MyLittleSubTask* stR = stCurrent->MakeRight();
	if (stR==NULL) {	// грид уже готов - надо бы его попробовать засунуть в arrGrid
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
		// вот главный аргумент для того, чтобы писать "MyMatrix* pMatrix", а не "MyMatrix *pMatrix"
	MyArrayLittleSubTask& alst = littletask;  
	cout << "** begin printing array of little SUBtask" << endl << alst;
	cout << "*** end printing new little task" << endl;
	return os;
}
