﻿#include "pch.h"
#include "D2DWindow.h" 
#include "D2DMDISplitControls.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())


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
	rc.top = rc.bottom-25;
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
			auto cx = (float)LOWORD(lParam);
			auto cy = (float)HIWORD(lParam);

			rc_.SetSize(FSizeF(cx,cy));
		}

		break;

	}


	if ( r == 0 )
		r = D2DControls::WndProc(b,message,wParam,lParam);




	return r;

}

std::shared_ptr<D2DMDISplitChild> D2DMDISplitFrame::Add(int typ, DWORD stat, LPCWSTR name, int id)
{
	short idx = (short)prvRect_.size();
	auto r = std::make_shared<D2DMDISplitChild>(idx);

	FRectF rc(0,0,rc_.Size());

	r->CreateControl( this->GetParent(), this, rc, stat, name, id);

	controls_.push_back(r);

	//prvRect_[r->idx_]=r->GetRect();

	return r;


}

// /////////////////////////////////////////////////////////////////////////////////////

D2DMDISplitChild::D2DMDISplitChild()
{
	sz_ = FSizeF(300,1000);
}
D2DMDISplitChild::D2DMDISplitChild(short idx)
{
	sz_ = FSizeF(300,1000);
}
D2DMDISplitChild::~D2DMDISplitChild()
{

}

void D2DMDISplitChild::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();


	//cxt.DFillRect(rc_, D2RGB(105,100,100)); 

	cxt.DText(FPointF(), this->name_.c_str(), D2RGB(170,170,170));

	
	mat.PushTransform();
	mat.Offset(0, -scv_->LogicalOffset());

	float y = 100.0f;
	while( y < sz_.height )
	{
		cxt.DFillRect(FRectF(0,y,sz_.width,y+1), theWhite);


		y+=100.0f;
	}
	mat.PopTransform();

	mat.PushTransform();
	mat.Offset(300-BARW, 0 );
	scv_->Draw(cxt);
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
			auto cx = (float)LOWORD(lParam);
			auto cy = (float)HIWORD(lParam);

			rc_.SetSize(FSizeF(cx,cy));
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


	FRectF rc1(0,0,BARW,rc_.Height());
	auto scV = std::make_shared<D2DScrollbar>();
	scV->CreateControl(parent, this, rc1,STAT_DEFAULT, NONAME );
	this->Add(scV);
	scv_ = scV;

}

// ///////////////////////////////////////////////////////////////////
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
	if (max_size_ <= view_size_ )
	{
		thumb_size_ = view_size_;
		return offset_ * 1.0f;
	}
	else if (max_size_ < view_size_ * 2 )
	{
		thumb_size_ = (max_size_-view_size_);
		return offset_ * 1.0f;
	}
	else
	{
		thumb_size_ = 10; //(max_size_ - view_size_);
		return offset_ * (max_size_ - view_size_)/ view_size_;
	}



}

void D2DScrollbar::Draw(D2DContext& cxt)
{
	FRectF rc(0,0,sz_);
	
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();
	
	if (bVertical_)
	{
		cxt.DFillRect(rc, theBlack );

		FRectF thum(0,offset_,20, offset_+Thumheight());

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
				MouseParam& pm = *(MouseParam*)lParam;
				auto pt1 = mat_.DPtoLP(pm.pt);
				auto pt2 = mat_.DPtoLP(pm.ptprv);

				offset_ = max(0.0f, (offset_ + (bVertical_ ? pt1.y-pt2.y : pt1.x-pt2.x )));

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


	}

	return 0;
}

void D2DScrollbar::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);
	sz_ = rc.Size();

	

	bVertical_ = (sz_.height > sz_.width );

	view_size_ = (bVertical_ ? sz_.height : sz_.width);
	max_size_ = view_size_ * 1.5;
	thumb_size_ = 0;

	offset_ = 0;
}