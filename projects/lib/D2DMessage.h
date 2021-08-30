#pragma once

#include "AppBase.h"

#define STAT_VISIBLE			0x1
#define STAT_MODAL			0x2
#define STAT_CAPTURED	0x4
#define STAT_DEAD				0x8
#define STAT_ENABLE			0x16


struct UIHandle
{
	WORD typ;
	LPVOID p;
};

struct UIHandleWin
{
	WORD typ;
	LPVOID p;
};

struct D2DNMHDR
{
	UIHandle sender;
	UINT code;

	int prm1;
	LPCWSTR prm2;
};


#define TYP_MAIN_WINDOW 5
#define TYP_TEXTBOX 1
#define TYP_BUTTON 2
#define TYP_CONTROLS 3
#define TYP_DROPDOWNLISTBOX 4


#define EVENT_PUSH 1

#define WM_D2D_CB_INSERTSTRING			(WM_APP+1)
#define WM_D2D_RESOURCES_UPDATE	(WM_APP+2)
#define WM_D2D_CREATE							(WM_APP+3)