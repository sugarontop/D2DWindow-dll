#include "pch.h"
#include "D2DWhiteControl.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())


/////////////////////////////////////////////////////////////////////////////////////////////////////

D2DWhiteControl::D2DWhiteControl(LPVOID captureobj, DelegateDrawFunc func1, DelegateProcFunc func2)
{
	DrawFunc_ = func1;
	ProcFunc_ = func2;
	rc_ = FRectF(0,0,0,0);
	captureobj_ = captureobj;
}

D2DWhiteControl::~D2DWhiteControl()
{
	if (ProcFunc_ != nullptr)
	{
		AppBase b;
		b.hWnd=0;
		ProcFunc_(captureobj_, b, WM_D2D_DESTROY, 0, 0 );
	}
}

void D2DWhiteControl::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControl::CreateControl(parent, pacontrol, rc, stat, name, local_id);

	rc_ = rc;
}
LRESULT D2DWhiteControl::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;
	
	if (stat_ & STAT_ENABLE)
	{

		if ( message == WM_SIZE)
		{
			int a = 0;
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
void D2DWhiteControl::Draw(D2DContext& cxt)
{
	_ASSERT(DrawFunc_);

	if (stat_ & STAT_VISIBLE)
	{
		auto old = cxt.temp_;
		DrawFunc_(captureobj_, cxt);
		cxt.temp_ = old;
	


		D2DMatrix mat(*cxt);
		mat.PushTransform();
		mat.Offset(rc_);
		D2DControls::Draw(cxt);

		mat.PopTransform();
	}
	
}