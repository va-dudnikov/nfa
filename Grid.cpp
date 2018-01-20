
#include "stdafx.h"
#include "Grid.h"

// #include "Utily.h"
// #include "Random.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyGrid - 
		
bool MyGrid::ExistsSquare (int x, int y) {
	return GetXXX(x) && GetYYY(y);
}

bool operator<= (MyGrid& grid1, MyGrid& grid2) {
	//return grid1.GetXX()<=grid2.GetXX() && grid1.GetYY()<=grid2.GetYY();
	return grid1.XXX<=grid2.XXX && grid1.YYY<=grid2.YYY;
}

bool operator== (MyGrid& grid1, MyGrid& grid2) {
	//return grid1.GetXX()==grid2.GetXX() && grid1.GetYY()==grid2.GetYY();
	return grid1.XXX==grid2.XXX && grid1.YYY==grid2.YYY;
}

ostream& operator<< (ostream& os, MyGrid& grid) {
	os << grid.XXX << "-" << grid.YYY;
	return os;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyArrayGrid - 

MyArrayGrid::~MyArrayGrid () {
    for (int i=0; i<GetSize(); i++) {
        MyGrid* pGrid = (MyGrid*)GetAt(i);
        delete pGrid;
    }
}
	
int MyArrayGrid::ExistsGrid (MyGrid* grid) {
    for (int i=0; i<GetSize(); i++) {
        MyGrid* pGrid = (MyGrid*)GetAt(i);
		if (*grid == *pGrid) return i;
		if (*grid <= *pGrid) return -1;
	}
	return -2;
}
	
int MyArrayGrid::GridBySqare (int x, int y) {
    for (int i=0; i<GetSize(); i++) {
        MyGrid* pGrid = (MyGrid*)GetAt(i);
		if (pGrid->ExistsSquare(x,y)) return i;
	}
	return -2;
}

ostream& operator<< (ostream& os, MyArrayGrid& arrgrid) {
    for (int i=0; i<arrgrid.GetSize(); i++) {
        MyGrid* pGrid = (MyGrid*)arrgrid.GetAt(i);
        os << *pGrid << endl;
    }
	return os;
}








/*	
MyGrid::MyGrid (MyXY* pX, MyXY* pY) {
	this->pX = new MyXY(*pX);
	this->pY = new MyXY(*pY);
}
	
int MyGrid::GetKolvo () { 
	return pX->GetDimGrid() * pY->GetDimGrid();
}

bool MyGrid::operator== (MyGrid& other) {
	return (*pX==*(other.pX)) && (*pY==*(other.pY));
}

bool MyGrid::operator|| (MyGrid& other) {
	return *pX==*(other.pX);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MyGridNom - 
	
MyGridNom::MyGridNom (int Nomer, MyXY* pX, MyXY* pY) : MyGrid(pX,pY) {
	this->Nomer = Nomer;
}
	
MyGridNom::MyGridNom (int Nomer, MyGrid& grid) : 
	MyGrid(grid.GetXX(),grid.GetYY()) 
{
	this->Nomer = Nomer;
}

ostream& operator<< (ostream& os, MyGridNom& grid) {
	os << *grid.pX << "-" << *grid.pY;
	return os;
}
*/
