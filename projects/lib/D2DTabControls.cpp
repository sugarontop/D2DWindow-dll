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
		auto clr1 = D2RGB(220,220,220);
		auto clr2 = D2RGB(0,0,0);

		if ( k == tabidx )
		{
			clr1 = D2RGB(110,110,110);
			clr2 = D2RGB(255,255,255);

		}

		cxt.DFillRect(it, clr1);
		auto nm = this->controls_[k]->GetName();
		cxt.DText(FPointF(it.left+5, it.top), nm.c_str(), clr2);

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

HRESULT D2DTabControls::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT r = 0;

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
				for(auto& it : tabrects_)
				{
					if ( it.PtInRect(pt))
					{
						
						tab_idx_ = k;
						r = 1;

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
		case WM_D2D_SET_SIZE:
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
				it->WndProc(b,WM_D2D_SET_SIZE,0,(LPARAM)&rc);

			return 0;

		}
		break;

	}

	if ( r == 0 )
	{
		r = controls_[tab_idx_]->WndProc(b,message,wParam,lParam);
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


if (name == L"test" )
{
	for(int i = 0; i < 3; i++ )
	{
		WCHAR nm[64];
		wsprintf(nm,L"aNAME_%d", i);

		auto page1 = std::make_shared<D2DControls_with_Scrollbar>();
		page1->CreateControl(parent,this, FRectF(0,0,0,0), STAT_DEFAULT, nm );
		Add(page1);


		if (i==1)
		{			
			UIHandleWin hwin={};

			hwin.p=parent;

			UIHandle hs={};

			hs.p = page1.get();

			auto ha = D2DCreateSquarePaper(hwin,hs, FRectF(0,0,6000,9000),  STAT_DEFAULT, L"MySquarePaper",-1);

			auto krc = rc_;
			for(int ij = 0; ij < 1; ij++ )
			{
				yahoo_finance* yf = new yahoo_finance();
				yf->CreateControl(parent, (D2DControls*)ha.p, FRectF(50+ij*10,150+ij*10,FSizeF(800,500)), STAT_DEFAULT, NONAME );
				((D2DControls*)ha.p)->Add(std::shared_ptr<yahoo_finance>(yf));

				yf->sc_control_ = page1.get();

			}
		}

	}



	for(int i=0; i < 3; i++ )
	{
		FRectF rc(0,0,FSizeF(200,20));
		rc.Offset(i*180,0);
		tabrects_.push_back(rc);

	}
}
else {

	
	for(int i = 0; i < 1; i++ )
	{
		WCHAR nm[64];
		wsprintf(nm,L"NAME_%d", i);

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
		rc.Offset(i*180,0);
		tabrects_.push_back(rc);

	}

}

}

D2DControls* D2DTabControls::AddNewTab(LPCWSTR tabnm)
{
	D2DControls* ret = nullptr;
	if (BITFLG(STAT_SIMPLE))
	{
		auto page1 = std::make_shared<D2DControls>();
		page1->CreateControl(parent_window_,this, FRectF(0,0,rc_.Size()), STAT_DEFAULT, tabnm );
		Add(page1);
		ret = page1.get();
	}
	else
	{
		auto page1 = std::make_shared<D2DControls_with_Scrollbar>();
		page1->CreateControl(parent_window_,this, FRectF(0,0,rc_.Size()), STAT_DEFAULT, tabnm );
		Add(page1);
		ret = page1.get();

	}



	auto i = tabrects_.size();
	FRectF rc(0,0,FSizeF(200,20));
	rc.Offset(i*180,0);
	tabrects_.push_back(rc);

	return ret;
}
void D2DTabControls::DelTab(USHORT idx)
{
	
	

}




