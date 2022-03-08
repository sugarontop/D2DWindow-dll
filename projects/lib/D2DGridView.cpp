#include "pch.h"
#include "D2DGridView.h"


using namespace V6;

void D2DGridView::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rc_ = rc;
}
D2DGridView::~D2DGridView()
{
	Clear();
}

void D2DGridView::Clear()
{
	for(auto& it : Items_ )
	{
		for(auto i=0; i <it.colcnt; i++)
			it.value[i]->Release();

		delete [] it.value;
	}
	Items_.clear();
}

void D2DGridView::Draw(D2DContext& cxt)
{
	if (stat_ & STAT_VISIBLE)
    {
        D2DMatrix mat(*cxt);
        mat.PushTransform();
        (*cxt)->DrawRectangle(rc_, cxt.black_);

        D2DRectFilter fil(cxt, rc_ );

        cxt.DFillRect(rc_, D2RGB(240, 240, 240));
        mat_ = mat.PushTransform();
        mat.Offset(rc_);
		auto br = cxt.black_;
		FPointF pt(3,2), pt2(80.f, 2);		
		
		(*cxt)->DrawLine(FPointF(pt2.x-3,0), FPointF(pt2.x-3, rc_.Height()), br, 0.5f);
	
		FRectF rc(0,25.8f,rc_.Width(),26.0f);
        for(auto& it : Items_ )
		{
			_ASSERT(it.colcnt >= 2);
			(*cxt)->DrawTextLayout(pt,it.value[0],br, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_CLIP);
			(*cxt)->DrawTextLayout(pt2,it.value[1],br,D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_CLIP);

			(*cxt)->DrawRectangle(rc, br );
			mat.Offset(0, 26.0f);
		}


		

		mat.PopTransform();
	}
}
LRESULT D2DGridView::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;
	
	switch(message)
	{
		case WM_D2D_CREATE:

			r = 1;
		break;
		case WM_D2D_COMMAND_SET:
		{
			if ( (UINT_PTR)this == (UINT_PTR)wParam)
			{
				LPCWSTR cmdstr = (LPCWSTR)lParam;

				auto ar = SplitW(cmdstr,L"&");
				std::wstring cmd;
				
				std::wstring title;
				for(auto& it : ar)
				{
					auto ar2 = SplitW(it.c_str(), L"=");

					if ( ar2.size() == 1 )
					{
						cmd = ar2[0];

					}
					else if ( ar2.size() == 2)
					{
						if ( cmd==L"add" && (ar2[0] == L"title") )
						{
							title = ar2[1];
						}	
						else if ( cmd==L"add" && (ar2[0] == L"val") )
						{
							std::wstring& val = ar2[1];

							Row r;
							r.colcnt = 2;
							r.value = new PIDWriteTextLayout[2];

							auto& cxt = this->GetParent()->GetContext();

							IDWriteTextLayout *playout, *playout2;
							if (SOK(cxt.wfactory_->CreateTextLayout(title.c_str(), title.length(), cxt.textformat_, 1000,1000, &playout )))
								r.value[0] = playout;
							if (SOK(cxt.wfactory_->CreateTextLayout(val.c_str(), val.length(), cxt.textformat_, 1000,1000, &playout2 )))
								r.value[1] = playout2;

							Items_.push_back(r);
						}
					}

				}
				r = 1;
			}


		}
		break;

	}
	return r;


}