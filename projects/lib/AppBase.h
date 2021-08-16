#pragma once

struct AppBase
{
	AppBase():hWnd(0),card(0),bRedraw(false){}
	AppBase(HWND h, void* p): hWnd(h),card(p),bRedraw(false){}
	HWND hWnd;
	bool bRedraw;
	void* card;
};