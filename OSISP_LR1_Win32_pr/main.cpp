#include "Drawing.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam);

Drawing dr;

void WinDialog() {
	OPENFILENAME ofn;       
	char szFile[260];       
	HWND hwnd;              
	HANDLE hf;              

					   
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;

	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Картинки (*.bmp)\0*.bmp\0Картинки (*.jpg)\0*.jpg\0Картинки (*.png)\0*.png\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = "..\\Images\\";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		/*HBITMAP hBitmap;
		hBitmap = (HBITMAP)LoadImage(NULL, ofn.lpstrFile, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
		HDC hLocalDC;
		hLocalDC = CreateCompatibleDC(dr.dc);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(hLocalDC, hBitmap);
		BitBlt(dr.dc, 0,0, NULL, NULL, hLocalDC, 400, 400, SRCCOPY);*/
		dr.ShowBitmap((string)ofn.lpstrFile);
	}
}

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex; HWND hWnd; MSG msg;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_HAND);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNSHADOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "HelloWorldClass";
	wcex.hIconSm = wcex.hIcon;
	RegisterClassEx(&wcex);

	hWnd = CreateWindow("HelloWorldClass", "Hello, World!",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	dr.SetWindow(hWnd);
	dr.DrawRect();


	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	int step = 10;
//	POINT pt;
	switch (message)
	{
	case WM_RBUTTONDOWN:
		WinDialog();
		break;
	case WM_LBUTTONDOWN:
		dr.isClicked = true;
		dr.MoveByClick();
		break;
	case WM_LBUTTONUP:
		dr.isClicked = false;
		break;
	case WM_PAINT:
		break;
	case WM_MOUSEMOVE:
		dr.MoveByClick();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOUSEWHEEL:
		dr.MoveByWheel(wParam, 10);
		break;
	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_ESCAPE:
			if (MessageBox(hWnd, TEXT("Выйти из программы?"), TEXT("Выход"), 
				MB_YESNO | MB_ICONQUESTION) == IDYES)
				PostQuitMessage(0);
			break;
		case 0x45:
			dr.Rotate(10);
		case VK_LEFT:
			dr.MoveLeft(step);
			break;
		case VK_RIGHT:
			dr.MoveRight(step);
			break;
		case VK_DOWN:
			dr.MoveDown(step);
			break;
		case VK_UP:
			dr.MoveUp(step);
			break;
		default:
			break;
		} 
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
