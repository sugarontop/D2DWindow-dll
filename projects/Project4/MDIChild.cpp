
#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"

using namespace V6;

enum class mtype { MDI_FULLSIZE, MDI_MOVABLE };
const float bar_h = 30.0f;

struct WhiteBoard
{     
	DelegateDrawFunc f1;
	DelegateProcFunc f2;
	D2DMat mat;
};
struct CaptureObj1
{
	FRectF rc;
	FRectF prerc;
	D2DMat mat;
	WhiteBoard wboard;
	mtype typ;
	UIHandle me;
};

void MDIStatusChange(CaptureObj1* obj)
{
	if ( obj->typ == mtype::MDI_MOVABLE )
	{
		obj->typ = mtype::MDI_FULLSIZE;

		auto h = D2DGetParent(obj->me);
		auto rc = D2DGetRect(h);

		obj->prerc = obj->rc;
		obj->rc = rc;
		D2DSetRect(obj->me, rc);
	}
	else if ( obj->typ == mtype::MDI_FULLSIZE )
	{
		obj->typ = mtype::MDI_MOVABLE;

		obj->rc = obj->prerc;
		D2DSetRect(obj->me, obj->prerc);
	}


}

HRESULT TitleBarMoouseProc(LPVOID captureobj,AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT r = 0;

	CaptureObj1* obj = (CaptureObj1*)captureobj;

	switch(message)
	{
		case WM_LBUTTONDOWN:
		{
			MouseParam* mp = (MouseParam*)lParam;
			auto pt = obj->mat.DPtoLP(mp->pt);

			if ( obj->rc.PtInRect(pt))
			{
				auto rc = obj->rc;
				rc.SetHeight(bar_h);
				if ( rc.PtInRect(pt))
				{
					D2DSetCapture(obj->me);

					r = 1;
				}
			}


		}
		break;
		case WM_MOUSEMOVE:
		{
			auto h1 = D2DGetCapture();

			if (h1.p == obj->me.p )
			{
				MouseParam* mp = (MouseParam*)lParam;
				auto pt = obj->mat.DPtoLP(mp->pt);
				auto ptprv = obj->mat.DPtoLP(mp->ptprv);

				obj->rc.Offset( pt.x - ptprv.x, pt.y - ptprv.y );


				D2DSetRect(obj->me, obj->rc);

				b.bRedraw = true;
				r = 1;
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			auto h1 = D2DGetCapture();

			if (h1.p == obj->me.p )
			{
				D2DReleaseCapture();
				r = 1;
			}


		}
		break;
		case WM_LBUTTONDBLCLK:
		{
			MouseParam* mp = (MouseParam*)lParam;
			auto pt = obj->mat.DPtoLP(mp->pt);

			if ( obj->rc.PtInRect(pt))
			{
				auto rc = obj->rc;
				rc.SetHeight(bar_h);
				if ( rc.PtInRect(pt))
				{
					MDIStatusChange(obj);
					

					r = 1;
				}
			}
		}
		break;

	}



	return r;
}



UIHandle CreateMDIChild(UIHandleWin hwin, UIHandle hcs)
{
	CaptureObj1* obj = new CaptureObj1();
	obj->typ = mtype::MDI_MOVABLE;



	obj->wboard.f1 = [](LPVOID captureobj, D2DContext& cxt) {
		CaptureObj1* obj = (CaptureObj1*)captureobj;
		WhiteBoard& wb = obj->wboard;

		D2DMatrix mat(*cxt);
		obj->mat = mat.PushTransform();
		{
			mat.Offset(obj->rc);
			FRectF rc = obj->rc;
		

			if ( obj->typ == mtype::MDI_MOVABLE )
			{
				FRectF rcbar(0,0,rc.Size().width, bar_h);
				cxt.DFillRect(rcbar, D2RGB(255,255,255));

				rc.SetRect(0, bar_h, rcbar.Width(), rc.Height());
				cxt.DFillRect(rc, D2RGB(220,220,220));
			}
			else if ( obj->typ == mtype::MDI_FULLSIZE )
			{
				cxt.DFillRect(rc.ZeroRect(), D2RGB(220,220,220));
			}
		}
		mat.PopTransform();
	};
	obj->wboard.f2 = [](LPVOID captureobj,AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)->HRESULT {

		CaptureObj1* obj = (CaptureObj1*)captureobj;
		HRESULT r = 0;
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
			case WM_D2D_DESTROY:
			{

				CaptureObj1* pc = (CaptureObj1*) captureobj;
				delete pc;

			}

			case WM_LBUTTONDOWN:
			case WM_MOUSEMOVE:
			case WM_LBUTTONUP:
			case WM_LBUTTONDBLCLK:
			{
				if ( obj->typ == mtype::MDI_MOVABLE )
					r = TitleBarMoouseProc(captureobj,b,message,wParam,lParam);

			}
			break;
		

		}        
		return r;
	};

	FRectF rc(0, 0, FSizeF(800, 600));
	auto mc1 = D2DCreateWhiteControls(obj, obj->wboard.f1, obj->wboard.f2, hwin, hcs, rc, STAT_VISIBLE | STAT_ENABLE, L"MDIChild0", 110);

	
	return mc1;
	
	

}



