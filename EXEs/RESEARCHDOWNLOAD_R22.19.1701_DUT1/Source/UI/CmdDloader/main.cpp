// InvokeDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Download.h"

CDownload g_fnDownload;
CCmdHelper g_fnCmdHelper;
CEvnetHelper g_fnEventHelper;
CConsoleHelper g_fnConsoleHelper;
CMD_PARAMS g_tCmdParams;


int _tmain( int argc, _TCHAR* argv[] )
{
    if ( g_fnCmdHelper.GetAllCmdParams( argv, argc ) )
    {
        if ( g_tCmdParams.bHelp )
        {
            g_fnCmdHelper.ShowUsage();
        }
        else
        {
            CHKBOOL( g_fnEventHelper.CreateAllEvent() );
            CHKBOOL( g_fnConsoleHelper.InitConsole() );
            g_fnDownload.Download();
            g_fnEventHelper.CloseAllEvent();
            g_fnConsoleHelper.ReleaseConsole();
        }
    }
    else
    {
        g_fnCmdHelper.ShowUsage();
    }
    return 0;
}
