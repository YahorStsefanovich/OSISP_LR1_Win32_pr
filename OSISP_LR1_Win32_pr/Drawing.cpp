#include "Drawing.h"
#include "math.h"
#include "string.h"


void Drawing::SetWindow(HWND hWnd) {
	hwnd = hWnd;
	dc = GetDC(hwnd); //Цепляемся к консольному окну
	graphics = new Graphics(dc);
	winCoord = {};
	GetClientRect(hwnd, &winCoord); //Узнаём координаты
	rectCoord.bottom = (int)winCoord.bottom / 2 + (int)height/2;
	rectCoord.left = (int)winCoord.right / 2 - (int)height / 2;
	rectCoord.right = (int)winCoord.right / 2 + (int)height / 2;
	rectCoord.top = (int)winCoord.bottom / 2 - (int)height / 2;
}

Drawing::Drawing() {
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	gdiplusStartupInput.GdiplusVersion = 1;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	brush = CreateHatchBrush(HS_BDIAGONAL, RGB(0, 255, 0)); // Создаём кисть определённого стиля и цвета
	window = {}; //Переменная window будет использована для получения ширины и высоты окна
	isClicked = false;
}

Drawing::~Drawing() {
	ReleaseDC(hwnd, dc); //Освобождаем общий или оконный (не влияющий на класс или локальность) контекст устройства, делая его доступным для других прикладных задач. 
	DeleteObject(brush); //Очищаем память от созданной, но уже ненужной кисти
}

void Drawing::DrawRect() {
	SelectObject(dc, brush); //Выбираем кисть 
	Rectangle(dc, rectCoord.left, rectCoord.bottom,
		rectCoord.right, rectCoord.top); //Нарисовали прямоугольник, закрашенный неким стилем		
}

void Drawing::MoveRight(int step) {
	SelectObject(dc, brush); //Выбираем кисть
	InvalidateRect(hwnd, NULL, true);
	UpdateWindow(hwnd);
	//if (winCoord.right >= rectCoord.right + step) {
	if (winCoord.right >= rectCoord.right + step) {
		rectCoord.left += step;
		rectCoord.right += step;
	}
	DrawRect();
}

void Drawing::MoveLeft(int step) {
	SelectObject(dc, brush); //Выбираем кисть
	InvalidateRect(hwnd, NULL, true);
	UpdateWindow(hwnd);
	if (winCoord.left <= rectCoord.left - step) {
		rectCoord.left -= step;
		rectCoord.right -= step;
	}
	DrawRect();
}

void Drawing::MoveUp(int step) {
	SelectObject(dc, brush); //Выбираем кисть
	InvalidateRect(hwnd, NULL, true);
	UpdateWindow(hwnd);
	if (winCoord.top <= rectCoord.top - step) {
		rectCoord.top -= step;
		rectCoord.bottom -= step;
	}
	DrawRect();
}

void Drawing::MoveDown(int step) {
	SelectObject(dc, brush); //Выбираем кисть
	InvalidateRect(hwnd, NULL, true);
	UpdateWindow(hwnd);
	if (winCoord.bottom >= rectCoord.bottom + step) {
		rectCoord.top += step;
		rectCoord.bottom += step;
	}
	DrawRect();
}

void Drawing::MoveByClick() {
	POINT pt;
	GetCursorPos(&pt);
	int step = 1;
	ScreenToClient(hwnd, &pt);
	if (isClicked)
	{
		while ((pt.x < rectCoord.left) || (pt.x > rectCoord.right) || 
			(pt.y < rectCoord.top) || (pt.y > rectCoord.bottom))
		{
			if ((pt.x < rectCoord.left) && (pt.x >= winCoord.left))
				MoveLeft(step);
			if ((pt.x > rectCoord.right) && (pt.x <= winCoord.right))
				MoveRight(step);
			if ((pt.y < rectCoord.top) && (pt.y >= winCoord.top))
				MoveUp(step);
			if ((pt.y > rectCoord.bottom) && (pt.y <= winCoord.bottom))
				MoveDown(step);
		}
	}
}

void Drawing::MoveByWheel(WPARAM wParam, int step) {
	if (GET_KEYSTATE_WPARAM(wParam) == MK_SHIFT) {
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
			MoveRight(step);
		}
		if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
			MoveLeft(step);
		}
	}
	else
	{
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
			MoveUp(step);
		}
		if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
			MoveDown(step);
		}
	}
}

void Drawing::Rotate(int degree)
{
	//SetGraphicsMode(dc, GM_ADVANCED);
	//SetMapMode(dc, MM_LOENGLISH);
	//
	//XFORM xForm;
	//RECT rect;
	//double const PI = 3.14159265;

	//int x0 = (int)((rectCoord.right + rectCoord.left) / 2);
	//int y0 = (int)((rectCoord.bottom + rectCoord.top) / 2);
	//double rad = degree * PI / 180;

	////SetWindowOrgEx(dc, x0, y0, NULL);

	//double x = cos(rad)*x0 - sin(rad)*y0;
	//double y = sin(rad)*x0 + cos(rad)*x0;

	//rectCoord.bottom = (int)y0 + (int)height / 2;
	//rectCoord.left = (int)x0 - (int)height / 2;
	//rectCoord.right = (int)x0 + (int)height / 2;
	//rectCoord.top = (int)y0 - (int)height / 2;

	//xForm.eM11 = cos(rad);
	//xForm.eM12 = sin(rad);
	//xForm.eM21 = -sin(rad);
	//xForm.eM22 = cos(rad);
	//xForm.eDx =  0.0;
	//xForm.eDy =  0.0;

	//GetClientRect(hwnd, (LPRECT)&rect);
	//DPtoLP(dc, (LPPOINT)&rect, 2);
	//SetWorldTransform(this->dc, &xForm);
	//DrawRect();
	graphics->RotateTransform(degree);
	Gdiplus::Rect rc(0, 0, height, height);
	SelectObject(dc, brush); //Выбираем кисть
	InvalidateRect(hwnd, NULL, false);
	UpdateWindow(hwnd);
	ShowBitmap(fileName);
}

void Drawing::ShowBitmap(string hBmp)
{
	fileName = hBmp;
	wstring wide_string = wstring(hBmp.begin(), hBmp.end());
	const wchar_t* result = wide_string.c_str();
	img = new Gdiplus::Image(result);
	graphics->TranslateTransform((rectCoord.left+rectCoord.right)/2, 
		(rectCoord.top + rectCoord.bottom) / 2);
	Gdiplus::Rect rc(0, 0, height, height);
	graphics->DrawImage(img, rc);
}