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
		cxt.DFillRect(rc_.ZeroRect(), ColorF::Blue);
	}
	else if ( mode_ == 2 )
	{
		mat.Offset(rc_);
		cxt.DFillRect(rc_.ZeroRect(), ColorF::LightPink);

		auto s = L"*** place holder controls ***";
		(*cxt)->DrawText(s,wcslen(s),cxt.textformat_,FRectF(10,10,FSizeF(400,20)), cxt.black_);
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
			const FRectF& rc = *(FRectF*)(lParam);
			if (BITFLG(STAT_IGNORE_HSIZE) == false )
			{
				rc_.SetWidth(rc.Width() - rc_.left);
			}
			if (BITFLG(STAT_IGNORE_VSIZE) == false )
			{
				rc_.SetHeight(rc.Height() - rc_.top);
			}



			wParam=1;
		}
		break;
		case WM_D2D_COMMAND_SET:
		{
			if ( (UINT_PTR)this == (UINT_PTR)wParam)
			{
				LPCWSTR cmd = (LPCWSTR)lParam;

				auto ar = SplitW(cmd,L"&");

				for(auto& it : ar)
				{
					auto ar2 = SplitW(it.c_str(), L"=");

					if ( ar2.size() == 2)
					{
						if (ar2[0] == L"mode")
						{
							mode_ = _wtoi(ar2[1].c_str());
						}
						else if ( ar2[0] == L"enable")
						{
							int a = _wtoi(ar2[1].c_str());

							if (a ==0)
								stat_ &= ~STAT_ENABLE;
							else
								stat_ |= STAT_ENABLE;

						}
					}
				}

				r = 1;
			}
		}
		break;

	}

	if (r == 0 && BITFLG(STAT_ENABLE) && mode_ == 0)
		r = D2DControls::DefWndProc(b,message,wParam,lParam);

	return r;

}

