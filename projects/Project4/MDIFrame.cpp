

#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"

using namespace V6;

extern UIHandleWin hwin;


UIHandle CreateMDIChild(UIHandleWin hwin, UIHandle hcs);
void CreateControl2(UIHandleWin hwin, UIHandle hcs );

void CreateControl(HWND hWnd)
{
	hwin = D2DCreateMainHWnd(hWnd, 14);

	auto root = D2DGetRootControls(hwin);


	struct WhiteBoard
	{     
		DelegateDrawFunc f1;
		DelegateProcFunc f2;
		int typ;
		D2DMat mat;
	};
	struct CaptureObj1
	{
		FRectF rc;
		D2DMat mat;
		WhiteBoard wboard;
		UIHandle me;
	};

	static CaptureObj1 obj;
	obj.wboard.typ = 0;


	obj.wboard.f1 = [](LPVOID captureobj, D2DContext& cxt) {

		CaptureObj1* obj = (CaptureObj1*)captureobj;
		WhiteBoard& wb = obj->wboard;

		D2DMatrix mat(*cxt);
		mat.PushTransform();


		FRectF rc = obj->rc;

		cxt.DFillRect(rc.ZeroRect(), D2RGB(170,170,170));

		mat.PopTransform();
	};
	obj.wboard.f2 = [](LPVOID captureobj,AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)->LRESULT {

		CaptureObj1* obj = (CaptureObj1*)captureobj;
		LRESULT r = 0;
		switch( message )
		{
			case WM_D2D_CREATE :
			{
				UIHandle& h = *(UIHandle*)lParam;

				obj->me = h;
				obj->rc = D2DGetRect(h);
				r = 1;
			}
			break;
			case WM_SIZE:
			{
				auto cx = LOWORD(lParam);
				auto cy = HIWORD(lParam);
				obj->rc.SetSize(cx, cy);
				D2DSetRect(obj->me, obj->rc);
			}
			break;

		}        
		return r;
	};

	FRectF rc(0, 0, FSizeF(30, 30));
	auto hmdi = D2DCreateWhiteControls(&obj, obj.wboard.f1, obj.wboard.f2, hwin, root, rc, STAT_VISIBLE | STAT_ENABLE, L"MDIFrame", 110);


	auto hmdichild = CreateMDIChild(hwin, hmdi);
	CreateControl2(hwin, hmdichild );
	CreateMDIChild(hwin, hmdi);

}