#pragma once

#include "AppBase.h"

#define STAT_VISIBLE			0x1
#define STAT_MODAL				0x2
#define STAT_CAPTURED			0x4
#define STAT_DEAD				0x8
#define STAT_ENABLE				0x10
#define STAT_FLOATING			0x20

#define STAT_DEBUG				0x20
#define STAT_DEBUG1				0x40
#define STAT_SIMPLE				0x80

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

typedef DWORD (*D2DEventHandlerDelegate)(void* sender,LPCWSTR eventName, void* param);


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
#define TYP_IMAGE 15
#define TYP_SCROLLBAR 16

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


#define WM_D2D_LISTBOX_ADD_ITEM					(WM_APP+13)
#define WM_D2D_TAB_CHANGE						(WM_APP+14)
#define WM_D2D_TABCONTROL_TAB_NAME				(WM_APP+15)

#define WM_D2D_SET_SIZE_SIZE					(WM_APP+16) // LPARAM is FSizeF
#define WM_D2D_GET_RESOURCE_BINARY				(WM_APP+17) // LPARAM is IStream**
#define WM_D2D_SET_RESOURCE						(WM_APP+18) // LPARAM is name
#define WM_D2D_IME_ONOFF						(WM_APP+19) 
#define WM_D2D_ONIME_ONOFF						(WM_APP+20) 
#define WM_D2D_SET_TEXT							(WM_APP+21) 
#define WM_D2D_CB_GETSELECT						(WM_APP+22)


#define WM_D2D_MINIMUM_SIZE						(WM_APP+23)


#define WM_D2D_BROADCAST_MESSAGE_FIRST			(WM_APP+5000) // capture‚Í–³Ž‹‚·‚é