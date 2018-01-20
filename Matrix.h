
#if !defined(_Matrix)
#define _Matrix

#include <fstream>
#include <iostream>
#include <iomanip>

#include "Grid.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyMatrix - 

class MyMatrix {
private:
	int nDimX, nDimY;
	bool* Array;
	int nSumma;	
		// суммарное число заполненных клеток; 
		// обнуление поля проводится только в InitMemory(), 
		// а увеличение - только в SetTrue() !!!
	int* costX;
	int* costY;	
		// просто общее число true в строках и столбцах;
		// аналогично предыдущему - 
		// обнуление поля проводится только в InitMemory(), 
		// а увеличение - только в SetTrue() !!!
	// !!! ОЧЕНЬ важно!!!
	// зд. можно ввести "вторичную колбасистость" -
	// т.е. сумму стоимостей столюцов, соответсвующих 1 в нашей строке;
	// это ЯКОБЫ используется при выборе оптимального варианта Extand -
	// но при этом же можно использовать с генетически подбираемыми коэффициентами
	// оба варианта этой "колбасистости"
public:
	MyMatrix (int nDimX, int nDimY);				// заодно инициализация всеми false
	MyMatrix (int nDimX, int nDimY, int kolGrid);	// заодно инициализация случайными значениями
	MyMatrix (MyMatrix& copy);
	virtual ~MyMatrix ();
private:
	void InitMemory (int nDimX, int nDimY);
	int MakeIndex (int nX, int nY);
public:
	void SetTrue (int nX, int nY);
	bool Get (int nX, int nY);
	int GetDimX () { return nDimX; }
	int GetDimY () { return nDimY; }
	int GetSumma () { return nSumma; }
	int GetCostX (int nX) { return costX[nX]; }
	int GetCostY (int nY) { return costY[nY]; }
	//
	void AddGrid (MyGrid& grid);
	void AddRandomGrid ();
	//void InitRandom (int kolGrid);
 	//MyGrid* MakeGridRnd ();
		// возвращает НОВЫЙ грид - или NULL, если не удалось;
		// а сам алгоритм см. в тексте, 
		// он, конечно, отличается от алгоритма из MyTwoMatrix::MakeGrid()
	MyGrid* MakeGridRnd (int nX, int nY);
		// в новой версии то же самое название бум использовать
		// для значительно более простого алгоритма инициализации случайного грида;
		// но: задана стартовая клетка
private:
	// две следующие возвращают возможную для увеличения грида координату - первую попавшуюся;
	// или -1, если невозможно ПО ЭТОЙ координате увеличить
	int ExtandXRnd (MyGrid* pGrid);
	int ExtandYRnd (MyGrid* pGrid);
public:
	int HowManyNew (MyGrid& grid);
		// сколько в этом гриде 1, НЕ входящих в нашу матрицу
	friend ostream& operator<< (ostream& os, MyMatrix& matrix);
};

int operator- (MyMatrix& matr1, MyMatrix& matr2);
	// считаем число 1 в matr1, не входящих в matr2

#endif
