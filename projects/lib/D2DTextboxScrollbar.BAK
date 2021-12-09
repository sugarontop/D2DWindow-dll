#include "pch.h"
#include "D2DTextbox.h"
using namespace V6;


Scrollbar::Scrollbar():parent_(0),offbar_y_(0),scbai_(1),pos_(0)
{

}
void Scrollbar::SetTotalHeight( float h )
{ 
	total_height_=h;
	
	Clear();
}
void Scrollbar::Clear()
{
	pos_ = 0;
	offbar_y_ = 0;
	scbai_ = 1;	
	scbarThumbHeight_ = total_height_;
}
float Scrollbar::Scroll()
{
	return offbar_y_ * scbai_;
}
void Scrollbar::Create(ScrollbarParent* parent, FRectF rc )
{
	rc_ = rc;
	parent_ = parent;
	offbar_y_ = 0;
	bvscrollbar_ = (rc.Width() < rc.Height() );
}
HRESULT Scrollbar::WndProc(AppBase& b, UINT msg, WPARAM wp, LPARAM lp)
{
	static FPointF spt;
	static Scrollbar* target = nullptr;

	switch( msg )
	{
		case WM_LBUTTONDOWN:
		{
			MouseParam* mp = (MouseParam*)lp;

			FPointF pt(mp->pt);
			pt = mat_.DPtoLP(pt);
			if ( rc_.PtInRect(pt) && target == nullptr)
			{
				spt = pt;
				target = this;
				return 1;
			}

		}
		break;
		case WM_MOUSEMOVE:
		{			
			MouseParam* mp = (MouseParam*)lp;

			FPointF pt(mp->pt);
			pt = mat_.DPtoLP(pt);			 
			if (target == this)
			{
				auto off = pt.y - spt.y;

				offbar_y_= min( parent_->sc_barTotalHeight(bvscrollbar_)-scbarThumbHeight_,  max(0.0f, offbar_y_ + off));

				spt = pt;
				return 1;
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			MouseParam* mp = (MouseParam*)lp;
			FPointF pt(mp->pt);
			pt = mat_.DPtoLP(pt);
			if (target == this)
			{
				target = nullptr;

				return 1;
			}
			target = nullptr;

		}
		break;
	}
	return 0;
}
float Scrollbar::SetScroll(float offset)
{
	offbar_y_ = max(0, offbar_y_+ offset / scbai_ );
	return 0;
}
void Scrollbar::OnDraw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);
	bool bHbar = bvscrollbar_;

	cxt.DFillRect(rc_, D2RGB(240,240,240));
	mat.PushTransform();
	{
		(*cxt)->GetTransform(&mat_);
		mat.Offset(rc_); 
		mat.PushTransform();
		{
			mat.Offset(0.0f, offbar_y_);	
			float overflow = max(0, parent_->sc_dataHeight(bHbar)-parent_->sc_barTotalHeight(bHbar));
			scbarThumbHeight_ = parent_->sc_barTotalHeight(bHbar)-overflow;
			const float min_thum_height=8.0f;
			scbai_ = 1.0f; 
			if ( scbarThumbHeight_ < min_thum_height  )
			{
				scbarThumbHeight_ = min_thum_height;
				scbai_ = (parent_->sc_dataHeight(bHbar) - parent_->sc_barTotalHeight(bHbar)) / (parent_->sc_barTotalHeight(bHbar)-scbarThumbHeight_ ); 
			}

			FRectF scbar( 2, 0, rc_.Width()-2,  scbarThumbHeight_);
			cxt.DFillRect(scbar, D2RGB(220,220,220));
		}
		mat.PopTransform();
	}
	mat.PopTransform();
}