
#if !defined(_Little)
#define _Little

#include <fstream.h>
#include <iostream.h>
#include <iomanip.h>

#include "Matrix.h"
#include "Grid.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyLittleSubTask - 

class MyLittleSubTask {
private:	// внимание! ещё некоторые поля добавлены ниже
	MyMatrix* pMatrix;			// kein Destructor!
	MyXY yesX, yesY, noX, noY;	// изменять только с помощью методов AddYesX и т.п.!
public:
	MyLittleSubTask (MyMatrix* pMatrix);
	MyLittleSubTask (MyMatrix* pMatrix, int x, int y);	
		// c занесением заданной 1й клетки в yesX и yesY
	MyLittleSubTask (MyMatrix* pMatrix, int x, int y, int nScore);	
		// ещё и c дополнительной принудительной установкой nScore - см. это поле ниже
	MyLittleSubTask (MyLittleSubTask& copy);
	virtual ~MyLittleSubTask () {}
	bool InitRandom (int& x, int& y);	// случайно выбирает возможную клетку и заносит в yesX и yesY
private:
	bool InitRandom () { int x,y; return InitRandom (x,y); }
	int CostX (int newX);	// аналогично следующему
	int CostY (int newY);	// "очки" для столбца, возможно включаемого в грид
		// возврат -1 - вааще не подходит
public:
	bool ExtandX (int& newX, int& nScore);	// аналогично следующему
	bool ExtandY (int& newY, int& nScore);	// см. рис. в маленькой тетради
		// возврат параметров - новый столбец и "колбасистость"
		// далее 2 шт. - упрощение 2 предыдущих; рассматриваем только не входящие в yesX и yesY
private:
	bool CanExtandX ();
	bool CanExtandY ();
	// это версия от 28 янв 08; всё следующее надо переписать
	/*
	void AddYesX (int newX) { yesX.SetNewTrue(newX); }
	void AddYesY (int newY) { yesY.SetNewTrue(newY); }
	int yesScore ()	{ return yesX.GetDim() * yesY.GetDim(); } // с CostX() и CostY() не связано
	int yesScoreP() { return yesX.GetDim() + yesY.GetDim(); } // аналогично
	int noScore  ()	{ return noX. GetDim() * noY. GetDim(); } // аналогично
	MyLittleSubTask* MakeRight ();
		// делает правую подзадачу, изменяя себя под левую;
		// возврат NULL - если расширить не удалось, т.е. грид уже готов;
		// в этом случае есть смысл вызывать следующую функцию:
	*/
	// далее идёт версия начиная с 29 янв 08
private:	// новoе полe
	int nScore;
		// здесь считаем, что чем больше, тем лучше -
		// и В САМОМ НАЧАЛЕ это будет количество уже заполненных клеток,
		// т.е. просто произведение размерностей yesX и yesY;
		// но потом мы вычитаем (например, с коэффициентом 0.5) аналогичное для noX и noY
private:
	void MakeScore ();	// делает nScore по этому алгоритму
public:
	void SetScore (int nScore)  { this->nScore = nScore; }
		// устанавливает это nScore принудительно 
		// (и это должно сохраняться в подзадачах-потомках)
	int GetScore ()  { return nScore; }
	void AddYesX (int newX);
	void AddYesY (int newY);
	void AddNoX (int newX);
	void AddNoY (int newY);
	MyLittleSubTask* MakeRight ();
		// делает правую подзадачу, изменяя себя под левую;
		// возврат NULL - если расширить не удалось, т.е. грид уже готов;
		// в этом случае есть смысл вызывать следующую функцию:
	MyGrid* MakeGrid ();	// естественно, грид делается на основе yesX и yesY
	friend ostream& operator<< (ostream& os, MyLittleSubTask& mlst);
};

bool operator< (MyLittleSubTask& mlst1, MyLittleSubTask& mlst2);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyArrayLittleSubTask - 

class MyArrayLittleSubTask : public CPtrArray {	// of MyLittleSubTask*
public:
	MyArrayLittleSubTask () : CPtrArray() {}
	virtual ~MyArrayLittleSubTask ();
	MyLittleSubTask* MyGet (int n) { return (MyLittleSubTask*)GetAt(n); }
	void MyAdd (MyLittleSubTask* pSTAdd); 
	//void MyAddPrim (MyLittleSubTask* pSTAdd); 
		// добавляет в порядке уменьшения произведений числа эл-тов ("границы");
		// точнее - согласно оператору > (для MyLittleSubTask)
	MyLittleSubTask* ExtractFirst ();
		// извлекает первую; возвращает NULL, если таковой нет
	MyLittleSubTask* GetFirst ();
		// в отличие от пред., просто возвращает первую, не извлекая её; NULL, если таковой нет
	friend ostream& operator<< (ostream& os, MyArrayLittleSubTask& arrsubtask);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyLittleTask - 

class MyLittleTask : public MyArrayLittleSubTask {
private:
	MyMatrix* pMatrix;			// kein Destructor!
	MyArrayGrid* pArrGrid;     	// kein Destructor!
public:
	MyLittleTask (MyMatrix* pMatrix, MyArrayGrid* pArrGrid);
	virtual ~MyLittleTask () {}
	int Step ();
		// пытаемся сделать очередной шаг, беря первую подзадачу из списка;
		// забываем про ППЗ - но включаем в порядке уменьшения колбасистости;
		// возврат:	0 - ОК, новый грид, включили его в arrGrid;
		//			1 - новый грид был, но в arrGrid он уже был включён;
		//			2 - обычное завершение (в отличие от старого варианта)
		//			3 - сгенерировали неправильный грид ("неполный")
		//			4 - список подзадач был пуст;
	void SpecialAddSubTask (int x, int y);
		// в качестве самой первой добавляет подзадачу с единственной (заданной) клеткой
		// и бесконечной колбасистостью
	friend ostream& operator<< (ostream& os, MyLittleTask& littletask);
};

#endif
