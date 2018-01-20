// NFA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NFA.h"
#include "Little.h"
#include "Big.h"
#include "Random.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		CString strHello;
		strHello.LoadString(IDS_HELLO);
		cout << (LPCTSTR)strHello << endl;
		                
        // MyMatrix matr(13,21,25);
        // MyMatrix matr(15,24,29);
        MyMatrix matr(29,54,47);
        cout << matr;

		/*
        MyArrayGrid arrGrid;            
        MyLittleTask lt(&matr,&arrGrid);
        cout << lt;
        for (int i=0; i<1000; i++) {
            int j = lt.Step();
            cout << endl << "return of step #" << i << ": " << j;
            if (j>3) break;
            cout << lt;
        }
		*/

        cout << endl;
		MyTask task(matr);
        cout << task;
        for (int i=0; i<1000; i++) {
			task.BigStep();
			cout << i << ". " << task;
        }

	}

	return nRetCode;
}


