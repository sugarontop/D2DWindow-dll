#include "pch.h"

#include "D2DMessage.h"
#include "D2DContext.h"

#include "AppBase.h"
#include "D2D1UI_1.h"

using namespace V6;


#include "CToolboxControls.h"

#define OBI_WIDTH 200

void CToolboxControls::CreateControl(UIHandleWin hwin, UIHandle hcs,TYP typ, LPCWSTR nm )
{
	auto& obj = obj_;

	
	obj.active_idx = 0;
	obj.wboard.typ = 0;
	obj.hwin = hwin;

	obj.wboard.drawFunc = [](LPVOID captureobj, D2DContext& cxt) {

		CaptureObj& obj = *(CaptureObj*)captureobj;

		if ( obj.show_stat == 2 || obj.show_stat == 3 )
		{
			auto time = ::GetTickCount();
			
			if ( time - obj.time_s > 10 )
			{
				obj.acordion_idx++;
				obj.time_s = time;
			}

			auto rc = obj.acordion[obj.acordion_idx];
			D2DSetRect(obj.me, rc ); // mat‚Ì		
			cxt.DDrawRect(rc, D2RGB(220,220,220),D2RGB(220,220,220) );

			cxt.Redraw();
		}
		else if ( obj.show_stat ==1 )
			cxt.DDrawRect(obj.rc, D2RGB(220,220,220),D2RGB(220,220,220) );
			


		if ( obj.acordion_idx >= TESTTEST )
		{
			obj.show_stat -= 2;
			obj.acordion_idx = 0;
		}
	};

	obj.wboard.procFunc = [](LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)->HRESULT {

		CaptureObj& obj = *(CaptureObj*)captureobj;
		HRESULT r = 0;
		switch (message)
		{
			case WM_D2D_CREATE:
			{
				UIHandle& me = *(UIHandle*)lParam;			

				obj.me = me;
				r = 1;
			}
			break;
			case WM_SIZE:
			{
				auto parent = D2DGetParent(obj.me);				
				FRectF rc = D2DGetRect(parent);


				obj.rc.left = obj.rc.right = rc.right;
				obj.rc.left = obj.rc.right - OBI_WIDTH;
				obj.rc.SetHeight(rc.Height());

				D2DSetRect(obj.me, obj.rc);
			}
			break;
		}

		return r;
	};

	FRectF rc(0, 0, FSizeF(100, 600));

	obj.rc = rc;
	auto whb2 = D2DCreateWhiteControls(&obj, obj.wboard.drawFunc, obj.wboard.procFunc, hwin, hcs, rc, STAT_VISIBLE | STAT_ENABLE, nm, 0);


}

bool CToolboxControls::SwShow()
{
	if ( obj_.show_stat == 0 )
	{
		obj_.show_stat = 3; // •\Ž¦‚·‚é

		auto parent = D2DGetParent(obj_.me);				
		FRectF rc2 = D2DGetRect(parent);
		FRectF rc1 = rc2;

		rc2.left = rc2.right-OBI_WIDTH;
		rc1.left = rc1.right;
		obj_.acordion_idx = 0;
		obj_.time_s = ::GetTickCount();

		RectAnimation( rc1, rc2, obj_.acordion, TESTTEST, 0 );
	}
	else if ( obj_.show_stat == 1 )
	{
		obj_.show_stat = 2; //‰B‚·

		auto parent = D2DGetParent(obj_.me);				
		FRectF rc2 = D2DGetRect(parent);
		FRectF rc1 = rc2;


		rc2.left = rc2.right-OBI_WIDTH;
		rc1.left = rc1.right;

		obj_.acordion_idx = 0;
		obj_.time_s = ::GetTickCount();

		RectAnimation( rc2, rc1, obj_.acordion, TESTTEST, 0 );

	}
	else if ( obj_.show_stat == 2 || obj_.show_stat == 3 )
	{


	}

	return (obj_.show_stat == 1 );
}