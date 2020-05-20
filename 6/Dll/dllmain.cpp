#include "pch.h"
#include <iostream>
#include <string>
#include <ole2.h>

using namespace std;

extern "C" __declspec(dllexport) wchar_t* SpecifyDocPath();
extern "C" __declspec(dllexport) void RunMSWord();
HRESULT AutoWrap(int autoType, VARIANT* pvResult, IDispatch* pDisp, LPOLESTR ptName, int cArgs...);

wchar_t* SpecifyDocPath()
{
    WIN32_FIND_DATA findFileData;
    wchar_t* path = new wchar_t[512];
    bool gotFile = false;
    cout << "Determine the full folder path to search a doc file" << endl;
    do
    {
        wcin >> path;

        HANDLE fileFoundHandler = FindFirstFile(path, &findFileData);

        if (fileFoundHandler != INVALID_HANDLE_VALUE)
        {
            gotFile = true;
            wcout << L"Opening file: " << findFileData.cFileName << endl;
            FindClose(fileFoundHandler);
        }
        else
        {
            cout << "Invalid path has been specified, try again" << endl;
        }
    } while (!gotFile);

    return path;
}

void RunMSWord()
{
    // Initialize COM for this thread...
    CoInitialize(NULL);

    wchar_t* wPath = SpecifyDocPath();
    wcout << L"RunMSWord: " << wPath << endl;

    // Get CLSID for Word.Application...
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(L"Word.Application", &clsid);
    if (FAILED(hr)) {
        ::MessageBox(NULL, TEXT("CLSIDFromProgID() failed"), TEXT("Error"), 0x10010);
        return;
    }

    // Start Word and get IDispatch...
    IDispatch* pWordApp;
    hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&pWordApp);
    if (FAILED(hr)) {
        ::MessageBox(NULL, TEXT("Word not registered properly"), TEXT("Error"), 0x10010);
        return;
    }

    // Make Word visible
    {
        VARIANT x;
        x.vt = VT_I4;
        x.lVal = 1;
        AutoWrap(DISPATCH_PROPERTYPUT, NULL, pWordApp, (LPOLESTR)L"Visible", 1, x);
    }

    // Get Documents collection
    IDispatch* pDocs;
    {
        VARIANT result;
        VariantInit(&result);
        AutoWrap(DISPATCH_PROPERTYGET, &result, pWordApp, (LPOLESTR)L"Documents", 0);

        pDocs = result.pdispVal;
    }

    IDispatch* pDoc;
    {
        VARIANT result;
        VariantInit(&result);
        VARIANT x;
        x.vt = VT_BSTR;
        x.bstrVal = SysAllocString(wPath);

        AutoWrap(DISPATCH_METHOD, &result, pDocs, (LPOLESTR)L"Open", 1, x);
        pDoc = result.pdispVal;
        SysFreeString(x.bstrVal);
    }

    pDoc->Release();
    pDocs->Release();
    pWordApp->Release();

    // Uninitialize COM for this thread...
    CoUninitialize();
}

HRESULT AutoWrap(int autoType, VARIANT* pvResult, IDispatch* pDisp, LPOLESTR ptName, int cArgs...)
{
    // Begin variable-argument list...
    va_list marker;
    va_start(marker, cArgs); //формируем список аргументов, передаваемых в cArgs

    if (!pDisp) {
        MessageBox(NULL, TEXT("NULL IDispatch passed to AutoWrap()"), TEXT("Error"), 0x10010);
        _exit(0);
    }
    // pDisp Ц это объект, на котoром выполн€етс€ метод, напр. document
    // Variables used...
    DISPPARAMS dp = { NULL, NULL, 0, 0 }; //структура определ€ет параметры,
    // передаваемые в метод
    DISPID dispidNamed = DISPID_PROPERTYPUT;
    DISPID dispID;
    HRESULT hr;
    char buf[200];
    char szName[200];

    // Convert down to ANSI
    WideCharToMultiByte(CP_ACP, 0, ptName, -1, szName, 256, NULL, NULL);

    // Get DISPID for name passed...
    hr = pDisp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispID);
    if (FAILED(hr)) {
        sprintf_s(buf, "IDispatch::GetIDsOfNames(\"%s\") failed w/err0x%08lx", szName, hr);
        _exit(0);
        return hr;
    }

    // Allocate memory for arguments...
    VARIANT* pArgs = new VARIANT[cArgs + 1];

    // Extract arguments...
    for (int i = 0; i < cArgs; i++) {
        pArgs[i] = va_arg(marker, VARIANT);
    }

    // Build DISPPARAMS
    dp.cArgs = cArgs;
    dp.rgvarg = pArgs;

    // Handle special-case for property-puts!
    if (autoType & DISPATCH_PROPERTYPUT) {
        dp.cNamedArgs = 1;
        dp.rgdispidNamedArgs = &dispidNamed;
    }

    // Make the call!
    hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, autoType, &dp, pvResult, NULL, NULL);
    if (FAILED(hr)) {
        sprintf_s(buf, "IDispatch::Invoke(\"%s\"=%08lx) failed w/err 0x%08lx", szName, dispID, hr);
        _exit(0);
        return hr;
    }
    // End variable-argument section...
    va_end(marker);

    delete[] pArgs;

    return hr;
}
