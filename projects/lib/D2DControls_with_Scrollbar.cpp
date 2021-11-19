
#include "pch.h"
#include "D2DWindow.h" 
#include "D2DControls_with_Scrollbar.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())
#define BOTTOM_BAR		25.0f


D2DControls_with_Scrollbar::D2DControls_with_Scrollbar()
{
	vscroll_x_ = 0;
	hscroll_x_ = 0;
}

void D2DControls_with_Scrollbar::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();
#ifdef _DEBUG
	cxt.DText(FPointF(), this->name_.c_str(), D2RGB(170,170,170));
#endif
	
	//_ASSERT(rc_.Width() != 0 && rc_.Height()!=0 );
	D2DRectFilter f(cxt, rc_);

	mat.PushTransform();
	mat.Offset(-sch_->LogicalOffset(), -scv_->LogicalOffset());

	D2DControls::Draw(cxt);

	mat.PopTransform();

	mat.PushTransform();
	mat.Offset(vscroll_x_, 0 );
	scv_->Draw2(cxt);
	mat.PopTransform();

	mat.PushTransform();
	mat.Offset(hscroll_x_, rc_.Height()-BARW );
	sch_->Draw2(cxt);
	mat.PopTransform();


	mat.PopTransform();
}

HRESULT D2DControls_with_Scrollbar::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT r = 0;

	switch( message )
	{
		case WM_SIZE:
		case WM_D2D_SET_SIZE:
		{
			if (controls_.size() == 3)
			{
				if (wParam == 0 || wParam == 1)
				{
					FRectF& rc = *(FRectF*)(lParam);

					
					if ( wParam == 0 )
						rc_.SetWH(rc);
				
					auto crc = this->controls_[2]->GetRect(); // 0,1 is scrollbar, 2 is child

					vscroll_x_ = rc_.Width()-BARW;
					hscroll_x_ = 0;

					scv_->SetStat(STAT_DEFAULT);
					sch_->SetStat(STAT_DEFAULT);

					scv_->SetMaxSize( crc.Height());
					sch_->SetMaxSize(crc.Width());				
					sch_->SetSize(rc_.Size());
					scv_->SetSize(rc_.Size());

					crc = rc_.ZeroRect();
					this->controls_[2]->WndProc(b,message,1,(LPARAM)&crc);
				}				
			}

			return 0;
		}
		break;
		

	}


	if ( r == 0 )
		r = D2DControls::WndProc(b,message,wParam,lParam);




	return r;

}
void D2DControls_with_Scrollbar::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);
	rc_ = rc;

	vscroll_x_ = 0;
	hscroll_x_ = 0;

	FRectF rc1(0,0,BARW,100); 
	auto scV = std::make_shared<D2DScrollbar>();
	scV->CreateControl(parent, this, rc1,0, NONAME );
	this->Add(scV);
	scv_ = scV;


	rc1.SetRect(0,0,100, BARW);
	auto scH = std::make_shared<D2DScrollbar>();
	scH->CreateControl(parent, this, rc1,0, NONAME );
	this->Add(scH);
	sch_ = scH;



	scV->SetMaxSize(rc_.Height());
	scH->SetMaxSize(rc_.Width());
}

D2DControls* D2DControls_with_Scrollbar::GetMainControls()
{
	_ASSERT( controls_.size() == 3);

	return dynamic_cast<D2DControls*>(controls_[2].get());
}

// /////////////////////////////////////////////////////////////////////////////////////////////////

void D2DControls_with_Move::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();
	
	//D2DRectFilter f(cxt, rc_);

	FRectF rc = controls_[0]->GetRect();
	mat.Offset(rc_.left-rc.left, rc_.top-rc.top);
	
	controls_[0]->Draw(cxt);

	mat.PopTransform();
	
	//D2DControls::Draw(cxt);

	mat.PushTransform();
	

	mat.Offset(rc_);
	auto sz = rc_.Size();
	
	rc.SetRect(0,0,10,10); cxt.DFillRect(rc, ColorF::Red);
	rc.SetRect(sz.width-10,0,FSizeF(10,10)); cxt.DFillRect(rc, ColorF::Red);
	rc.SetRect(0,sz.height-10,FSizeF(10,10)); cxt.DFillRect(rc, ColorF::Red);
	rc.SetRect(sz.width-10,sz.height-10,FSizeF(10,10)); cxt.DFillRect(rc, ColorF::Red);
	mat.PopTransform();

	
}
void D2DControls_with_Move::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);
	rc_ = rc;

}
HRESULT D2DControls_with_Move::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT r = 0;

	switch( message )
	{
		case WM_LBUTTONDOWN:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);
			if ( rc_.PtInRect(pt))
			{
				APP.SetCapture(this);

				r = 1;
			}
			else if ( target_ )
			{
				target_->SetNewParent(prv_controls_);

				target_->SetRect(rc_);
				
				target_ = nullptr;
				prv_controls_ = nullptr;

				

				r = 1;

				this->DestroyControl();

			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			if ( APP.GetCapture() == this )
			{
				MouseParam& pm = *(MouseParam*)lParam;

				auto pt = mat_.DPtoLP(pm.pt);
				auto pt1 = mat_.DPtoLP(pm.ptprv);

				auto offx = pt.x-pt1.x;
				auto offy = pt.y-pt1.y;
				rc_.Offset(offx,offy);

				b.bRedraw = true;
				r = 1;

			}

		}
		break;
		case WM_LBUTTONUP:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			if ( APP.GetCapture() == this )
			{
				APP.ReleaseCapture();


				r = 1;
			}
		}		
		break;
	}


	if ( r == 0 )
		r = D2DControls::WndProc(b,message,wParam,lParam);

	return r;

}