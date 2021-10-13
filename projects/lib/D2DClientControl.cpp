#include "pch.h"
#include "D2DClientControl.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())


/////////////////////////////////////////////////////////////////////////////////////////////////////

D2DClientControls::D2DClientControls(LPVOID captureobj, DelegateDrawFunc func1, DelegateProcFunc func2)
{
	DrawFunc_ = func1;
	ProcFunc_ = func2;
	rc_ = FRectF(0,0,0,0);
	captureobj_ = captureobj;
}
void D2DClientControls::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControl::CreateControl(parent, pacontrol, rc, stat, name, local_id);

	rc_ = rc;
}
HRESULT D2DClientControls::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT r = 0;
	
	if (stat_ & STAT_ENABLE)
	{
		if ( message == WM_MOUSEMOVE || message == WM_LBUTTONDOWN || message == WM_LBUTTONUP 
			|| message == WM_RBUTTONDOWN || message == WM_RBUTTONUP || message == WM_LBUTTONDBLCLK )
		{
			// client area外のメッセージは無視する
			MouseParam& m = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(m.pt);

			if ( rc_.PtInRect(pt) == false )
				return r;
		}



		if (ProcFunc_ != nullptr)
		{
			auto old = b.card;
			r = ProcFunc_(captureobj_, b, message, wParam, lParam);
			b.card = old;
		}

		if ( r == 0 )
		{
			r = D2DControls::WndProc(b,message,wParam,lParam);

		}
	}
	return r;
}
void D2DClientControls::Draw(D2DContext& cxt)
{
	_ASSERT(DrawFunc_);

	if (stat_ & STAT_VISIBLE)
	{
		D2DMatrix mat(*cxt);
		mat_ = mat.PushTransform();
		(*cxt)->PushAxisAlignedClip( rc_, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE );		

		cxt.DrawBlackLine(rc_);
		mat.Offset(rc_);

		

		auto old = cxt.temp_;
		DrawFunc_(captureobj_, cxt);
		cxt.temp_ = old;


		D2DControls::Draw(cxt);

		(*cxt)->PopAxisAlignedClip();

		mat.PopTransform();
	}
	
}