#include <iostream>
#include <fstream>
#include <tchar.h>
#include <Windows.h>

using namespace std;


SERVICE_STATUS          serviceStatus;
SERVICE_STATUS_HANDLE   serviceStatusHandle;

DWORD ServiceInitialization(DWORD   argc, LPTSTR* argv, DWORD* specificError);
void WINAPI ServiceStart(DWORD argc, LPTSTR* argv);
void WINAPI ServiceCtrlHandler(DWORD opcode);
void SvcDebugOut(LPSTR String, DWORD Status);
void AddLogMessage(char* text);
void ShutdownSystem();


int _tmain(int argc, _TCHAR* argv[])
{
    SERVICE_TABLE_ENTRY ServiceTable[1];
    ServiceTable[0].lpServiceName = (wchar_t*)TEXT("SERV_G");
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceStart;

    if (!StartServiceCtrlDispatcher(ServiceTable))
    {
        printf("Error in registration:%x", GetLastError());
    }
    else
    {
        printf("Registration OK");
    }

    return 0;
}


DWORD ServiceInitialization(DWORD argc, LPTSTR* argv, DWORD* specificError)
{
    *argv;
    argc;
    specificError;
    // should return status...
    return 0;
}

void WINAPI ServiceStart(DWORD argc, LPTSTR* argv)
{
    DWORD status;
    DWORD specificError;
    // Type of service, application or driver...
    serviceStatus.dwServiceType = SERVICE_WIN32;
    // The service is starting...
    serviceStatus.dwCurrentState = SERVICE_START_PENDING;
    // The service can be stopped & can be paused and continued.
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
    serviceStatus.dwWin32ExitCode = 0;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCheckPoint = 0;
    serviceStatus.dwWaitHint = 0;

    serviceStatusHandle = RegisterServiceCtrlHandler(TEXT("SERV_G"), ServiceCtrlHandler);

    if (serviceStatusHandle == (SERVICE_STATUS_HANDLE)0)
    {
        SvcDebugOut((char*)"[MY_SERVICE] RegisterServiceCtrlHandler() failed, error: %d.\n", GetLastError());
        return;
    }
    else
        printf("RegisterServiceCtrlHandler() looks OK.\n");

    //===============================================================
    // Initialization code goes here. Return the status...
    status = ServiceInitialization(argc, argv, &specificError);
    // Handle error condition
    if (status != NO_ERROR)
    {
        // The service is not running...
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        serviceStatus.dwCheckPoint = 0;
        serviceStatus.dwWaitHint = 0;
        serviceStatus.dwWin32ExitCode = status;
        serviceStatus.dwServiceSpecificExitCode = specificError;
        SetServiceStatus(serviceStatusHandle, &serviceStatus);
        return;
    }

    // Initialization complete - report running status.
    serviceStatus.dwCurrentState = SERVICE_RUNNING;
    serviceStatus.dwCheckPoint = 0;
    serviceStatus.dwWaitHint = 0;

    if (!SetServiceStatus(serviceStatusHandle, &serviceStatus))
    {
        status = GetLastError();
        SvcDebugOut((char*)"SetServiceStatus() error: %ld\n", status);
    }
    else
    {
        printf("SetServiceStatus() looks OK.\n");
    }
    // By assuming starting and initializing the service is OK, this
    // is where the service does its work...
    SvcDebugOut((char*)"Returning the Main Thread.\n", 0);
    return;
}

void  WINAPI ServiceCtrlHandler(DWORD request)
{
    switch (request)
    {
    case SERVICE_CONTROL_STOP:
        AddLogMessage((char*)"Stopped.");

        serviceStatus.dwWin32ExitCode = 0;
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(serviceStatusHandle, &serviceStatus);
        ShutdownSystem();
        return;
    case SERVICE_CONTROL_SHUTDOWN:
        AddLogMessage((char*)"Shutdown.");

        serviceStatus.dwWin32ExitCode = 0;
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(serviceStatusHandle, &serviceStatus);
        return;
    }

    SetServiceStatus(serviceStatusHandle, &serviceStatus);
}

void SvcDebugOut(LPSTR String, DWORD Status)
{
    CHAR  Buffer[1024];

    if (strlen(String) < 1000)
    {
        sprintf_s(Buffer, String, Status);
        OutputDebugStringA(Buffer);
    }
    else
    {
        printf("String too long...\n");
    }
}

void AddLogMessage(char* text)
{
    ofstream out;
    out.open("C:\\file.txt");
    if (out.is_open())
    {
        out << text << std::endl;
    }
}

void ShutdownSystem()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    // ѕолучаем маркер текущего процесса.
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return;

    // ѕолучаем LUID дл€ привелегии завершени€ работы.
    LookupPrivilegeValue(
        NULL, //ищет привелегии в локальной системе
        SE_SHUTDOWN_NAME, //им€ привелегии
        &tkp.Privileges[0].Luid //получает LUID, по которому известна привилеги€ в системе,
    );

    tkp.PrivilegeCount = 1;  // будет установлена одна привелеги€
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // ѕолучаем привелегию завершени€ работы дл€ этого процесса.
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

    if (GetLastError() != ERROR_SUCCESS)
        return;

    /*SYSTEMTIME st;
    //ZeroMemory(&st, sizeof(st));
    st.wYear = 2000;
    st.wMonth = 1;
    st.wDay = 1;
    st.wHour = 12;
    if (!SetSystemTime(&st))
    {
        cerr << "SetSystemTime failed" << endl;
    }
    else
    {
        cout << "OK" << endl;
    }
    return;*/

    // Ўутдауним систему и заставл€ем все приложени€ закрытьс€.
    ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0);
}
