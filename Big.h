
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
double operator&& (MyWordArray& wa1, MyWordArray& wa2);	// ��� �������� �������� "��������� �������"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MySubTask (Big) - 

class MySubTask {
private:
	MyMatrix* pMatrix;			// kein Destructor!
	MyArrayGrid* pArrayGrid;	// kein Destructor!
	MyWordArray arrYes, arrNo;	
		// � ���, �������, ������ ������ �����, ��� � Little! 
		// ����� ��� �� ������, ������� �� ���� �� *pArrayGrid
	MyMatrix matrThis;			// ��� ����������� � ���� ��������� �������� 1
public:
	MySubTask (MyMatrix* pMatrix, MyArrayGrid* pArrayGrid);
		// ��� - ����� �������� ������������� ���������
	MySubTask (MySubTask& copy);
	virtual ~MySubTask () {}
	int SelectGridSimple ();
		// �������� ���� �� ����� ��� �� ��������������:
		// �������, ������� ����� ��������� 1 ��������� � ������� matrThis
		// ���� � ������� *pArrayGrid ��� �����, ������� �������� �� ���� ���� 1
		// (�������� - ������ ��� �� �������� � arrYes ���� � arrNo),
		// �� ���������� -1
	//int BoundSimple ()  { return *pMatrix - matrThis; }
	int BoundSimple ();
		// ����� ������� ������ ������� - ������ ����� �� ��������� 1
	int BoundSecond ();
		// ��������� ������ ������ �������
	bool Ready ()  { return BoundSimple()<=0; }
	int GetSumma ()  { return matrThis.GetSumma(); }
		// ���� ��� �� ������� - ������ ����� ���� 1
	int GetSizeYes () { return arrYes.GetSize(); }
	MyWordArray& GetYes () { return arrYes; }
	MyWordArray& GetNo () { return arrNo; }
	MySubTask* MakeRight ();
	MySubTask* MakeRight (int nom);
		// ������ ������ ���������, ������� ���� ��� �����;
		// ������� NULL - ���� ��������� �� �������,
		// �.�. �� ���� �� ��� ������� ������ �������� ������;
		// � ��������� ������ "��" �� ����������
	bool GetRandomSquare (int& x, int& y);
		// �������� �������� ������ - �� �������� � *pMatrix � �� �������� � matrThis
private:
	void AddYes (int nom);
	void AddNo  (int nom);
public:
	friend ostream& operator<< (ostream& os, MySubTask& subtask);
};

//bool operator<= (MySubTask& st1, MySubTask& st2);
	// ������ ��� ����� ����� ������� 
bool operator== (MySubTask& st1, MySubTask& st2);
	// ������ �� �� ��� ���������� �����
double operator&& (MySubTask& st1, MySubTask& st2);
	// ������� ������� "���������" ������� �� ����������


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyArraySubTask (Big) - 

class MyArraySubTask : public CPtrArray {	// of MySubTask*
public:
	MyArraySubTask () : CPtrArray() {}
	virtual ~MyArraySubTask ();
	MySubTask* MyGet (int n) { return (MySubTask*)GetAt(n); }
	//void MyAdd (MySubTask* pSTAdd, int nMin=0); 
	void MyAdd (MySubTask* pAdd); 
		// ��������� � ������� ���������� �������� BoundSecond() ("�������");
		// ��� ���� ������, ����� �� ���� ������������ �����
	MySubTask* ExtractFirst ();
		// ��������� ������; ���������� NULL, ���� ������� ���
	//MySubTask* GetFirst ();
		// � ������� �� ����., ������ ���������� ������, �� �������� �; NULL, ���� ������� ���
	void Clean ();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyTask (Big) - 

class MyTask : public MyArraySubTask {
private:
	MyMatrix matrMain;
	MyArrayGrid arrGridMain;
	MyLittleTask ltMain;
	// ����� - ��� �������� ������
	MyWordArray arrYesOpt;
	int nOpt;	// ����� ������ � ������� ����������� �������
public:
	MyTask (MyMatrix& matr);
	~MyTask () {}
private:
	int Step ();
		// �������� ������� ��������� ���, ���� ������ ��������� �� ������;
		// �� �������� ��� ���;
		// �������:	0 - ��, ������� ��� ������; ��, ������ �����, ��� �� ����� �� ������
		//			1 - ��� ������ �� �����, �� �� �������� ������� ����������� �������
		//			2 - ��� �����, �������� ������� ����������� �������;
		//			4 - ������ �������� ��� ����;
	void SubStep ();
		// �� ����� ������ ��������� ���� ������������� ������
		// � � �������� ����� ����� ������ �������� ����� ��������� � ������������ ���� �������
	int ProofReady (MySubTask* stR, bool bFirst);
		// ������� 0,1,2 - ���������� Step()
		// ��� ���� ���� ������� ������ (����� �� �����������), �� ��������� ���������,
		// � ���� �� ������ - �� ����������� � ������ �������� ����� ������
public:
	int BigStep ();
	friend ostream& operator<< (ostream& os, MyTask& task);
};

#endif
