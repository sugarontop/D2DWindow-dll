#include "pch.h"
#include "D2DWindow.h" 
#include "D2DTabControls.h"
#include "D2DSquarePaper.h"
#include "D2DControls_with_Scrollbar.h"
using namespace V6;

#define  APP (D2DApp::GetInstance())



void D2DTabControls::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat.PushTransform();

	mat_ = mat.Offset(rc_);

	auto tab_height = DrawTab(cxt,tab_idx_);

	mat.Offset(0,tab_height);

	controls_[tab_idx_]->Draw(cxt);

	mat.PopTransform();

}

float D2DTabControls::DrawTab(D2DContext& cxt, USHORT tabidx)
{
	D2DMatrix mat(*cxt);
	mat.PushTransform();

	float tab_height = tabrects_[0].Height();

	

	USHORT k = 0;
	for(auto& it : tabrects_)
	{
		auto clr1 = D2RGB(180,180,180);
		auto clr2 = ColorF::Black;

		if ( k == tabidx )
		{
			clr1 = DEFAULT_TAB_ACTIVE_COLOR;
			clr2 = ColorF::White;

		}

		cxt.DFillRect(it, clr1);
		auto tab_nm = this->controls_[k]->GetLocalName();



		cxt.DText(FPointF(it.left+5, it.top), tab_nm.c_str(), clr2);

		k++;
	}


	mat.PopTransform();
	return tab_height;
}

D2DControl* D2DTabControls::GetControlFromIdx(USHORT idx)
{
	if ( idx < (USHORT)controls_.size())
		return this->controls_[idx].get();

	return nullptr;
}

LRESULT D2DTabControls::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;

	switch( message )
	{
		case WM_D2D_CREATE:

			r = 1;
		break;


		case WM_LBUTTONDOWN:
		{
			MouseParam& pm = *(MouseParam*)lParam;

			auto pt = mat_.DPtoLP(pm.pt);

			if ( rc_.PtInRect(pt))
			{
				int k = 0;
				auto old = tab_idx_;
				for(auto& it : tabrects_)
				{
					if ( it.PtInRect(pt))
					{
						tab_idx_ = k;
						r = 1;

						if ( old !=tab_idx_ )
							parent_window_->SendMessage(WM_D2D_TAB_CHANGE, (WPARAM)this, (LPARAM)tab_idx_);
							
						APP.SetCapture(this);
						break;
					}
					k++;
				}
			}

		}
		break;
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		{
			if ( APP.IsCapture(this))
			{
				APP.ReleaseCapture();
				r = 1;
			}
		}
		break;
		case WM_D2D_SET_SIZE_FROM_CHILDWINDOW:
		{
			FSizeF sz = *(FSizeF*)lParam;
			rc_.SetSize(sz);

			sz.height -= tabrects_[0].Height();

			for(auto& it : controls_)
				it->WndProc(b,WM_D2D_SET_SIZE_FROM_CHILDWINDOW,wParam,(LPARAM)&sz);
			return 0;
		}
		break;
		case WM_D2D_SET_SIZE:
		{
			{					
				FRectF rc = *(FRectF*)lParam;
			
				if ( size_fix_ )
				{
					rc = rc_.ZeroRect();
					rc.top += tabrects_[0].Height();
				}
				else
				{
					rc_.SetWH(rc);
			
					rc.top += tabrects_[0].Height();
			
					rc = rc.ZeroRect();
				}
			
				for(auto& it : controls_)
					it->WndProc(b,WM_D2D_SET_SIZE,wParam,(LPARAM)&rc);

			}
			
			return 0;

		}
		break;
		case WM_D2D_TABCONTROL_TAB_NAME:
		{
			std::wstring s = (LPCWSTR)(LPARAM)lParam;

			r = 0;
		}
		break;
		case WM_D2D_SET_TEXT:
		{
			UINT idx = (UINT)wParam;

			if ( idx < controls_.size())
			{
				auto nm = GetName();
				nm += L"@";
				nm += (LPCWSTR)lParam;

				controls_[idx]->SetName(nm.c_str());
				r = 1;
			}
		}
		break;
		case WM_D2D_SET_COLOR:
		{
			ColorF clr = *(ColorF*)lParam;

			if ( wParam == COLOR_IDX_BACK )
				back_ = clr;
			else if ( wParam == COLOR_IDX_FORE )
				fore_ = clr;
			//else if ( wParam == 2 )
			//	border_ = clr;

			r = 1;
		}
		break;

	}

	if ( r == 0 )
	{
		auto ctrls = dynamic_cast<D2DControls*>(controls_[tab_idx_].get());


		if ( message == WM_LBUTTONDOWN )
		{
			int a = 0;

		}


		r = ctrls->InnerWndProc(b,message,wParam,lParam);


		//r = D2DControls::DefWndProc(b,message,wParam,lParam);
	}


	return r;

}

#include "..\D2DUI_1\D2D1UI_1.h"
#include "D2DMDISplitControls.h"
#include "..\Project4\yahoo.h"

void D2DTabControls::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);


	if (rc.Empty())
	{
		auto xrc = parent_control_->GetRect();
		rc_ = xrc.ZeroRect();

	}
	else
		rc_ = rc;


	tab_idx_ = 0;
	size_fix_ = false;

	back_ = ColorF::White;
	fore_ = ColorF::Black;

	for(int i = 0; i < 1; i++ )
	{
		WCHAR nm[64];
		wsprintf(nm,L"%s@%d", name_.c_str(), i);

		if (BITFLG(STAT_SIMPLE))
		{			
			auto page1 = std::make_shared<D2DControls>();
			page1->CreateControl(parent,this, FRectF(0,0,rc_.Size()), STAT_DEFAULT, nm );
			Add(page1);
		}
		else
		{
			auto page1 = std::make_shared<D2DControls_with_Scrollbar>();
			page1->CreateControl(parent,this, FRectF(0,0,rc_.Size()), STAT_DEFAULT, nm );
			Add(page1);
		}
	}
	for(int i=0; i < 1; i++ )
	{
		FRectF rc(0,0,FSizeF(200,20));
		rc.Offset(i*180.0f,0);
		tabrects_.push_back(rc);

	}
}

D2DControls* D2DTabControls::AddNewTab(LPCWSTR tabnm)
{
	D2DControls* ret = nullptr;

	std::wstring _tabnm = name_;
	_tabnm += L"@";
	_tabnm += tabnm;

	if (BITFLG(STAT_SIMPLE))
	{
		auto page1 = std::make_shared<D2DControls>();
		page1->CreateControl(parent_window_,this, FRectF(0,0,rc_.Size()), STAT_DEFAULT, _tabnm.c_str() );
		Add(page1);
		ret = page1.get();
	}
	else
	{
		auto page1 = std::make_shared<D2DControls_with_Scrollbar>();
		page1->CreateControl(parent_window_,this, FRectF(0,0,rc_.Size()), STAT_DEFAULT, _tabnm.c_str() );
		Add(page1);
		ret = page1.get();

	}



	auto i = tabrects_.size();
	FRectF rc(0,0,FSizeF(200,20));
	rc.Offset(i*180.0f,0);
	tabrects_.push_back(rc);

	return ret;
}
void D2DTabControls::DelTab(USHORT idx)
{
	
	

}
std::wstring D2DTabControls::GetTreeTyp(USHORT* typ)
{ 
	*typ=3; 
	return L"D2DTabControls";
}



