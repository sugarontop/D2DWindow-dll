#include "pch.h"
#include "D2DButton.h"
#include "D2DMessage.h"
using namespace V6;

#define  APP (D2DApp::GetInstance())

D2DButton::D2DButton()
{

}

void D2DButton::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rc_ = rc;
	text_ =name;


}
void D2DButton::SetText(LPCWSTR str)
{
	text_ = str;
}
// D2DCaptureObject interface
HRESULT  D2DButton::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( (stat_&STAT_ENABLE) == 0 )
		return 0;

	HRESULT ret = 0;
	bool bl = false;
	static int mouse_mode = 0;

	switch( message )
	{
		case WM_LBUTTONDOWN:
		{			
			MouseParam* mp = (MouseParam*)lParam;
			part_stat_ = 0;

			auto pt = mat_.DPtoLP(mp->pt);

			if ( rc_.ZeroPtInRect( pt ) )
			{
				APP.SetCapture(this);

				part_stat_ = 1;
				ret = 1;
			}

		}
		break;
		case WM_LBUTTONUP:
		{
			if (APP.GetCapture() == this)
			{
				MouseParam* mp = (MouseParam*)lParam;
				auto pt = mat_.DPtoLP(mp->pt);

				APP.ReleaseCapture();

				if ( rc_.ZeroPtInRect( pt ) && part_stat_ == 1)
				{				
					{
						D2DNMHDR d;
						d.code = EVENT_PUSH;
						d.sender.p = this;
						d.sender.typ = TYP_BUTTON;

						parent_window_->SendMessage( WM_NOTIFY, id_, (LPARAM)&d );

					}


					ret = 1;
				}
				part_stat_ = 0;
			}

			
		}
		break;
	}
	return ret;
}

void  D2DButton::Draw(D2DContext& cxt) 
{
	if ( stat_&STAT_VISIBLE )
	{
		D2DMatrix mat(*cxt);
		mat.PushTransform();

		mat_ = mat.Offset(rc_); // ¶ã‚ð(0,0)‚Æ‚µ‚Ä•\Ž¦
		
		auto rc = rc_.ZeroRect();

		if (part_stat_ == 1)
			mat.Offset(2, 2);

		(*cxt)->DrawRectangle(rc, cxt.black_);
		(*cxt)->FillRectangle(rc, cxt.white_);

		(*cxt)->DrawText(text_.c_str(), text_.length(), cxt.textformat_, rc, cxt.black_ );

		mat.PopTransform();

	}
}
