
#if !defined(_Grid)
#define _Grid

#include <fstream.h>
#include <iostream.h>
#include <iomanip.h>

#include "SubSet.h"



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyGrid - 

class MyGrid {
private:
	MyXY XXX;
	MyXY YYY;
public:
	MyGrid (int nDimX, int nDimY) : XXX(nDimX), YYY(nDimY) {}
		// ������ ������������� ����� false
	MyGrid (int nDimX, int nDimGridX, int nDimY, int nDimGridY) :
		XXX(nDimX,nDimGridX), YYY(nDimY,nDimGridY) {}
		// ������ ������������� ���������� ����������
	MyGrid (MyXY& XX, MyXY& YY) : XXX(XX), YYY(YY) {}
	//MyXY& GetXX () { return XXX; }
	//MyXY& GetYY () { return YYY; }
	// �� ��� ��� ��� �� �����... ������������ ������:
	bool GetXXX (int n)  { return XXX.Get(n); }
	bool GetYYY (int n)  { return YYY.Get(n); }
	void SetNewTrueXXX (int n)  { XXX.SetNewTrue(n); }
	void SetNewTrueYYY (int n)  { YYY.SetNewTrue(n); }
	bool ExistsSquare (int x, int y);
	friend bool operator== (MyGrid& grid1, MyGrid& grid2);
	friend bool operator<= (MyGrid& grid1, MyGrid& grid2);
	friend ostream& operator<< (ostream& os, MyGrid& grid);
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyArrayGrid - 

class MyArrayGrid : public CPtrArray {	// of MyGrid*
	// �������� ������ � ����� - �.�. ��� ������ ����� ��� ������������
public:
	MyArrayGrid () : CPtrArray() {}
	virtual ~MyArrayGrid ();
	MyGrid* MyGet (int n) { return (MyGrid*)GetAt(n); }
	int ExistsGrid (MyGrid* grid);
		// ���������� ��������� �, ��� -1, ���� ��� "�������", ��� -2, ���� �� �����
	int GridBySqare (int x, int y);
		// ���������� ������ ��������� �, ��� -2, ���� �� �����
	friend ostream& operator<< (ostream& os, MyArrayGrid& arrgrid);
};


#endif