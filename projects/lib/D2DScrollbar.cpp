#include "pch.h"
#include "D2DWindow.h" 
#include "D2DScrollbar.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())

#define VIEW_SIZE (view_size_-other_scrollbar_size_)

D2DScrollbar::D2DScrollbar()
{
	thumb_size_ = 0;

}
float D2DScrollbar::Thumheight() const
{

	return thumb_size_;

}
void D2DScrollbar::SetSize(const FSizeF& sz)
{ 
	sz_ = sz; 

	view_size_ = (bVertical_ ? sz_.height : sz_.width );

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
	else if (max_size_ < VIEW_SIZE * 3 )
	{
		thumb_size_ = 100;
		return offset_ * (max_size_ - VIEW_SIZE)/ (VIEW_SIZE - thumb_size_);
	}
	else
	{
		thumb_size_ = 10;
		return offset_ * (max_size_ - VIEW_SIZE)/ (VIEW_SIZE - thumb_size_);
	}



}
FSizeF D2DScrollbar::GetSize() const
{
	FSizeF sz(sz_);
	if ( bVertical_ )
		sz.width=BARW;
	else
		sz.height = BARW;

	return sz;

}
void D2DScrollbar::Draw(D2DContext& cxt)
{
	// through
}
void D2DScrollbar::Draw2(D2DContext& cxt)
{
	if ( stat_ & STAT_VISIBLE )
	{
	
		FRectF rc(0,0,GetSize());

		D2DMatrix mat(*cxt);

		mat_ = mat.PushTransform();

		if (bVertical_)
		{
			cxt.DFillRect(rc, D2RGB(50,50,50) );

			FRectF thum(0,offset_, BARW, offset_+Thumheight());

			cxt.DFillRect(thum, theGray3 );
		}
		else
		{

			cxt.DFillRect(rc, D2RGB(50,50,50) );

			FRectF thum(offset_,0, offset_+Thumheight(), BARW);

			cxt.DFillRect(thum, theGray3 );


		}
		mat.PopTransform();
	}
}
LRESULT D2DScrollbar::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;

	if ( stat_ & STAT_ENABLE )
	{
		switch( message )
		{
			case WM_LBUTTONDOWN:
			{
				MouseParam& pm = *(MouseParam*)lParam;
				auto pt = mat_.DPtoLP(pm.pt);
				FRectF rc(0,0,GetSize());

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

		}
	}

	return 0;
}
std::wstring D2DScrollbar::GetTreeTyp(USHORT* typ)
{ 
	*typ=2; 
	return L"D2DScrollbar";
}

void D2DScrollbar::Offset( float off )
{

	offset_ = max(0.0f, (offset_ + off));
	offset_ = min(VIEW_SIZE-thumb_size_, offset_);

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