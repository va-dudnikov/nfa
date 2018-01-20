
#if !defined(_SubSet)
#define _SubSet

#include <fstream.h>
#include <iostream.h>
#include <iomanip.h>



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int maxDim = 50;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyXY - 

class MyXY {
private:
	int nDim, nDimGrid;
	bool* Array;
public:
	MyXY (int nDim);				// заодно инициализация всеми false
	MyXY (int nDim, int nDimGrid);	// заодно инициализация случайными зн.
	MyXY (MyXY& copy);
	virtual ~MyXY ();
private:
	void InitMemory (int nDim);		// 
public:
	void InitRandom (int nDimGrid);	// 
	// void SetFalse (int n) { Array[n] = false; }	// не надо такое использовать вааще!!!
	// void SetTrue  (int n) { Array[n] = true;  }	// не надо такое использовать вааще!!!
	void SetNewTrue  (int n) { 
		if (Array[n]) return;  
		Array[n] = true;
		nDimGrid++;
	}
	bool Get (int n) { return Array[n]; }
	int GetDim () { return nDim; }
	int GetDimGrid () { return nDimGrid; }
		// "при сомнении" надо сначала вызывать MakeDimGrid()
	bool Empty () { return nDimGrid<=0; }
	int GetByNumber (int nom);	
		// возвращает номер (от 0 до nDim-1) nom-й единицы грида;
		// при этом nom считаем тоже начиная с 0
private:
	int MakeDimGrid ();	
		// установка nDimGrid по фактическому количеству числу единиц 
		// (с возвратом знач.)
public:
	friend bool operator<= (MyXY& xy1, MyXY& xy2);
	friend bool operator== (MyXY& xy1, MyXY& xy2);
	friend ostream& operator<< (ostream& os, MyXY& xy);
};



/*
class MyXY {
public:
	MyXY (MyXY& copy);
	int GetDim () { return nDim; }
	void InitRandom ();		// с установкой случайной размерности nDimGrid
	void InitTrueFalse (bool bInit);		// с установкой размерности nDimGrid=nDim
	bool operator== (MyXY& other);
	friend ostream& operator<< (ostream& os, MyXY& xy);
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSubSet - 

class MySubSet : public MyXY {
private:
	bool Inited;
public:
	MySubSet (int nDim);
	bool* MakeNext ();	
		// если очередное подмножество сделать не удалось, возвращает NULL
private:
	void Init (int n, int m);
public:
	bool operator== (MySubSet& other);
	friend ostream& operator<< (ostream& os, MySubSet& ss);
};
*/


#endif