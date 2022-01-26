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

DLLEXPORT bool WINAPI D2DResource(D2DWindow* win,  FRectF& rc );
DLLEXPORT void WINAPI D2DInitail(INT_PTR p );

DLLEXPORT UIHandle WINAPI D2DCreateWhiteWindow( UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateSquarePaper(UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateTextbox(UIHandle ctrls, const FRectF& rc, bool multiline, DWORD stat, LPCWSTR name, int id=-1, int ext=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateButton(UIHandle ctrls , const FRectF& rc, DWORD stat, LPCWSTR name, int id=-1);
DLLEXPORT UIHandle WINAPI D2DCreateDropdownListbox(UIHandle ctrls , const FRectF& rc, DWORD stat, LPCWSTR name, int id=-1);
DLLEXPORT UIHandle WINAPI D2DCreateControls(UIHandle ctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id=-1);
DLLEXPORT UIHandle WINAPI D2DMessageBox(UIHandleWin win,  const D2D1_RECT_F& rc, LPCWSTR title, LPCWSTR message);
DLLEXPORT UIHandle WINAPI D2DFloatingMenu(UIHandleWin hwin, const D2D1_RECT_F& rc, LPVOID sender, MenuItem* items, int items_cnt);
DLLEXPORT UIHandle WINAPI D2DCreateStatic(UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR text, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateEmptyControls(UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateXXXControls(UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateControlsWithScrollbar(UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateListbox(UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateWhiteControls(LPVOID captureobj, DelegateDrawFunc func1, DelegateProcFunc func2, UIHandle ctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id);
DLLEXPORT UIHandle WINAPI D2DCreateClientControls(LPVOID captureobj, DelegateDrawFunc func1, DelegateProcFunc func2, UIHandle ctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id);

DLLEXPORT UIHandle WINAPI D2DCreateImage(UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id=-1 );

DLLEXPORT bool WINAPI D2DSetProcfunction(UIHandle, DelegateProcFunc func2);

DLLEXPORT UIHandle WINAPI D2DCast(void* target);

DLLEXPORT void WINAPI D2DSetFocus(UIHandle h);
DLLEXPORT void WINAPI D2DSetText(UIHandle h, LPCWSTR str);
DLLEXPORT void WINAPI D2DSetFont(UIHandle h, LPCWSTR fontnm, float height);
DLLEXPORT int WINAPI D2DSetStat(UIHandle h, int stat);
DLLEXPORT void WINAPI D2DSetColor(UIHandle h, ColorF back, ColorF front, ColorF border );
DLLEXPORT void WINAPI D2DReadOnly(UIHandle h, bool readonly);

DLLEXPORT int WINAPI D2DAddItem(UIHandle, int idx, LPCWSTR str);

DLLEXPORT void WINAPI D2DInsertText(UIHandle h, LPCWSTR str, int len, int start_pos);
DLLEXPORT BSTR WINAPI D2DGetText(UIHandle h, bool bAll );
DLLEXPORT int WINAPI D2DSendMessage(UIHandle h, UINT msg, WPARAM, LPARAM);

DLLEXPORT UIHandleWin WINAPI D2DCreateMainHWnd( HWND hWnd, float font_height, int typ=0 );

DLLEXPORT UIHandle WINAPI D2DGetRootControls(UIHandleWin hMainWnd );
DLLEXPORT UIHandle WINAPI D2DGetControlFromID(UIHandleWin hMainWnd, UINT id);
DLLEXPORT UIHandle WINAPI D2DGetControlFromName(UIHandleWin hMainWnd, LPCWSTR nm);

DLLEXPORT UIHandle WINAPI D2DGetParent(UIHandle h);

DLLEXPORT ID2D1DeviceContext* WINAPI D2DGetDeviceContext(UIHandleWin main);

DLLEXPORT void WINAPI D2DSetCapture(UIHandle h);
DLLEXPORT UIHandle WINAPI D2DGetCapture();
DLLEXPORT bool WINAPI D2DIsCapture(UIHandle h);
DLLEXPORT void WINAPI D2DReleaseCapture();

DLLEXPORT void WINAPI D2DSwapChain(UIHandleWin main, HWND hWnd  );
DLLEXPORT void WINAPI D2DDraw(UIHandleWin main, void* option  );


DLLEXPORT  void WINAPI D2DDestroyControl(UIHandle hcs  );
DLLEXPORT  void WINAPI D2DDestroyWindow(UIHandleWin main  );


DLLEXPORT LRESULT WINAPI D2DDefWndProc(UIHandleWin main ,AppBase& app, UINT message, WPARAM wParam, LPARAM lParam);
DLLEXPORT void WINAPI D2DForceWndProc(UIHandleWin main, AppBase& app, UINT message, WPARAM wParam, LPARAM lParam);


DLLEXPORT D2D1_RECT_F WINAPI D2DGetRect(UIHandle h );
DLLEXPORT void WINAPI D2DSetRect( UIHandle h, D2D1_RECT_F rc );
DLLEXPORT void WINAPI D2DMDISetTopControl(UIHandle h);


//DLLEXPORT void D2DEventHandlerOnClick( UIHandle h, D2DEventHandler handler);
DLLEXPORT void WINAPI D2DEventHandler( UIHandle h, D2DEventHandlerDelegate handler);

////////
//DLLEXPORT UIHandle D2DCreateDropdownListbox(D2DWindow* win, D2DControls* ctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id);
DLLEXPORT XDropdownListBox* WINAPI D2DConvert(UIHandle r);


DLLEXPORT D2D1_RECT_F* WINAPI RectAnimation(const D2D1_RECT_F&  rcS, const D2D1_RECT_F& rcE, D2D1_RECT_F* p, int p_size, int style);
DLLEXPORT void WINAPI SmoothRect(int typ, UIHandleWin win, FRectF* target, FRectF dstRect);
DLLEXPORT bool WINAPI D2DStream2Bitmap( IStream* bmpstream, ID2D1RenderTarget* target, ID2D1Bitmap** bmp);




};