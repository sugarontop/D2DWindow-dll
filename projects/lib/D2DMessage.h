#pragma once

#include "AppBase.h"

#define STAT_VISIBLE			0x1
#define STAT_MODAL				0x2
#define STAT_CAPTURED			0x4
#define STAT_DEAD				0x8
#define STAT_ENABLE				0x10

#define STAT_DEBUG				0x20
#define STAT_DEBUG1				0x40

#define STAT_DEFAULT		(STAT_VISIBLE|STAT_ENABLE)


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

	LPVOID sender_parent;

	int prm1;
	LPCWSTR prm2;
};

#define NONAME		(L"__NONAME")

#define ID_FLOATING_MENU	-10


#define TYP_NULL 0
#define TYP_TEXTBOX 1
#define TYP_BUTTON 2
#define TYP_CONTROLS 3
#define TYP_DROPDOWNLISTBOX 4
#define TYP_MAIN_WINDOW 5
#define TYP_WHITE_CONTROL 6
#define TYP_SIMPLE_LISTBOX 7
#define TYP_MESSAGEBOX 8
#define TYP_CLIENT_CONTROL 9
#define TYP_STATIC 10
#define TYP_MDI_CONTROLS 11
#define TYP_MDISPLIT_CONTROLS  12
#define TYP_ACCORDIONBAR  13
#define TYP_WHITE_WINDOW 14

#define theWhite D2RGB(255,255,255)
#define theBlack D2RGB(0,0,0)
#define theClear D2RGBA(255,255,255,0)

#define theGray0 D2RGB(70,50,70)
#define theGray1 D2RGB(150,150,150)
#define theGray2 D2RGB(170,170,170)
#define theGray3 D2RGB(220,220,220)


#define EVENT_PUSH 1

#define WM_D2D_CB_INSERTSTRING					(WM_APP+1)
#define WM_D2D_RESOURCES_UPDATE					(WM_APP+2)
#define WM_D2D_CREATE							(WM_APP+3)
#define WM_D2D_DESTROY							(WM_APP+4)




#define WM_D2D_MDI_TOPCHANGE 					(WM_APP+6)
#define WM_D2D_MDI_SIZE_LARGEST					(WM_APP+7)
#define WM_D2D_MDI_SIZE_PRV						(WM_APP+8)
#define WM_D2D_MDI_TILE_HORZ					(WM_APP+9) //•À‚×‚Ä•\Ž¦


#define WM_D2D_ONCLOSE							(WM_APP+10)
#define WM_D2D_SET_SIZE							(WM_APP+11)
#define WM_D2D_SET_COLOR						(WM_APP+12) // w=0:back,1:fore,l=ColorF
