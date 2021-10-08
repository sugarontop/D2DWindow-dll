#pragma once


#ifdef _WINDLL
#define DLLEXPORT extern "C" __declspec(dllexport)
#else
#define DLLEXPORT extern "C"
#endif

#include "D2DMessage.h"
#include "D2DInterface.h"
namespace V6 {

class D2DWindow;
class D2DControls;
class FRectF;

DLLEXPORT bool D2DResource(D2DWindow* win,  FRectF& rc );


DLLEXPORT UIHandle D2DCreateTextbox(UIHandleWin win, UIHandle ctrls, const FRectF& rc, bool multiline, DWORD stat, LPCWSTR name );
DLLEXPORT UIHandle D2DCreateButton(UIHandleWin win, UIHandle ctrls , const FRectF& rc, DWORD stat, LPCWSTR name, int id);
DLLEXPORT UIHandle D2DCreateDropdownListbox(UIHandleWin win, UIHandle ctrls , const FRectF& rc, DWORD stat, LPCWSTR name, int id);
DLLEXPORT UIHandle D2DCreateControls(UIHandleWin win, UIHandle ctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id);

DLLEXPORT UIHandle D2DCreateWhiteControls(LPVOID captureobj, DelegateDrawFunc func1, DelegateProcFunc func2, UIHandleWin win, UIHandle ctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id);

DLLEXPORT void D2DSetText(UIHandle h, LPCWSTR str);
DLLEXPORT int D2DSetStat(UIHandle h, int stat);
DLLEXPORT void D2DSetColor(UIHandle h, ColorF back, ColorF front, ColorF border );

DLLEXPORT int D2DAddItem(UIHandle, int idx, LPCWSTR str);

DLLEXPORT void D2DInsertText(UIHandle h, LPCWSTR str, int len, int start_pos);
DLLEXPORT BSTR D2DGetText(UIHandle h, bool bAll );


DLLEXPORT UIHandleWin D2DCreateMainHWnd( HWND hWnd, float font_height );
DLLEXPORT UIHandle D2DGetRootControls(UIHandleWin hMainWnd );
DLLEXPORT UIHandle D2DGetControlFromID(UIHandleWin hMainWnd, UINT id);
DLLEXPORT UIHandle D2DGetControlFromName(UIHandleWin hMainWnd, LPCWSTR nm);


DLLEXPORT ID2D1DeviceContext* D2DGetDeviceContext(UIHandleWin main  );

DLLEXPORT void D2DSetCapture(UIHandle h);
DLLEXPORT UIHandle D2DGetCapture();
DLLEXPORT void D2DReleaseCapture();

DLLEXPORT void D2DSwapChain(UIHandleWin main, HWND hWnd  );
DLLEXPORT void D2DDraw(UIHandleWin main, void* option  );


DLLEXPORT  void D2DDestroyWindow(UIHandleWin main  );


DLLEXPORT HRESULT D2DDefWndProc(UIHandleWin main ,AppBase& app, UINT message, WPARAM wParam, LPARAM lParam);
DLLEXPORT void D2DForceWndProc(UIHandleWin main, AppBase& app, UINT message, WPARAM wParam, LPARAM lParam);


DLLEXPORT D2D1_RECT_F D2DGetRect(UIHandle h );
DLLEXPORT void D2DSetRect( UIHandle h, D2D1_RECT_F rc );

////////
//DLLEXPORT UIHandle D2DCreateDropdownListbox(D2DWindow* win, D2DControls* ctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id);
DLLEXPORT XDropdownListBox* D2DConvert(UIHandle r);


DLLEXPORT D2D1_RECT_F* RectAnimation(const D2D1_RECT_F&  rcS, const D2D1_RECT_F& rcE, D2D1_RECT_F* p, int p_size, int style=0);
};