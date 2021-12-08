#include "pch.h"
#include "D2DClassMap.h"
#include "D2DDropdownListbox.h"

using namespace V6;


void D2DClassMap::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);
	mat_ = mat.PushTransform();

	mat.Scale(0.7f,0.7f);


	//cxt.DText(FPointF(100,100), L"D2DClassMap", ColorF::Black);

	for(auto& it : ar_)
	{
		float x = it.col * 120.0f;
		float y = it.row * 60.0f;
		
		WCHAR cb[256];
		_snwprintf_s(cb, 256, L"row=%d col=%d", it.row, it.col);

		auto cs = dynamic_cast<D2DControls*>(it.p);
		auto ls = dynamic_cast<D2DDropdownListbox*>(it.p);

		

		if ( cs == nullptr || ls )
		{
			FRectF rc(x,y+25,FSizeF(100,25));

			

			cxt.DFillRect(rc, D2RGBA( 0,0,200,50));
			cxt.DText(rc.LeftTop(),it.p->GetName().c_str(), ColorF::White);
			//cxt.DText(FPointF(x,y+10),cb, ColorF::Gray);
		}
		else
		{			
			FRectF rc(x,y,FSizeF(110,50));

			cxt.DFillRect(rc, ColorF::Pink);
			cxt.DText(rc.LeftTop(),it.p->GetName().c_str(), ColorF::Black);
			//cxt.DText(FPointF(x,y),cb, ColorF::Black);
		}
	}
	mat.PopTransform();
}
HRESULT D2DClassMap::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT h = 0;


	switch(message )
	{
		case WM_LBUTTONDOWN:
		{
			MouseParam& pm = *(MouseParam*)lParam;

			auto pt = mat_.DPtoLP(pm.pt);
			FRectF rc(0,0,50,50);
			if ( rc.PtInRect(pt))
				ListUp();

			h = 1;
		}
		break;
	}


	return h;
}
void D2DClassMap::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rc_ = rc;

}
void D2DClassMap::ListUp()
{
	std::vector<ControlMapItem> ar;

	parent_window_->ListUp(ar);

	ar_ = ar;
}