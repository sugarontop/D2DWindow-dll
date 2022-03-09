#include "pch.h"
#include "D2DGridView.h"


using namespace V6;

void D2DGridView::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	fore_ = D2RGB(0,0,0);
	back_ = D2RGB(255,255,255);
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
	if (BITFLG(STAT_VISIBLE))
    {		
		ComPTR<ID2D1SolidColorBrush> back;
		ComPTR<ID2D1SolidColorBrush> fore;
		
		(*cxt)->CreateSolidColorBrush(fore_, &fore);	
		(*cxt)->CreateSolidColorBrush(back_, &back);

		D2DMatrix mat(*cxt);
        mat.PushTransform();

        D2DRectFilter fil(cxt, rc_ );
		(*cxt)->FillRectangle(rc_,back);

        mat_ = mat.PushTransform();
        mat.Offset(rc_);

		FPointF pt(3,2), pt2(80.f, 2);		
		
		(*cxt)->DrawLine(FPointF(pt2.x-3,0), FPointF(pt2.x-3, rc_.Height()), fore, 1.0f);
	
		FRectF rc(0,25.8f,rc_.Width(),26.0f);
        for(auto& it : Items_ )
		{
			_ASSERT(it.colcnt >= 2);
			(*cxt)->DrawTextLayout(pt,it.value[0],fore, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_CLIP);
			(*cxt)->DrawTextLayout(pt2,it.value[1],fore,D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_CLIP);

			(*cxt)->DrawRectangle(rc, fore );
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
			//var k3 = dd.create("type=grid&id=10105&x=650&y=50&cx=150&cy=450&nm=gd1");
			//k3.set("color=#FFFFA0&bkcolor=#111111");
			//k3.set("add&title=ABCDEFG1&val=000000");

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
						else if (ar2[0] == L"color")
						{
							D2DColor clr(ar2[1].c_str());
							fore_ = clr;
						}
						else if (ar2[0] == L"bkcolor")
						{
							D2DColor clr(ar2[1].c_str());
							back_ = clr;
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