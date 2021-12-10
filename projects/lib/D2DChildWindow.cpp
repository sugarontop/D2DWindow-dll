#include "pch.h"
#include "D2DChildWindow.h"

using namespace V6;


#define  APP (D2DApp::GetInstance())

#define TITLEBAR_HEIGHT 26.0f

void D2DChildWidow::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControls::CreateControl(parent, pacontrol,rc, stat, name, local_id);
	mode_ = 0;
	title_bar_mode_ = 0;

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


		case WM_SIZE:
		case WM_D2D_SET_SIZE:
		{
			FSizeF sz( rc_.Width(), rc_.Height()-TITLEBAR_HEIGHT);

			D2DControls::WndProc(b,WM_D2D_SET_SIZE_SIZE,0,(LPARAM)&sz);

			bl = false;

		}
		break;

		case WM_LBUTTONDOWN:
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);

			auto old = title_bar_mode_;
			

			if ( rc_.PtInRect(pt) || mode_ == 1)
			{
				if ( (rc_.top < pt.y && pt.y < rc_.top+TITLEBAR_HEIGHT) || mode_ ==1 )
				{
					h = TitleBarProc(b,message,pm);
				}
			}
			else
				bl = false;


			if ( old != title_bar_mode_ )							
				b.bRedraw = true;
			
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
			if ( title_bar_mode_ == 0)
			{
				APP.SetCapture(this);
				mode_ = 1;
			}
			else if ( title_bar_mode_ > 0)
			{
				APP.SetCapture(this);
				mode_ = 0;
			}

			h = 1;
		}
		break;
		case WM_MOUSEMOVE:
		{
			auto pt = mat_.DPtoLP(pm.pt);
			auto old = title_bar_mode_;

			if ( APP.IsCapture(this))
			{								
				auto pt2 = mat_.DPtoLP(pm.ptprv);

				rc_.Offset(pt.x-pt2.x, pt.y-pt2.y);

				b.bRedraw = true;

				h = 1;
			}
			else
			{
				FRectF rc2(rc_.right-45,rc_.top,rc_.right,rc_.top+27);
				
				title_bar_mode_ = 0;
				if ( rc2.PtInRect(pt))
				{
					title_bar_mode_ = 1; h=1;
				}
				else
				{
					rc2.Offset(-45,0);
					if ( rc2.PtInRect(pt)){
						title_bar_mode_ = 2; h=1;
					}
					else
					{
						rc2.Offset(-45,0);
						if ( rc2.PtInRect(pt)) {
							title_bar_mode_ = 3; h=1;
						}
						else
							title_bar_mode_ = 0;
					}
				}
			}

			
		}
		break;
		case WM_LBUTTONUP:
		{
			if ( APP.IsCapture(this))
			{
				APP.ReleaseCapture();
				mode_ = 0;


				if ( title_bar_mode_ == 1 )
				{
					DestroyControl();

				}
			}


			h = 1;
		}
		break;
	}

	return h;
}





 

 static void DrawTitlebar2(D2DContext& cxt, const FRectF& rc,ColorF titlebar, LPCWSTR title, int tmd)
{
	FRectF rcbar = rc.ZeroRect(); 
	cxt.DFillRect(rcbar, titlebar);

	(*cxt)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

	D2DMatrix mat(*cxt);
	mat.PushTransform();
	mat.Offset(rc.Width()-45, 0); // width:45, three buttons


	FRectF rc2(0,0,45,27);
	if ( tmd == 1 )
		cxt.DFillRect(rc2, D2RGB(232,17,35)); // DESTROY

	(*cxt)->DrawLine(FPointF(11+5,11), FPointF(21+5,21), cxt.black_);
	(*cxt)->DrawLine(FPointF(21+5,11), FPointF(11+5,21), cxt.black_);



	mat.Offset(-45, 0);

	if ( tmd == 2 )
		cxt.DFillRect(rc2, D2RGB(229,229,229)); // LARGE

	(*cxt)->DrawRectangle(FRectF(11,11,21,21), cxt.black_);

	mat.Offset(-45, 0);

	if ( tmd == 3 )
		cxt.DFillRect(rc2, D2RGB(229,229,229)); // CLOSE

	(*cxt)->DrawRectangle(FRectF(11,15,21,15), cxt.black_);

	mat.PopTransform();

	(*cxt)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);


	(*cxt)->DrawText(title, wcslen(title), cxt.textformat_, rcbar, cxt.black_);

}

void D2DChildWidow::DrawTitlebar(D2DContext& cxt)
{
	DrawTitlebar2(cxt, FRectF(0,0,rc_.Width(),TITLEBAR_HEIGHT),ColorF::LightGreen, L"nothing", title_bar_mode_ );
	

}