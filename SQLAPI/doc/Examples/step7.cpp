//  Cancel - Creates a new thread with long running query when the letter 'a' is typed.
//  Cancels a query when letter 'C' is typed.
//  All threads are terminated when the letter 'Q' is entered.
//
//  This program requires the multithread library.

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>

#include <iostream>

#include <SQLAPI.h>

void main(void);					/* Thread 1: main */
void KbdFunc(void );				/* Keyboard input, thread dispatch */
void LongQuery(void *addl);			/* Threads 2: Long running query */
void LongQueryCancel();
void ShutDown(void);				/* Program shutdown */
void WriteTitle();					/* Display title bar information */

HANDLE  hConsoleOut;                   /* Handle to the console */
HANDLE  hQueryMutex;                   /* "Query Running" mutex */
HANDLE  hScreenMutex;                  /* "Screen update" mutex  */

SAConnection	g_Conn;
SACommand		g_Cmd(&g_Conn);

// connection and query information
// modify here to provide your real information
const char *LONG_RUNNING_QUERY =
	"select * from test_bulk "
	" where fvarchar20 like '%hj' or fvarchar20 like '%dfds'"
	" or fvarchar20 in (select fvarchar20 from test_bulk)"
	" order by fvarchar20 desc, finteger desc";
const char *DATABASE = "test";
const char *USER = "tester";
const char *PASSWORD = "tester";
SAClient_t CLIENT = SA_DB2_Client;


void main()                            /* Thread One */
{
    /* Get display screen information & clear the screen.*/
    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteTitle();
    /* Create the mutexes and reset thread count. */
    hScreenMutex = CreateMutex(NULL, FALSE, NULL);   /* Cleared */
    hQueryMutex = CreateMutex(NULL, FALSE, NULL);     /* Cleared */

	try
	{
		// connect
		g_Conn.Connect(DATABASE, USER, PASSWORD, CLIENT);

		/* Start waiting for keyboard input to dispatch threads or exit. */
		KbdFunc();
	}
	catch(SAException &x)
	{
		std::cout << (const char*)x.ErrText() << std::endl;

		try
		{
			g_Conn.Rollback();
		}
		catch(SAException &)
		{
		}
	}

    /* All threads done. Clean up handles. */
    CloseHandle(hScreenMutex);
    CloseHandle(hQueryMutex);
    CloseHandle(hConsoleOut);
}

void ShutDown(void)
{
	// try to cancel the query if still running
	if(WaitForSingleObject(hQueryMutex, 75L) == WAIT_TIMEOUT)
		LongQueryCancel();
	else
		ReleaseMutex(hQueryMutex);
}

void KbdFunc(void)
{
    int         KeyInfo;

    do
    {
        KeyInfo = _getch();

        if(tolower(KeyInfo) == 'a')
			_beginthread(LongQuery, 0, NULL);

        if(tolower(KeyInfo) == 'c')
            LongQueryCancel();
    }
	while(tolower(KeyInfo) != 'q');

    ShutDown();
}

void LongQuery(void *addl)
{
	if(WaitForSingleObject(hQueryMutex, 75L) == WAIT_TIMEOUT)
	{
		WaitForSingleObject(hScreenMutex, INFINITE);
		std::cout << "query still running, use 'C' to cancel it." << std::endl;
		ReleaseMutex(hScreenMutex);
		return;
	}

	try
	{
		g_Cmd.setCommandText(LONG_RUNNING_QUERY);

		WaitForSingleObject(hScreenMutex, INFINITE);
		std::cout << "executing " << (const char*)g_Cmd.CommandText() << std::endl;
		ReleaseMutex(hScreenMutex);

		g_Cmd.Execute();

		WaitForSingleObject(hScreenMutex, INFINITE);
		std::cout << "executed" << std::endl;
		ReleaseMutex(hScreenMutex);
	}
	catch(SAException &x)
	{
		WaitForSingleObject(hScreenMutex, INFINITE);
		std::cout << (const char*)x.ErrText() << std::endl;
		ReleaseMutex(hScreenMutex);
	}

	ReleaseMutex(hQueryMutex);
}

void LongQueryCancel()
{
	try
	{
		WaitForSingleObject(hScreenMutex, INFINITE);		
		std::cout << "CANCELLING..." << std::endl;
		ReleaseMutex(hScreenMutex);

		g_Cmd.Cancel();

		WaitForSingleObject(hScreenMutex, INFINITE);
		std::cout << "CANCELLED" << std::endl;
		ReleaseMutex(hScreenMutex);
	}
	catch(SAException &x)
	{
		WaitForSingleObject(hScreenMutex, INFINITE);
		std::cout << (const char*)x.ErrText() << std::endl;
		ReleaseMutex(hScreenMutex);
	}
}

void WriteTitle()
{
    char    NThreadMsg[1024];

    sprintf(NThreadMsg, "Press 'A' to start a long running query in a thread, 'C' to cancel it, 'Q' to quit.");
    SetConsoleTitle(NThreadMsg);
}
