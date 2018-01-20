
#if !defined(_Big)
#define _Big

#include <fstream.h>
#include <iostream.h>
#include <iomanip.h>

#include "Matrix.h"
#include "Little.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyWordArray - 

class MyWordArray : public CWordArray {
public:
	MyWordArray () : CWordArray() {}
	MyWordArray (MyWordArray& copy);
	bool ExistsNom (int nom);
	operator= (MyWordArray& copy);
	void DeleteNom (int nom);
private:
	void AddCopy (MyWordArray& copy);
};

bool operator>= (MyWordArray& wa1, MyWordArray& wa2);
bool operator== (MyWordArray& wa1, MyWordArray& wa2);
double operator&& (MyWordArray& wa1, MyWordArray& wa2);	// для простого варианта "списочной метрики"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MySubTask (Big) - 

class MySubTask {
private:
	MyMatrix* pMatrix;			// kein Destructor!
	MyArrayGrid* pArrayGrid;	// kein Destructor!
	MyWordArray arrYes, arrNo;	
		// у них, конечно, совсем другой смысл, чем в Little! 
		// здесь это №№ гридов, которые мы берём из *pArrayGrid
	MyMatrix matrThis;			// уже заполненные в этой подзадаче элементы 1
public:
	MySubTask (MyMatrix* pMatrix, MyArrayGrid* pArrayGrid);
		// это - самая исходная инициализация подзадачи
	MySubTask (MySubTask& copy);
	virtual ~MySubTask () {}
	int SelectGridSimple ();
		// выбирает грид из числа ещё не использованных:
		// смотрим, сколько новых элементов 1 добавится к матрице matrThis
		// если в массиве *pArrayGrid нет таких, которые добавили бы хоть одну 1
		// (например - вообще нет НЕ входящих в arrYes либо в arrNo),
		// то возвращаем -1
	//int BoundSimple ()  { return *pMatrix - matrThis; }
	int BoundSimple ();
		// самая простая нижняя граница - просто число не хватающих 1
	int BoundSecond ();
		// следующая версия нижней границы
	bool Ready ()  { return BoundSimple()<=0; }
	int GetSumma ()  { return matrThis.GetSumma(); }
		// тоже как бы граница - просто сумма всех 1
	int GetSizeYes () { return arrYes.GetSize(); }
	MyWordArray& GetYes () { return arrYes; }
	MyWordArray& GetNo () { return arrNo; }
	MySubTask* MakeRight ();
	MySubTask* MakeRight (int nom);
		// делает правую подзадачу, изменяя себя под левую;
		// возврат NULL - если расширить не удалось,
		// т.е. ни один из уже готовых гридов добавить нельзя;
		// в последнем случае "мы" не изменяемся
	bool GetRandomSquare (int& x, int& y);
		// случайно выбирает клетку - из входящих в *pMatrix и не входящих в matrThis
private:
	void AddYes (int nom);
	void AddNo  (int nom);
public:
	friend ostream& operator<< (ostream& os, MySubTask& subtask);
};

//bool operator<= (MySubTask& st1, MySubTask& st2);
	// меньше или равна соотв граница 
bool operator== (MySubTask& st1, MySubTask& st2);
	// совсем не то что предыдущий метод
double operator&& (MySubTask& st1, MySubTask& st2);
	// простой вариант "списочной" метрики на подзадачах


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyArraySubTask (Big) - 

class MyArraySubTask : public CPtrArray {	// of MySubTask*
public:
	MyArraySubTask () : CPtrArray() {}
	virtual ~MyArraySubTask ();
	MySubTask* MyGet (int n) { return (MySubTask*)GetAt(n); }
	//void MyAdd (MySubTask* pSTAdd, int nMin=0); 
	void MyAdd (MySubTask* pAdd); 
		// добавляет в порядке увеличения значения BoundSecond() ("границы");
		// при этом следим, чтобы не было дублирования задач
	MySubTask* ExtractFirst ();
		// извлекает первую; возвращает NULL, если таковой нет
	//MySubTask* GetFirst ();
		// в отличие от пред., просто возвращает первую, не извлекая её; NULL, если таковой нет
	void Clean ();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyTask (Big) - 

class MyTask : public MyArraySubTask {
private:
	MyMatrix matrMain;
	MyArrayGrid arrGridMain;
	MyLittleTask ltMain;
	// далее - для текущего ответа
	MyWordArray arrYesOpt;
	int nOpt;	// число гридов в текущем оптимальном решении
public:
	MyTask (MyMatrix& matr);
	~MyTask () {}
private:
	int Step ();
		// пытаемся сделать очередной шаг, беря первую подзадачу из списка;
		// не забываем про ППЗ;
		// возврат:	0 - ОК, сделали что хотели; но, скорее всего, ППЗ до конца не довели
		//			1 - ППЗ довели до конца, но не заменили текущее оптимальное решение
		//			2 - ещё лучше, заменили текущее оптимальное решение;
		//			4 - список подзадач был пуст;
	void SubStep ();
		// из самой первой подзадачи берёт незаполненную клетку
		// и в качестве новой самой первой включает малую подзадачу с единственной этой клеткой
	int ProofReady (MySubTask* stR, bool bFirst);
		// возврат 0,1,2 - аналогично Step()
		// при этом если решение готово (пусть не оптимальное), то подзадача удаляется,
		// а если не готово - то дабавляется в массив подзадач самой первой
public:
	int BigStep ();
	friend ostream& operator<< (ostream& os, MyTask& task);
};

#endif
