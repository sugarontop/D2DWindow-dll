#include "pch.h"
#include "D2DChildWindow.h"

using namespace V6;


#define  APP (D2DApp::GetInstance())

#define TITLEBAR_HEIGHT 26.0f

void D2DChildWidow::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControls::CreateControl(parent, pacontrol,rc, stat, name, local_id);
	mode_ = 0;

}

void D2DChildWidow::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();

	cxt.DFillRect(rc_, ColorF::Pink);

	mat.Offset(rc_.LeftTop());
	
	DrawTitlebar(cxt);

	mat.Offset(0,TITLEBAR_HEIGHT);

	for(auto& it : controls_ )
		it->Draw(cxt);

	mat.PopTransform();
}
LRESULT D2DChildWidow::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT h = 0;
	bool bl = true;

	switch( message )
	{
		case WM_D2D_CREATE:
			h = 1;
		break;
		case WM_LBUTTONDOWN:
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);

			if ( rc_.PtInRect(pt) || mode_ == 1)
			{
				if ( (rc_.top < pt.y && pt.y < rc_.top+TITLEBAR_HEIGHT) || mode_ ==1 )
				{
					h = TitleBarProc(b,message,pm);
				}
			}
			else
				bl = false;
		}
		break;

	}

	if ( h==0 && bl )
		h = D2DControls::WndProc(b,message,wParam,lParam);



	return h;
}

LRESULT D2DChildWidow::TitleBarProc(AppBase& b, UINT message, MouseParam& pm)
{
	LRESULT h = 0;
	switch( message )
	{
		case WM_LBUTTONDOWN:
		{
			APP.SetCapture(this);
			mode_ = 1;
			h = 1;
		}
		break;
		case WM_MOUSEMOVE:
		{
			if ( APP.IsCapture(this))
			{
				
				auto pt = mat_.DPtoLP(pm.pt);
				auto pt2 = mat_.DPtoLP(pm.ptprv);

				rc_.Offset(pt.x-pt2.x, pt.y-pt2.y);

				b.bRedraw = true;

				h = 1;
			}

		}
		break;
		case WM_LBUTTONUP:
		{
			if ( APP.IsCapture(this))			
				APP.ReleaseCapture();
			mode_ = 0;
			h = 1;
		}
		break;
	}

	return h;
}



void D2DChildWidow::DrawTitlebar(D2DContext& cxt)
{
	cxt.DFillRect(FRectF(0,0,rc_.Width(),TITLEBAR_HEIGHT), ColorF::Black);



}

 

