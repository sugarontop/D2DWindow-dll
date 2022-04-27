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

DLLEXPORT bool WINAPI D2DResource(D2DWindow* win,  D2D1_RECT_F& rc );
DLLEXPORT HANDLE WINAPI D2DInstanceInitail(INT_PTR p );

DLLEXPORT UIHandle WINAPI D2DCreateWhiteWindow( UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateSquarePaper(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateTextbox(UIHandle ctrls, const D2D1_RECT_F& rc, bool multiline, DWORD stat, LPCWSTR name, int id=-1, int ext=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateButton(UIHandle ctrls , const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1);
DLLEXPORT UIHandle WINAPI D2DCreateDropdownListbox(UIHandle ctrls , const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1);
DLLEXPORT UIHandle WINAPI D2DCreateControls(UIHandle ctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1);
DLLEXPORT UIHandle WINAPI D2DMessageBox(UIHandleWin win,  const D2D1_RECT_F& rc, LPCWSTR title, LPCWSTR message);
DLLEXPORT UIHandle WINAPI D2DFloatingMenu(UIHandleWin hwin, const D2D1_RECT_F& rc, LPVOID sender, MenuItem* items, int items_cnt);
DLLEXPORT UIHandle WINAPI D2DCreateStatic(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR text, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateEmptyControls(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateXXXControls(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateSimpleControls(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );

DLLEXPORT UIHandle WINAPI D2DCreateListbox(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateWhiteControls(LPVOID captureobj, DelegateDrawFunc func1, DelegateProcFunc func2, UIHandle ctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id);
DLLEXPORT UIHandle WINAPI D2DCreateClientControls(LPVOID captureobj, DelegateDrawFunc func1, DelegateProcFunc func2, UIHandle ctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id);
DLLEXPORT UIHandle WINAPI D2DCreateScrollbar(UIHandle ctrls, bool bVertical, DWORD stat, LPCWSTR name, int id);
DLLEXPORT UIHandle WINAPI D2DCreateControlsWithScrollbar(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateControlsWithMove(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateMDISplitFrame(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateChildWindow(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateFileManage(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );

DLLEXPORT UIHandle WINAPI D2DCreateGridView(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateLogin(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );

DLLEXPORT UIHandle WINAPI D2DCreateTabControls(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DAddNewTab(UIHandle hctrls, LPCWSTR nm);
DLLEXPORT UIHandle WINAPI D2DGetTab(UIHandle hctrls, USHORT idx);
DLLEXPORT UIHandle WINAPI D2DGetControlFromIdx(UIHandle hctrls, USHORT idx);
DLLEXPORT bool WINAPI D2DIsControls(UIHandle h);

DLLEXPORT void WINAPI D2DInnerDraw(UIHandle h, LPVOID cxt);


DLLEXPORT UIHandle WINAPI D2DCreateImage(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );

DLLEXPORT bool WINAPI D2DSetProcfunction(UIHandle, DelegateProcFunc func2);

DLLEXPORT UIHandle WINAPI D2DCast(void* target);

DLLEXPORT void WINAPI D2DSetFocus(UIHandle h);
DLLEXPORT void WINAPI D2DSetText(UIHandle h, LPCWSTR str);
DLLEXPORT void WINAPI D2DSetFont(UIHandle h, LPCWSTR fontnm, float height);
DLLEXPORT DWORD WINAPI D2DSetStat(UIHandle h, DWORD stat);
DLLEXPORT void WINAPI D2DSetColor(UIHandle h, ColorF back, ColorF front, ColorF border );
DLLEXPORT void WINAPI D2DReadOnly(UIHandle h, bool readonly);
DLLEXPORT void WINAPI D2DEnable(UIHandle h, bool enable);

DLLEXPORT int WINAPI D2DAddItem(UIHandle, LPCWSTR str);

DLLEXPORT DWORD WINAPI D2DGetStat(UIHandle h);

DLLEXPORT void WINAPI D2DInsertText(UIHandle h, LPCWSTR str, int len, int start_pos);
DLLEXPORT BSTR WINAPI D2DGetText(UIHandle h, bool bAll );
DLLEXPORT int WINAPI D2DSendMessage(UIHandle h, UINT msg, WPARAM, LPARAM);

DLLEXPORT UIHandleWin WINAPI D2DCreateMainHWnd( HWND hWnd, float font_height, int typ=0 );

DLLEXPORT UIHandle WINAPI D2DGetRootControls(UIHandleWin hMainWnd );
DLLEXPORT UIHandle WINAPI D2DGetControlFromID(UIHandleWin hMainWnd, UINT id);
DLLEXPORT UIHandle WINAPI D2DGetControlFromName(UIHandleWin hMainWnd, LPCWSTR nm);

DLLEXPORT UIHandleWin WINAPI D2DGetWindow(UIHandle h);
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
DLLEXPORT LRESULT WINAPI D2DDefControlProc(UIHandle ctrls ,AppBase& app, UINT message, WPARAM wParam, LPARAM lParam);


DLLEXPORT D2D1_RECT_F WINAPI D2DGetRect(UIHandle h );
DLLEXPORT D2D1_RECT_F* WINAPI D2DGetRect2(UIHandle h);

DLLEXPORT int  WINAPI D2DGetId(UIHandle h );

DLLEXPORT BSTR WINAPI D2DGetName(UIHandle h);
DLLEXPORT BSTR WINAPI D2DGetLocalName(UIHandle h);

DLLEXPORT void WINAPI D2DSetRect( UIHandle h, D2D1_RECT_F rc );
DLLEXPORT UIHandle WINAPI D2DSetTopControl(UIHandle h);
DLLEXPORT UIHandle WINAPI D2DSetLastControl(UIHandle h);
DLLEXPORT void WINAPI D2DClear(UIHandle h);

DLLEXPORT void WINAPI D2DScrollbarSetMaxSize(UIHandle h, float height);


//DLLEXPORT void D2DEventHandlerOnClick( UIHandle h, D2DEventHandler handler);
DLLEXPORT void WINAPI D2DEventHandler( UIHandle h, D2DEventHandlerDelegate handler);

////////
//DLLEXPORT UIHandle D2DCreateDropdownListbox(D2DWindow* win, D2DControls* ctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id);
DLLEXPORT XDropdownListBox* WINAPI D2DConvert(UIHandle r);


DLLEXPORT D2D1_RECT_F* WINAPI RectAnimation(const D2D1_RECT_F&  rcS, const D2D1_RECT_F& rcE, D2D1_RECT_F* p, int p_size, int style);
DLLEXPORT void WINAPI D2DSmoothRect(int typ, int id, UIHandle h, D2D1_RECT_F* target, D2D1_RECT_F dstRect);
DLLEXPORT bool WINAPI D2DStream2Bitmap( IStream* bmpstream, ID2D1RenderTarget* target, ID2D1Bitmap** bmp);

DLLEXPORT void WINAPI D2DMDICreateChildView(UIHandle hctrls,int typ);
DLLEXPORT UIHandle WINAPI D2DMDIGetControl(UIHandle hctrls, LPCWSTR nm);

DLLEXPORT bool WINAPI D2DSetOnClick(UIHandle h, DelegateClick func);

DLLEXPORT int WINAPI D2DAddBitmapItem(UIHandle h, ID2D1Bitmap* bmp);


DLLEXPORT ID2D1RenderTarget* WINAPI D2DGetRenderTarget(UIHandle h);
};