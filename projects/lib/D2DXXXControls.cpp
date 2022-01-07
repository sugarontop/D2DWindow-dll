#include "pch.h"
#include "D2DXXXControls.h"

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

				r = 1;
			}
		}
		break;

		case WM_D2D_MINIMUM_SIZE:
		{
			rc_.SetRect(500,500,FSizeF(250,30));
			mode_ = 1;
			r = 1;
		}
		break;


	}

	if (r == 0 && BITFLG(STAT_ENABLE) && mode_ == 0)
		r = D2DControls::WndProc(b,message,wParam,lParam);

	return r;

}

