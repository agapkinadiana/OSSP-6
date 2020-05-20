// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <windows.h>                // Program Demonstrates Late Bound OLE COM Access To MS Excel Spreadsheet Using C++.
#include <tchar.h>                  // "Hello, World! Is Written To Cell A1 Of Sheet #1 In Visible Workbook. IDispatch
#include <cstdio>                   // Interface Using GetIDsOfNames() And Invoke() Used Throughout.
#include <OAIdl.h>
const CLSID CLSID_XLApplication = 
{ 
	0x00024500,
	0x0000,
	0x0000,
	{
		0xC0,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x46
	} 
}; // CLSID of Excel
const IID IID_Application = 
{ 
	0x000208D5,
	0x0000,
	0x0000,
	{
		0xC0,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x46
	}
}; // IID of _Application


extern "C" _declspec(dllexport) void StartExcel()
{
	DISPPARAMS NoArgs = { NULL,NULL,0,0 }; // This variable is used in easiest Invoke() call when the method has no parameters.  When
	IDispatch* pXLApp = NULL;            // using the IDispatch interface in conjunction with Invoke() method parameters must be loaded
	DISPPARAMS DispParams;             // into a DISPPARAMS struct.  The actual parameters are loaded into VARIANTs, and one of the
	VARIANT CallArgs[1];               // members of the DISPPARAMS struct is a pointer to the array of VARIANT.  The other members
	VARIANT vResult;                   // of the DISPARAMS struct tell Invoke() how many parameters are being passed, as well as other
	DISPID dispid;                     // specifics such as the type of the call (propput, propget, etc.).
	HRESULT hr;

	//Ёта переменна€ используетс€ в вызове Easy Invoke(), когда метод не имеет параметров.
	//ѕри использовании интерфейса IDispatch в сочетании с Invoke() параметры метода должны быть загружены в структуру DISPPARAMS.
	//‘актические параметры загружаютс€ в варианты, и один из членов структуры DISPPARAMS €вл€етс€ указателем на массив VARIANT.
	//ƒругие члены структуры DISPPARAMS сообщают Invoke(), сколько параметров передаетс€, а также другие особенности, такие как тип вызова(propput, propget и т.д.).).


	CoInitialize(NULL);
	hr = CoCreateInstance(CLSID_XLApplication, NULL, CLSCTX_LOCAL_SERVER, IID_Application, (void**)& pXLApp);
	if (SUCCEEDED(hr))
	{
		OLECHAR* szVisible = (OLECHAR*)L"Visible";
		hr = pXLApp->GetIDsOfNames(IID_NULL, &szVisible, 1, GetUserDefaultLCID(), &dispid);
		if (SUCCEEDED(hr))
		{
			VariantInit(&CallArgs[0]);
			CallArgs[0].vt = VT_BOOL;
			CallArgs[0].boolVal = TRUE;
			DISPID dispidNamed = DISPID_PROPERTYPUT;
			DispParams.rgvarg = CallArgs;
			DispParams.rgdispidNamedArgs = &dispidNamed;
			DispParams.cArgs = 1;
			DispParams.cNamedArgs = 1;
			VariantInit(&vResult);       //  Call or Invoke _Application::Visible(true);
			hr = pXLApp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &DispParams, &vResult, NULL, NULL);
			OLECHAR* szWorkbooks = (OLECHAR*)L"Workbooks";
			hr = pXLApp->GetIDsOfNames(IID_NULL, &szWorkbooks, 1, GetUserDefaultLCID(), &dispid);
			if (SUCCEEDED(hr))
			{
				IDispatch* pXLBooks = NULL;    //  Get Workbooks Collection
				VariantInit(&vResult);       //  Invoke _Application::Workbooks(&pXLBooks) << returns IDispatch** of Workbooks Collection
				hr = pXLApp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &NoArgs, &vResult, NULL, NULL);
				if (SUCCEEDED(hr))
				{
					pXLBooks = vResult.pdispVal;
					IDispatch* pXLBook = NULL;  //  Try to add Workbook
					OLECHAR* szAdd = (OLECHAR*)L"Add";
					hr = pXLBooks->GetIDsOfNames(IID_NULL, &szAdd, 1, GetUserDefaultLCID(), &dispid);
					if (SUCCEEDED(hr))
					{
						VariantInit(&vResult);    //  Invoke Workbooks::Add(&Workbook)  << returns IDispatch** of Workbook Object
						hr = pXLBooks->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD | DISPATCH_PROPERTYGET, &NoArgs, &vResult, NULL, NULL);
						if (SUCCEEDED(hr))
						{
							pXLBook = vResult.pdispVal;
							OLECHAR* szActiveSheet = (OLECHAR*)L"ActiveSheet";
							hr = pXLApp->GetIDsOfNames(IID_NULL, &szActiveSheet, 1, GetUserDefaultLCID(), &dispid);
							if (SUCCEEDED(hr))
							{
								IDispatch* pXLSheet = NULL;  // Try To Get ActiveSheet
								VariantInit(&vResult);     // Invoke _Application::ActiveSheet(&pXLSheet);  << ret IDispatch** to Worksheet (Worksheet)
								hr = pXLApp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &NoArgs, &vResult, NULL, NULL);
								if (SUCCEEDED(hr))
								{
									pXLSheet = vResult.pdispVal;
									OLECHAR* szRange1 = (OLECHAR*)L"Range";
									hr = pXLSheet->GetIDsOfNames(IID_NULL, &szRange1, 1, GetUserDefaultLCID(), &dispid);
									if (SUCCEEDED(hr))
									{
										IDispatch* pXLRange = NULL;
										VariantInit(&vResult);
										CallArgs[0].vt = VT_BSTR,
											CallArgs[0].bstrVal = SysAllocString(L"A1");
										DispParams.rgvarg = CallArgs;
										DispParams.rgdispidNamedArgs = 0;
										DispParams.cArgs = 1;  // Try to get Range
										DispParams.cNamedArgs = 0;  // Invoke _Worksheet::Range("D3")  << returns IDispatch** to dispinterface Range
										hr = pXLSheet->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &DispParams, &vResult, NULL, NULL);
										if (SUCCEEDED(hr))
										{
											pXLRange = vResult.pdispVal;
											OLECHAR* szValue = (OLECHAR*)L"Value";
											hr = pXLRange->GetIDsOfNames(IID_NULL, &szValue, 1, GetUserDefaultLCID(), &dispid);
											if (SUCCEEDED(hr))
											{
												VariantClear(&CallArgs[0]);
												CallArgs[0].vt = VT_BSTR;
												CallArgs[0].bstrVal = SysAllocString(L"1");  //Try to set data to cell A1 using pXLRange
												DispParams.rgvarg = CallArgs;
												DispParams.rgdispidNamedArgs = &dispidNamed;
												DispParams.cArgs = 1;  // Try to write to Value member of Range dispinterface
												DispParams.cNamedArgs = 1;  // Invoke Range::Value(L"qwwenh")
												hr = pXLRange->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &DispParams, NULL, NULL, NULL);
												pXLRange->Release();
											}
										}
									}
									//pXLSheet->Release();


									OLECHAR* szRange2 = (OLECHAR*)L"Range";
									hr = pXLSheet->GetIDsOfNames(IID_NULL, &szRange2, 1, GetUserDefaultLCID(), &dispid);
									if (SUCCEEDED(hr))
									{
										IDispatch* pXLRange = NULL;
										VariantInit(&vResult);
										CallArgs[0].vt = VT_BSTR,
											CallArgs[0].bstrVal = SysAllocString(L"B1");
										DispParams.rgvarg = CallArgs;
										DispParams.rgdispidNamedArgs = 0;
										DispParams.cArgs = 1;  // Try to get Range
										DispParams.cNamedArgs = 0;  // Invoke _Worksheet::Range("D3")  << returns IDispatch** to dispinterface Range
										hr = pXLSheet->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &DispParams, &vResult, NULL, NULL);
										if (SUCCEEDED(hr))
										{
											pXLRange = vResult.pdispVal;
											OLECHAR* szValue = (OLECHAR*)L"Value";
											hr = pXLRange->GetIDsOfNames(IID_NULL, &szValue, 1, GetUserDefaultLCID(), &dispid);
											if (SUCCEEDED(hr))
											{
												VariantClear(&CallArgs[0]);
												CallArgs[0].vt = VT_BSTR;
												CallArgs[0].bstrVal = SysAllocString(L"1");  //Try to set data to cell A1 using pXLRange
												DispParams.rgvarg = CallArgs;
												DispParams.rgdispidNamedArgs = &dispidNamed;
												DispParams.cArgs = 1;  // Try to write to Value member of Range dispinterface
												DispParams.cNamedArgs = 1;  // Invoke Range::Value(L"qwwenh")
												hr = pXLRange->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &DispParams, NULL, NULL, NULL);
												pXLRange->Release();
											}
										}
									}
									//pXLSheet->Release();


									OLECHAR* szRange3 = (OLECHAR*)L"Range";
									hr = pXLSheet->GetIDsOfNames(IID_NULL, &szRange3, 1, GetUserDefaultLCID(), &dispid);
									if (SUCCEEDED(hr))
									{
										IDispatch* pXLRange = NULL;
										VariantInit(&vResult);
										CallArgs[0].vt = VT_BSTR,
											CallArgs[0].bstrVal = SysAllocString(L"C1");
										DispParams.rgvarg = CallArgs;
										DispParams.rgdispidNamedArgs = 0;
										DispParams.cArgs = 1;  // Try to get Range
										DispParams.cNamedArgs = 0;  // Invoke _Worksheet::Range("D3")  << returns IDispatch** to dispinterface Range
										hr = pXLSheet->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &DispParams, &vResult, NULL, NULL);
										if (SUCCEEDED(hr))
										{
											pXLRange = vResult.pdispVal;
											OLECHAR* szValue = (OLECHAR*)L"Value";
											hr = pXLRange->GetIDsOfNames(IID_NULL, &szValue, 1, GetUserDefaultLCID(), &dispid);
											if (SUCCEEDED(hr))
											{
												VariantClear(&CallArgs[0]);
												CallArgs[0].vt = VT_BSTR;
												CallArgs[0].bstrVal = SysAllocString(L"=A1+B1");  //Try to set data to cell A1 using pXLRange
												DispParams.rgvarg = CallArgs;
												DispParams.rgdispidNamedArgs = &dispidNamed;
												DispParams.cArgs = 1;  // Try to write to Value member of Range dispinterface
												DispParams.cNamedArgs = 1;  // Invoke Range::Value(L"qwwenh")
												hr = pXLRange->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &DispParams, NULL, NULL, NULL);
												pXLRange->Release();
											}
										}
									}
									pXLSheet->Release();
								}
							}
							pXLBook->Release();
						}
					}
					pXLBooks->Release();
				}
			}
			getchar();
		}
		VariantInit(&vResult);  // Try to do _Application::Close()
		hr = pXLApp->Invoke(0x0000012e, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &NoArgs, &vResult, NULL, NULL);
		pXLApp->Release();
	}
	CoUninitialize();

	return;
}
