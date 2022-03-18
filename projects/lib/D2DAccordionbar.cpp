#include "pch.h"
#include "D2DWindow.h" 
#include "D2DAccordionbar.h"
#include "D2D1UI_1.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())

static int gj_ = 0;

#define BAR_WIDTH 800.0f
#define BTN_WIDTH 20.0f
#define STEP_CNT 50

void D2DAccordionbar::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);
	
	mat_ = mat.PushTransform();
	
	if ( mode_ == 1 || mode_ == 2)
	{
		FRectF rc(rc_);

		if ( gj_ < STEP_CNT )
		{
			mat.PushTransform();
			{
				FRectF arc = rcmove_[gj_++];

				FRectF brc(rc_);

				brc.right = brc.left + arc.right;

				D2DRectFilter fil(cxt, brc);
				float t = arc.right;
				mat.Offset(-BAR_WIDTH+t-BTN_WIDTH,0);
				D2DControls::Draw(cxt);

			}
			mat.PopTransform();

			rc.SetWidth(BTN_WIDTH);
			cxt.DFillRect(rc, theGray2);
			cxt.Redraw();
		}
		else
		{
			gj_ = 0;
			rcmove_ = nullptr;

			if (mode_ == 1 )
				mode_ = 3;
			else if (mode_ == 2 )
				mode_ = 0;
		}
	}

	if (mode_ == 3)
	{
		FRectF rc(rc_);
		D2DControls::Draw(cxt);

		rc.SetWidth(BTN_WIDTH);
		cxt.DFillRect(rc, theGray2);

		rc_.SetWidth(BAR_WIDTH);
	}	
	else if (mode_ == 0)
	{
		FRectF rc(rc_);
		rc.SetWidth(BTN_WIDTH);
		rc_.SetWidth(BTN_WIDTH);
		cxt.DFillRect(rc, theGray2);

		mat.PushTransform();
		{
			mat.Offset(rc_);
			
			mat._11 = 0.0f;
			mat._12 = 1.0f;
			mat._21 = -mat._12;
			mat._22 = mat._11;

			mat.SetTransform();

			cxt.DText(FPointF(5,-BTN_WIDTH), text_.c_str(), theBlack);

		}
		mat.PopTransform();
	}

	mat.PopTransform();
}

LRESULT D2DAccordionbar::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT hr = 0;

	if ( mode_ == 0 )
	{
		switch( message )
		{
			case WM_LBUTTONDOWN:
			{
				MouseParam& pm = *(MouseParam*)lParam;
				FPointF pt = mat_.DPtoLP(pm.pt);				
				if (rc_.PtInRect(pt))
				{
					APP.SetCapture(this);
					OpenCloseBar(true);
					hr = 1;
				}
			}
			break;
			case WM_LBUTTONUP:
			{
				MouseParam& pm = *(MouseParam*)lParam;
				FPointF pt = mat_.DPtoLP(pm.pt);				
				if (rc_.PtInRect(pt))
				{
					

					hr = 1;
				}
			}
			break;
		}
	}
	else if ( mode_ == 3 )
	{
		switch( message )
		{
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_MOUSEMOVE:
			{
				MouseParam& pm = *(MouseParam*)lParam;
				FPointF pt = mat_.DPtoLP(pm.pt);				
				if (!rc_.PtInRect(pt) && APP.IsCapture(this))
				{					
					if (message == WM_LBUTTONDOWN)
					{
						OpenCloseBar(false);

						APP.ReleaseCapture();
					}
					
					hr = 1;
					return hr;
				}
			}
		}

					
		hr = D2DControls::DefWndProc(b,message,wParam,lParam);
		

		if ( hr == 0 )
		{
			switch( message )
			{
				case WM_KEYDOWN:
				{
					auto key = 0xff & wParam;

					if ( key == VK_ESCAPE && APP.IsCapture(this) )
					{
						OpenCloseBar(false);

						APP.ReleaseCapture();
					
						hr = 1;
					}
				}
				break;
			}
		}
	}
	return hr;

}
void D2DAccordionbar::OpenCloseBar(bool bOpen)
{
	auto nf = std::shared_ptr<FRectF[]>( new FRectF[STEP_CNT]);
	
	rcmove_ = nf; //std::make_shared<FRectF[]>(STEP_CNT);

	if ( bOpen )
	{
		FRectF rcs(FPointF(),FSizeF(0,rc_.Height()));
		FRectF rce(FPointF(),FSizeF(BAR_WIDTH,rc_.Height()));

		RectAnimation(rcs,rce, rcmove_.get(), STEP_CNT, 0);

		mode_ = 1;
		gj_ = 0;
	}
	else
	{
		FRectF rcs(FPointF(),FSizeF(BAR_WIDTH,rc_.Height()));
		FRectF rce(FPointF(),FSizeF(0,rc_.Height()));
					
		RectAnimation(rcs,rce, rcmove_.get(), STEP_CNT, 0);

		mode_ = 2;
		gj_ = 0;
	}
}
void D2DAccordionbar::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControls::CreateControl(parent, pacontrol,rc, stat, name, local_id);
	mode_ = 0;
	text_ = L"ƒ{ƒ^ƒ“";
}
std::wstring D2DAccordionbar::GetTreeTyp(USHORT* typ)
{ 
	*typ=2; 
	return L"D2DAccordionbar";
}