#pragma once


#include <Windows.h>
#include <string>
#include <cstdlib>
#include <gdiplus.h>

using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")


using namespace std;

class Drawing
{
public:
	Drawing();
	~Drawing();

	void DrawRect();
	void MoveRight(int step);
	void MoveLeft(int step);
	void MoveUp(int step);
	void MoveDown(int step);
	void MoveByClick();
	void SetWindow(HWND hWnd);
	void MoveByWheel(WPARAM wParam, int step);
	void ShowBitmap(string);
	void Rotate(int degrees);
	bool isClicked;
	HDC dc;
private:
	HWND hwnd;
	RECT window;
	HBRUSH brush;
	RECT rectCoord;
	RECT winCoord;
	//int step = 10;
	//POINT pt;
	int height = 100;
	int angle;
	Gdiplus::Image *img;
	Graphics *graphics;
	string fileName;
};


