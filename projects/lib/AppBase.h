#pragma once

struct AppBase
{
	AppBase():hWnd(0),card(0),bRedraw(false){}
	AppBase(HWND h, void* p): hWnd(h),card(p),bRedraw(false){}
	HWND hWnd;
	bool bRedraw;
	void* card;

	void GetClientRect( int* cx, int* cy )
	{
		RECT rc;
		::GetClientRect(hWnd,&rc);

		*cx = rc.right - rc.left;
		*cy = rc.bottom - rc.top;

	}

};

struct MenuItem
{
	int id;
	int typ;
	LPCWSTR str;
};
