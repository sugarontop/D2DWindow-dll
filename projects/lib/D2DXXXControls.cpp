#include "pch.h"
#include "D2DXXXControls.h"
#include "D2D1UI_1.h"


#define  APP (D2DApp::GetInstance())

using namespace V6;

void D2DXXXControls::Draw(D2DContext& cxt)
{
	if (!BITFLG(STAT_VISIBLE)) return;
	
	D2DMatrix mat(*cxt);
	mat_ = mat.PushTransform();

	if (mode_ == 0)
		InnerDraw(cxt);
	else if ( mode_ == 1 )
	{
		mat.Offset(rc_);

		cxt.DFillRect(rc_.ZeroRect(), ColorF::LightCoral);


	}


	mat.PopTransform();

}

void D2DXXXControls::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);
	rc_ = rc;
	mode_ = 0;

}


LRESULT D2DXXXControls::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;

	switch( message )
	{
		case WM_D2D_CREATE:
		{
			r = 1;

		}
		break;
		case WM_LBUTTONDBLCLK:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);

			if ( rc_.PtInRect(pt))
			{
				mode_ = 0;
				rc_.SetRect(0,0,FSizeF(500,700));

				UIHandleWin hwin={};
				hwin.p = GetParent();

				r = 1;
			}
		}
		break;

		case WM_D2D_MINIMUM_SIZE:
		{			
			if (rc_.Empty())
				rc_.SetRect(0,0,FSizeF(500,700));


			UIHandle h={};
			h.p = this;
		
			D2DSmoothRect(1,0,h, &rc_, FRectF(500,500,FSizeF(250,30)));
			
			//rc_.SetRect(500,500,FSizeF(250,30));
			mode_ = 1;
			r = 1;
		}
		break;
		case WM_RBUTTONDOWN:
		{
			if ( mode_ == 1 )
			{
				
				
				UIHandle h={};
				h.p = this;

				D2DSmoothRect(1,0,h, &rc_, FRectF(500,100,FSizeF(50,30)));

				r = 1;
			}
			
		}
		break;
		case WM_SIZE:
		{
			int a = 0;
			wParam=1;
		}
		break;

	}

	if (r == 0 && BITFLG(STAT_ENABLE) && mode_ == 0)
		r = D2DControls::WndProc(b,message,wParam,lParam);

	return r;

}

