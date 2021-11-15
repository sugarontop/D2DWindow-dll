#include "pch.h"
#include "D2DWindow.h" 
#include "D2DMDISplitControls.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())
#define BOTTOM_BAR		25.0f


D2DMDISplitFrame::D2DMDISplitFrame()
{
	split_line_x_ = 100;
}

void D2DMDISplitFrame::Draw(D2DContext& cxt)
{
	
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();

	FRectF rc = rc_;
	rc.bottom = rc.bottom-25;
	rc.right = rc.left + split_line_x_;

	FRectF rc1 = rc;

	cxt.DFillRect(rc1, D2RGBA(170,170,170,255));

	rc.left = rc.left + split_line_x_;
	rc.right = rc_.right;
	cxt.DFillRect(rc, D2RGBA(150,150,150,255));


	rc = rc_;
	rc.top = rc.bottom-BOTTOM_BAR;
	cxt.DFillRect(rc, D2RGBA(0,122,204,255));


	for(auto it=controls_.begin(); it != controls_.end(); it++ )
	{
		auto rc1 = (*it)->GetRect();			
		D2DRectFilter f(cxt, rc1);

		mat.PushTransform();
		(*it)->Draw(cxt);
		mat.PopTransform();
	}

	mat.PopTransform();
}
HRESULT D2DMDISplitFrame::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT r = 0;

	switch( message )
	{
		case WM_SIZE:
		{
			FRectF rc1 = *(FRectF*)(lParam);

			FSizeF sz = rc1.Size();

			if ( child_typ_ == 1 )
			{
				auto right = controls_[0]->GetRect().right;

				rc_.SetSize(sz);


				FRectF rc(0,0,right, sz.height-BOTTOM_BAR);
			
				controls_[0]->WndProc(b,WM_D2D_SET_SIZE, 0,(LPARAM)&rc);
						

				rc.left = right;
				rc.right = sz.width;
				rc.top = 0;
				rc.bottom = sz.height-BOTTOM_BAR;

				controls_[1]->WndProc(b,WM_D2D_SET_SIZE, 0,(LPARAM)&rc);
			}			

			return 0;

		}

		break;

	}


	if ( r == 0 )
		r = D2DControls::WndProc(b,message,wParam,lParam);




	return r;

}

void D2DMDISplitFrame::CreateChildView(int typ, float left_width)
{
	child_typ_ = typ;

	if ( child_typ_ == 1 )
	{
		// 2分割 window
		split_line_x_ = left_width;
		LPCWSTR nm[2] = { L"LEFT", L"RIGHT" };
		float ww[2] = { split_line_x_, 1};
		for(int idx=0; idx < 2; idx++ )
		{	
			auto r =  std::make_shared<D2DMDISplitChild>();

			FRectF rc(0,0,ww[idx],1); 

			r->CreateControl(GetParent(), this, rc, STAT_DEFAULT, nm[idx], idx);

			this->control_map_[nm[idx]]= r.get();
			controls_.push_back(r);
	
		}
	}


}


// /////////////////////////////////////////////////////////////////////////////////////


D2DMDISplitChild::D2DMDISplitChild()
{
	
}

void D2DMDISplitChild::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();

	D2DControls::Draw(cxt);

	mat.PopTransform();
}

HRESULT D2DMDISplitChild::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT r = 0;

	switch( message )
	{
		case WM_D2D_SET_SIZE:
		{
			FRectF& rc = *(FRectF*)(lParam);
			rc_ = rc;

			this->controls_[0]->WndProc(b,WM_D2D_SET_SIZE,0,(LPARAM)&rc_);

			return 0;
		}
		break;

	}


	if ( r == 0 )
		r = D2DControls::DefWndProc(b,message,wParam,lParam);

	return r;
}
void D2DMDISplitChild::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);
	rc_ = rc;

}

// /////////////////////////////////////////////////////////////////////////////////////

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

// ///////////////////////////////////////////////////////////////////