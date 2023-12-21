#include "pch.h"
#include "Test.h"
#include <iostream>

void FUNCTION_DLL_API CDCtest(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	HBRUSH hbrRed = CreateSolidBrush(RGB(0, 255, 255));
	HDC hDc = GetDC(hwnd);
	FillRect(hDc, &rect, hbrRed);
}

void FUNCTION_DLL_API DrawBlue(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	HBRUSH hbrRed = CreateSolidBrush(RGB(0, 0, 255));
	HDC hDc = GetDC(hwnd);
	FillRect(hDc, &rect, hbrRed);
}

void FUNCTION_DLL_API DrawGreen(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	HBRUSH hbrRed = CreateSolidBrush(RGB(0, 255, 0));
	HDC hDc = GetDC(hwnd);
	FillRect(hDc, &rect, hbrRed);
}

void FUNCTION_DLL_API DrawRed(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	HBRUSH hbrRed = CreateSolidBrush(RGB(255, 0, 0));
	HDC hDc = GetDC(hwnd);
	FillRect(hDc, &rect, hbrRed);
}

//HBITMAP FUNCTION_DLL_API CDCresult(HWND hwnd)
//{
//	//hdc = BeginPaint(hwnd, &ps);
//	//��ʾDC���
//	HDC hdc = GetDC(hwnd);
//	//�ڴ�DC���
//	HDC hdcMem = CreateCompatibleDC(hdc);
//
//
//
//	HBITMAP hbitmap = CreateCompatibleBitmap(hdcMem, 100, 100);
//	return hbitmap;
//}

//LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	PAINTSTRUCT ps;
//	HDC hdc, hdcMem;
//	HBITMAP hBitmap;
//	BITMAP bmp;
//
//	switch (uMsg)
//	{
//	case WM_PAINT:
//		hdc = BeginPaint(hwnd, &ps);
//		hdcMem = CreateCompatibleDC(hdc);
//		//hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_GIRL));
//		hBitmap = (HBITMAP)LoadImage(NULL, TEXT("Girl.bmp"), IMAGE_BITMAP, 0, 0,
//			LR_LOADFROMFILE);
//		SelectObject(hdcMem, hBitmap);
//
//		// ��ͼ����
//		SetBkMode(hdcMem, TRANSPARENT);
//		TextOut(hdcMem, 10, 10, TEXT("����Ů ���Ӻ���"), _tcslen(TEXT("����Ů ���Ӻ���")));
//
//		// ���ڴ�λͼ���Ƶ����ڿͻ�����
//		GetObject(hBitmap, sizeof(bmp), &bmp);
//		BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
//
//		EndPaint(hwnd, &ps);
//		DeleteObject(hBitmap);
//		DeleteDC(hdcMem);
//		return 0;
//
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//	}
//
//	return DefWindowProc(hwnd, uMsg, wParam, lParam);
//}