
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
	enum TitleMouse { MOVE,SMALL,LARGE,DESTROY };
	TitleMouse tmd;
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
					if ( obj->tmd == CaptureObj1::TitleMouse::MOVE )
						D2DSetCapture(obj->me);
					else if ( obj->tmd == CaptureObj1::TitleMouse::DESTROY )
					{
						D2DDestroyControl(obj->me);
					}
					else if ( obj->tmd == CaptureObj1::TitleMouse::LARGE )
					{
						MDIStatusChange(obj);
					}
					else if ( obj->tmd == CaptureObj1::TitleMouse::SMALL )
					{
						//
					}


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

				if ( obj->tmd == CaptureObj1::TitleMouse::MOVE )
				{
					obj->rc.Offset( pt.x - ptprv.x, pt.y - ptprv.y );

					D2DSetRect(obj->me, obj->rc);
				}
				

				b.bRedraw = true;
				r = 1;
			}
			else
			{
				obj->tmd = CaptureObj1::TitleMouse::MOVE;
				MouseParam* mp = (MouseParam*)lParam;
				auto pt = obj->mat.DPtoLP(mp->pt);

				if ( obj->rc.PtInRect(pt))
				{
					FRectF rc(obj->rc.right-45*3, obj->rc.top, obj->rc.right, obj->rc.top+30);
					if ( rc.PtInRect(pt))
					{
						if ( pt.x < obj->rc.right-45*2 )
							obj->tmd = CaptureObj1::TitleMouse::SMALL;
						else if ( pt.x < obj->rc.right-45 )
							obj->tmd = CaptureObj1::TitleMouse::LARGE;
						else 
							obj->tmd = CaptureObj1::TitleMouse::DESTROY;
						b.bRedraw = true;
					}
				}
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

void DrawTitlebar(D2DContext& cxt, const FRectF& rc, CaptureObj1::TitleMouse tmd)
{
	FRectF rcbar(0,0,rc.Size().width, bar_h);
	cxt.DFillRect(rcbar, D2RGB(255,255,255));

	(*cxt)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

	D2DMatrix mat(*cxt);
	mat.PushTransform();
	mat.Offset(rc.Width()-45, 0); // width:45, three buttons


	FRectF rc2(0,0,45,27);
	if ( tmd == CaptureObj1::TitleMouse::DESTROY )
		cxt.DFillRect(rc2, D2RGB(232,17,35));

	(*cxt)->DrawLine(FPointF(11+5,11), FPointF(21+5,21), cxt.black_);
	(*cxt)->DrawLine(FPointF(21+5,11), FPointF(11+5,21), cxt.black_);



	mat.Offset(-45, 0);

	if ( tmd == CaptureObj1::TitleMouse::LARGE )
		cxt.DFillRect(rc2, D2RGB(229,229,229));
	
	(*cxt)->DrawRectangle(FRectF(11,11,21,21), cxt.black_);
	
	mat.Offset(-45, 0);

	if ( tmd == CaptureObj1::TitleMouse::SMALL )
		cxt.DFillRect(rc2, D2RGB(229,229,229));
	
	(*cxt)->DrawRectangle(FRectF(11,15,21,15), cxt.black_);

	mat.PopTransform();

	(*cxt)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
}


UIHandle CreateMDIChild(UIHandleWin hwin, UIHandle hcs)
{
	CaptureObj1* obj = new CaptureObj1();
	obj->typ = mtype::MDI_MOVABLE;
	obj->tmd = CaptureObj1::TitleMouse::MOVE;


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
				FRectF rc2(rc.ZeroRect());
				cxt.DFillRect(rc2, D2RGB(220,220,220));				
				cxt.DFillRect(rc2.Inflate(-2,-2), D2RGB(230,230,230));
				
				
				DrawTitlebar(cxt, rc, obj->tmd);
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
				{
					r = TitleBarMoouseProc(captureobj,b,message,wParam,lParam);

					if ( r != 0 && message == WM_LBUTTONDOWN )
					{
						D2DMDISetTopControl(obj->me);

					}
				}

			}
			break;
		

		}        
		return r;
	};

	FRectF rc(0, 0, FSizeF(800, 600));
	auto mc1 = D2DCreateWhiteControls(obj, obj->wboard.f1, obj->wboard.f2, hwin, hcs, rc, STAT_VISIBLE | STAT_ENABLE, L"MDIChild0", 110);

	
	return mc1;
	
	

}



