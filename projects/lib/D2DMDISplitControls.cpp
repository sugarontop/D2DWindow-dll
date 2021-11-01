#include "pch.h"
#include "D2DWindow.h" 
#include "D2DMDISplitControls.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())
#define BOTTOM_BAR 25.0f


D2DMDISplitFrame::D2DMDISplitFrame():split_line_x_(300)
{

}
D2DMDISplitFrame::~D2DMDISplitFrame()
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
		
		{
			D2DRectFilter f(cxt, rc1);
			controls_[0]->Draw(cxt);
		}


		mat.Offset(rc_);	
		mat.Offset(split_line_x_, 0);	

		for(auto it=controls_.begin()+1; it != controls_.end(); it++ )
			(*it)->Draw(cxt);
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

			D2DControls::WndProc(b,message,wParam,(LPARAM)&sz);
			

			return 0;

		}

		break;

	}


	if ( r == 0 )
		r = D2DControls::WndProc(b,message,wParam,lParam);




	return r;

}

std::shared_ptr<D2DControls> D2DMDISplitFrame::Add(int typ, float width, DWORD stat, LPCWSTR name, int id)
{
	if ( typ == 0 )
	{	
		short idx = (short)prvRect_.size();
		auto r = std::make_shared<D2DMDISplitChild>(idx);

		FRectF rc(0,0,width, 100); //rc_.Size());

		rc.bottom -= BOTTOM_BAR;

		r->CreateControl( this->GetParent(), this, rc, stat, name, id);

		controls_.push_back(r);

		return r;
	}
	else if ( typ == 1 )
	{	
		short idx = (short)prvRect_.size();
		auto r = std::make_shared<D2DControls>();

		FRectF rc(0,0,5000,1000); //width, 100); //rc_.Size());

		rc.bottom -= BOTTOM_BAR;

		r->CreateControl( this->GetParent(), this, rc, stat, name, id);

		controls_.push_back(r);

		


		return r;
	}




}

// /////////////////////////////////////////////////////////////////////////////////////


D2DMDISplitChild::D2DMDISplitChild(short idx)
{
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
	mat.Offset(rc_.Width()-BARW, 0 );
	scv_->Draw2(cxt);
	mat.PopTransform();

	mat.PushTransform();
	mat.Offset(0, rc_.Height()-BARW );
	sch_->Draw2(cxt);
	mat.PopTransform();


	mat.PopTransform();
}

HRESULT D2DMDISplitChild::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT r = 0;

	switch( message )
	{
		case WM_SIZE:
		{
			FSizeF sz = *(FSizeF*)(lParam);

			rc_.SetHeight( sz.height );

			
			for(auto& it : controls_)
			{
				if ( dynamic_cast<D2DScrollbar*>(it.get()) )
					it->WndProc(b,message,wParam,lParam);
				else
				{	
					it->WndProc(b,message,wParam,lParam);

					auto sz = it->GetRect().Size();
					scv_->SetMaxSize(sz.height); 
					sch_->SetMaxSize(sz.width); 

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
void D2DMDISplitChild::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);
	rc_ = rc;

	sz_ = rc_.Size(); //FSizeF(300,3000);

	FRectF rc1(0,0,BARW,rc_.Height());
	auto scV = std::make_shared<D2DScrollbar>();
	scV->CreateControl(parent, this, rc1,STAT_ENABLE, NONAME );
	this->Add(scV);
	scv_ = scV;


	rc1.SetRect(0,0,rc_.Width(), BARW);
	auto scH = std::make_shared<D2DScrollbar>();
	scH->CreateControl(parent, this, rc1,STAT_DEFAULT, NONAME );
	this->Add(scH);
	sch_ = scH;



	scV->SetMaxSize(sz_.height); // 3000.0f);
	scH->SetMaxSize(sz_.width); // 600.0f);
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
		case WM_SIZE :
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
				//view_size_ = sz.width;
				//sz_.width = sz.width;
			}


		}
		break;


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