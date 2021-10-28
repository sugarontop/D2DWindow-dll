
#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"

using namespace V6;


struct WhiteBoard
{
	DelegateDrawFunc drawFunc;
	DelegateProcFunc procFunc;
	int typ;
	D2DMat mat;
};
struct CaptureObj1
{
	FRectF rc;
	UIHandle page[3];
	int active_idx;
	D2DMat mat;
	WhiteBoard wboard;
	UIHandleWin hwin;
	ID2D1Bitmap* bmp1;

	WhiteBoard wbbox;
	FRectF rcwbbox;
	UIHandle wbboxhandle;
	//CToolboxControls* toolbox;
};


void CreateControlTest(UIHandleWin hwin, UIHandle hcs )
{
	CaptureObj1* pobj= new CaptureObj1();


	CaptureObj1& obj = *pobj;

	obj.active_idx = 0;
	obj.wboard.typ = 0;
	obj.hwin = hwin;
	obj.bmp1 = nullptr;

	obj.wboard.drawFunc = [](LPVOID captureobj, D2DContext& cxt) {


		FRectF rc(0,0,200,200);

		(*cxt)->DrawText(L"test", 4, cxt.textformat_, rc, cxt.black_ );



		FRectF rc2( 50,250, FSizeF(500,500));

		cxt.DFillRect(rc2, D2RGB(150,150,150));

		FRectF rc3(0,0,200,26);

		D2DMatrix mat(*cxt);
		mat.PushTransform();

		mat.Offset(rc2);

		mat.Offset(1,1);

		
		mat.PushTransform();
		cxt.DFillRect(rc3, D2RGB(255,255,255));

		mat.Offset(1+rc3.Width(), 0);

		cxt.DFillRect(rc3, D2RGB(255,255,255));
		mat.PopTransform();

		mat.Offset(0,rc3.Height()+1);

		mat.PushTransform();
		cxt.DFillRect(rc3, D2RGB(255,255,255));

		mat.Offset(1+rc3.Width(), 0);

		cxt.DFillRect(rc3, D2RGB(255,255,255));
		mat.PopTransform();


		mat.PopTransform();

	};

	obj.wboard.procFunc = [](LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)->HRESULT {

		CaptureObj1& obj = *(CaptureObj1*)captureobj;
		HRESULT r = 0;
		switch (message)
		{
			case WM_D2D_CREATE:
				r = 1;

				auto& hcs = *(UIHandle*)lParam;


				auto tx = D2DCreateTextbox(obj.hwin, hcs, FRectF(51,200+26*2-1, FSizeF(200,26)), FALSE, STAT_DEFAULT, L"tx3",-1,1 );
				auto tx1 = D2DCreateTextbox(obj.hwin, hcs, FRectF(51,200+26*2-1+27, FSizeF(200,26)), FALSE, STAT_DEFAULT, L"tx4",-1,1 );


				FRectF rc(51,200,FSizeF(200,30));

				D2DCreateStatic(obj.hwin, hcs, rc, STAT_DEFAULT, L"‚±‚ê‚ÍƒeƒXƒg", NONAME );
			break;
		}

		return r;
	};


	FRectF rc(0, 0, FSizeF(1, 1));

	obj.rc = rc;
	auto hcs2 = D2DCreateWhiteControls(&obj, obj.wboard.drawFunc, obj.wboard.procFunc, hwin, hcs, rc, STAT_VISIBLE | STAT_ENABLE, L"whb31", 310);
}