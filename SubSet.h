
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
	MyXY (int nDim);				// ������ ������������� ����� false
	MyXY (int nDim, int nDimGrid);	// ������ ������������� ���������� ��.
	MyXY (MyXY& copy);
	virtual ~MyXY ();
private:
	void InitMemory (int nDim);		// 
public:
	void InitRandom (int nDimGrid);	// 
	// void SetFalse (int n) { Array[n] = false; }	// �� ���� ����� ������������ �����!!!
	// void SetTrue  (int n) { Array[n] = true;  }	// �� ���� ����� ������������ �����!!!
	void SetNewTrue  (int n) { 
		if (Array[n]) return;  
		Array[n] = true;
		nDimGrid++;
	}
	bool Get (int n) { return Array[n]; }
	int GetDim () { return nDim; }
	int GetDimGrid () { return nDimGrid; }
		// "��� ��������" ���� ������� �������� MakeDimGrid()
	bool Empty () { return nDimGrid<=0; }
	int GetByNumber (int nom);	
		// ���������� ����� (�� 0 �� nDim-1) nom-� ������� �����;
		// ��� ���� nom ������� ���� ������� � 0
private:
	int MakeDimGrid ();	
		// ��������� nDimGrid �� ������������ ���������� ����� ������ 
		// (� ��������� ����.)
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
	void InitRandom ();		// � ���������� ��������� ����������� nDimGrid
	void InitTrueFalse (bool bInit);		// � ���������� ����������� nDimGrid=nDim
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
		// ���� ��������� ������������ ������� �� �������, ���������� NULL
private:
	void Init (int n, int m);
public:
	bool operator== (MySubSet& other);
	friend ostream& operator<< (ostream& os, MySubSet& ss);
};
*/


#endif