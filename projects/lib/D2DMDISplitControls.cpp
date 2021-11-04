#include "pch.h"
#include "D2DWindow.h" 
#include "D2DMDISplitControls.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())
#define BOTTOM_BAR		25.0f
#define INIT_SPLIT_LINE 300.0f


D2DMDISplitFrame::D2DMDISplitFrame():split_line_x_(INIT_SPLIT_LINE)
{

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


	if ( !controls_.empty())
	{
		for(auto it=controls_.begin(); it != controls_.end(); it++ )
		{
			auto rc1 = (*it)->GetRect();			
			D2DRectFilter f(cxt, rc1);

			mat.PushTransform();
			(*it)->Draw(cxt);

			mat.PopTransform();
		}
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
			FSizeF sz = *(FSizeF*)(lParam);

			rc_.SetSize(sz);

			sz.height -= BOTTOM_BAR;

			FRectF rc(0,0,sz);
			
			controls_[0]->WndProc(b,WM_D2D_SET_SIZE, 0,(LPARAM)&rc);


			auto right = controls_[0]->GetRect().right;


			rc.left = right;
			rc.right = sz.width;
			rc.top = 0;
			rc.bottom = sz.height;

			controls_[1]->WndProc(b,WM_D2D_SET_SIZE, 1,(LPARAM)&rc);
			

			return 0;

		}

		break;

	}


	if ( r == 0 )
		r = D2DControls::WndProc(b,message,wParam,lParam);




	return r;

}

void D2DMDISplitFrame::CreateChildView(int typ)
{
	child_typ_ = typ;

	if ( child_typ_ == 0 )
	{
		LPCWSTR nm[2] = { L"LEFT", L"RIGHT" };
		float ww[2] = { split_line_x_, 1};
		for(int idx=0; idx < 2; idx++ )
		{	
			auto r = std::make_shared<D2DMDISplitChild>(idx);

			FRectF rc(0,0,ww[idx],1000); 

			

			r->CreateControl( GetParent(), this, rc, STAT_DEFAULT, nm[idx], idx);

			this->control_map_[nm[idx]]= r.get();

			controls_.push_back(r);
		}
	}


}


// /////////////////////////////////////////////////////////////////////////////////////


D2DMDISplitChild::D2DMDISplitChild(short idx)
{
	vscroll_x_ = 0;
	hscroll_x_ = 0;
}

void D2DMDISplitChild::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();

	cxt.DText(FPointF(), this->name_.c_str(), D2RGB(170,170,170));

	
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

HRESULT D2DMDISplitChild::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT r = 0;

	switch( message )
	{
		case WM_D2D_SET_SIZE:
		{
			if (wParam == 0)
			{
				// LEFT SIDE View
				FRectF& rc = *(FRectF*)(lParam);

				rc_.SetHeight( rc.Height() );

				FRectF xrc(0,0,BARW,rc_.Height());
				scv_->SetRect(xrc);

				scv_->view_size_ = xrc.Height();
				
				xrc = this->controls_[2]->GetRect(); // 0,1 is scrollbar, 2 is child
				scv_->SetMaxSize( xrc.Height());

				vscroll_x_ = rc_.Width()-BARW;
				hscroll_x_ = 0;

				sch_->SetSize(rc_.Size());

				sch_->SetMaxSize(600);
				sch_->view_size_ = 300;
			}
			else if ( wParam == 1 )
			{
				// Right Side View
				FRectF& rc = *(FRectF*)(lParam);

				rc_ = rc;

				
				FRectF xrc(0,0,BARW,rc_.Height());
				scv_->SetRect(xrc);

				scv_->view_size_ = xrc.Height();

				xrc = this->controls_[2]->GetRect(); // 0,1 is scrollbar, 2 is child
				scv_->SetMaxSize( xrc.Height());
				sch_->SetMaxSize( xrc.Width());


				vscroll_x_ = rc_.right-BARW;
				hscroll_x_ = rc_.left;

				sch_->SetSize(rc_.Size());
				sch_->view_size_ = rc.Width();
				
			}
			return 0;
		}
		break;

	}


	if ( r == 0 )
		r = D2DControls::WndProc(b,message,wParam,lParam);




	return r;

}
void D2DMDISplitChild::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);
	rc_ = rc;

	sz_ = rc_.Size();

	FRectF rc1(0,0,BARW,100); //rc_.Height());
	auto scV = std::make_shared<D2DScrollbar>();
	scV->CreateControl(parent, this, rc1,STAT_ENABLE, NONAME );
	this->Add(scV);
	scv_ = scV;


	rc1.SetRect(0,0,100, BARW);
	auto scH = std::make_shared<D2DScrollbar>();
	scH->CreateControl(parent, this, rc1,STAT_DEFAULT, NONAME );
	this->Add(scH);
	sch_ = scH;



	scV->SetMaxSize(sz_.height);
	scH->SetMaxSize(sz_.width);
}

// ///////////////////////////////////////////////////////////////////


#define VIEW_SIZE (view_size_-other_scrollbar_size_)

D2DScrollbar::D2DScrollbar()
{
	thumb_size_ = 0;

}
float D2DScrollbar::Thumheight() const
{

	return thumb_size_;

}
float D2DScrollbar::LogicalOffset() 
{
	if (max_size_ <= VIEW_SIZE )
	{
		thumb_size_ = VIEW_SIZE;
		return offset_ * 1.0f;
	}
	else if (max_size_ < VIEW_SIZE * 2 )
	{
		thumb_size_ = (max_size_-VIEW_SIZE);
		return offset_ * 1.0f;
	}
	else
	{
		thumb_size_ = 10;
		return offset_ * (max_size_ - VIEW_SIZE)/ (VIEW_SIZE - thumb_size_);
	}



}
void D2DScrollbar::Draw(D2DContext& cxt)
{
	// throgh
}
void D2DScrollbar::Draw2(D2DContext& cxt)
{
	FRectF rc(0,0,sz_);
	
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();
	
	if (bVertical_)
	{
		cxt.DFillRect(rc, theBlack );

		FRectF thum(0,offset_, BARW, offset_+Thumheight());

		cxt.DFillRect(thum, theGray3 );
	}
	else
	{

		cxt.DFillRect(rc, theBlack );

		FRectF thum(offset_,0, offset_+Thumheight(), BARW);

		cxt.DFillRect(thum, theGray3 );


	}
	mat.PopTransform();
}
HRESULT D2DScrollbar::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT r = 0;

	switch( message )
	{
		case WM_LBUTTONDOWN:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);
			FRectF rc(0,0,sz_);

			if (rc.PtInRect(pt))
			{
				APP.SetCapture(this);
				r = 1;

			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			if ( APP.GetCapture() == this )
			{
				if ( 0 == GetAsyncKeyState(VK_LBUTTON))
				{
					APP.ReleaseCapture();
				}
				else
				{
					MouseParam& pm = *(MouseParam*)lParam;
					auto pt1 = mat_.DPtoLP(pm.pt);
					auto pt2 = mat_.DPtoLP(pm.ptprv);
	
					offset_ = max(0.0f, (offset_ + (bVertical_ ? pt1.y-pt2.y : pt1.x-pt2.x )));

					offset_ = min(VIEW_SIZE-thumb_size_, offset_);
				}

				b.bRedraw = true;
				r = 1;
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			if ( APP.GetCapture() == this )
			{
				APP.ReleaseCapture();


				r = 1;
			}
		}
		break;
		/*case WM_SIZE :
		{
			FSizeF sz = *(FSizeF*)(lParam);

			if ( this->bVertical_ )
			{
				view_size_ = sz.height;
				sz_.height = sz.height;
			}
			else
			{
				auto f = sz.width;
				view_size_ = sz.width;
				sz_.width = sz.width;
			}


		}
		break;*/


	}

	return 0;
}

void D2DScrollbar::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);
	sz_ = rc.Size();

	

	bVertical_ = ( sz_.height > sz_.width );

	view_size_ = (bVertical_ ? sz_.height : sz_.width );
	max_size_ = view_size_ ;
	thumb_size_ = 0;
	other_scrollbar_size_ = BARW;
	offset_ = 0;
}