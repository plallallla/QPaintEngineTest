#pragma once


// Ϊ����QTҲ��ʹ�������̬�⣬����2���»��ߣ�
#ifdef _USRDLL
#define FUNCTION_DLL_API __declspec(dllexport)
#else
#define FUNCTION_DLL_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif
	//void FUNCTION_DLL_API CDCtest(HDC);
	void FUNCTION_DLL_API CDCtest(HWND);
	void FUNCTION_DLL_API DrawRed(HWND);
	void FUNCTION_DLL_API DrawBlue(HWND);
	void FUNCTION_DLL_API DrawGreen(HWND);

	//HBITMAP FUNCTION_DLL_API CDCresult(HWND);
#ifdef __cplusplus
}
#endif