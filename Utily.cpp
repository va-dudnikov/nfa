
#include "stdafx.h"
#include "Utily.h"
#include <windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Obmen (int& n1, int& n2) {
	int n = n1;
	n1 = n2;
	n2 = n;
}

void Obmen (double& r1, double& r2) {
	double r = r1;
	r1 = r2;
	r2 = r;
}

bool ObmenIf (int& n1, int& n2) {
	if (n1>=n2) return false;
	::Obmen(n1,n2);
	return true;
}

bool Nulla (double r, double eps) { return r>=0.0 ? r<=eps : r>=-eps; }

bool Nulla (double r) { return Nulla(r,1.0e-6); }

double GetInfty () { return 1.0e8; }

double GetMinusInfty () { return -GetInfty(); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BoundsInt (int& n, int nMin, int nMax) {
	if (n<=nMin) n = nMin;
	else if (n>=nMax) n = nMax;
}

void Bounds (double& r, double rMin, double rMax) {
	if (r<=rMin) r = rMin;
	else if (r>=rMax) r = rMax;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MyError (int nCode) {
	CString str;
	if		(nCode==1)	str = "Neverniy index";
	else if	(nCode==2)	str = "Nevernoe vichislenie razmernosti grida";
	else if	(nCode==3)	str = "Ne nado otnositsa k neresh.zad. kak k resh.";
	else if	(nCode==4)	str = "Nomer grida dolzhen bhit' - a ego net...";
	else if	(nCode==5)	str = "Ne udalos' vibrat' grid - hotya d.b.";
	else if	(nCode==6)	str = "Neverno formiruetsya grid";
	else if	(nCode==7)	str = "Ne udalos' postupit' s gridom kak nado";
	else if	(nCode==8)	str = "Popitka obrabativat' matrici raznih razmernostey";
	else if	(nCode==9)	str = "V massiv podzadach popala reshyonnaya zadacha";
	else if	(nCode==10)	str = "Vidimo zadana dlya obrabotki pustaya matrica";
	else if	(nCode==11)	str = "Grid vklyuchyon ne s poslednim nomerom";
	else if	(nCode==12)	str = "Ne nashli ozhidavshuyusya kletku";
	else				str = "An error...";
	cout << str << endl;
	exit(1);
}
