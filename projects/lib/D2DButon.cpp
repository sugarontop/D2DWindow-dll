#include "pch.h"
#include "D2DButton.h"
#include "D2DMessage.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())
std::vector<std::wstring> SplitW( LPCWSTR str, LPCWSTR split );

void D2DButton::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rc_ = rc;
	text_ =name;

	ResourceUpdate(true, parent->GetContext());

}
void D2DButton::SetText(LPCWSTR str)
{
	text_ = str;
	textlayout_ = nullptr;
}
std::wstring D2DButton::GetTreeTyp(USHORT* typ)
{ 
	*typ=2; 
	return L"D2DButton";
}

LRESULT  D2DButton::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( !BITFLG(STAT_ENABLE) )
	{
		switch( message )
		{
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_LBUTTONDBLCLK:
				MouseParam* mp = (MouseParam*)lParam;
				auto pt = mat_.DPtoLP(mp->pt);
				if ( rc_.ZeroPtInRect( pt ) )
					return 1; // do nothing,
			break;
		}
		return 0;
	}

	LRESULT ret = 0;
	static int mouse_mode = 0;

	switch( message )
	{
		case WM_MOUSEMOVE:
		{
			MouseParam* mp = (MouseParam*)lParam;
			auto pt = mat_.DPtoLP(mp->pt);

			auto old = stat_;
			if ( rc_.ZeroPtInRect( pt ) )
				stat_ |= STAT_FLOATING;
			else
				stat_ &= ~STAT_FLOATING;

			if ( old != stat_ )
				b.bRedraw = true;
		}
		break;
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
						d.code = EVENTID_CLICK;
						d.sender.p = this;
						d.sender.typ = TYP_BUTTON;
						d.sender_parent = parent_control_;


						if ( click_ )
						{
							click_(this, L"CLICK", nullptr);

						}
						else
						{
							ret = GetParentControls()->SendMesage(WM_NOTIFY, id_, (LPARAM)&d );

							if ( ret == 0 )
								parent_window_->SendMessage( WM_NOTIFY, id_, (LPARAM)&d );
						}

					}


					ret = 1;
				}
				part_stat_ = 0;
			}

			
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
			MouseParam* mp = (MouseParam*)lParam;
			auto pt = mat_.DPtoLP(mp->pt);
			if ( rc_.ZeroPtInRect( pt ) )
				ret=1; // do nothing,
		}
		break;
		case WM_D2D_COMMAND_SET:
		{
			if ( (UINT_PTR)this == (UINT_PTR)wParam)
			{
				LPCWSTR cmd = (LPCWSTR)lParam;

				auto ar = SplitW(cmd,L"&");

				for(auto& it : ar)
				{
					auto ar2 = SplitW(it.c_str(), L"=");

					if ( ar2.size() == 2)
					{
						if (ar2[0] == L"text")
						{
							SetText(ar2[1].c_str());
						}
						else if ( ar2[0] == L"enable")
						{
							int a = _wtoi(ar2[1].c_str());

							if (a ==0)
								stat_ &= ~STAT_ENABLE;
							else
								stat_ |= STAT_ENABLE;

						}
					}
				}

				ret = 1;
			}
		}
		break;
	}
	return ret;
}


FPointF CreateCenterTextLayout(D2DContext& cxt, const std::wstring& str, const FRectF& rc, IDWriteTextLayout** ppout )
{
	if (SOK(cxt.wfactory_->CreateTextLayout(str.c_str(), str.length(), cxt.textformat_, rc.Width(), rc.Height(), ppout )))
	{
		DWRITE_TEXT_METRICS t;
		THR((*ppout)->GetMetrics(&t));

		return FPointF( (rc.Width()-t.width)/2.0f, (rc.Height()-t.height)/2.0f );
	}
	return FPointF();
}


void  D2DButton::Draw(D2DContext& cxt) 
{
	if ( BITFLG(STAT_VISIBLE) )
	{
		D2DMatrix mat(*cxt);
		mat.PushTransform();

		mat_ = mat.Offset(rc_); // ¶ã‚ð(0,0)‚Æ‚µ‚Ä•\Ž¦
		
		auto rc = rc_.ZeroRect();

		if (part_stat_ == 1)
			mat.Offset(2, 2);

		(*cxt)->DrawRectangle(rc, cxt.black_);
		
		
		
		if ( BITFLG(STAT_FLOATING))
		{
			ComPTR<ID2D1LinearGradientBrush> br;
			CreateButtonBrush(cxt, rc_.Height(), false, &br);
			(*cxt)->FillRectangle(rc, br);
		}
		else		
			(*cxt)->FillRectangle(rc, br_);

		if ( textlayout_ == nullptr )
			ptText_ = CreateCenterTextLayout(cxt, text_, rc, &textlayout_ );

		if ( BITFLG(STAT_ENABLE))
			(*cxt)->DrawTextLayout(ptText_, textlayout_, cxt.black_ );
		else
		{
			ComPTR<ID2D1SolidColorBrush> br;
			cxt.CreateBrush(D2RGB(120,120,120), &br);
			(*cxt)->DrawTextLayout(ptText_, textlayout_, br );
		}


		mat.PopTransform();

	}
}
void D2DButton::ResourceUpdate(bool bCreate, D2DContext& cxt)
{	
	br_.Release();		

	if ( bCreate )
		CreateButtonBrush(cxt, rc_.Height(), true, &br_);
}
void D2DButton::CreateButtonBrush(D2DContext& cxt, float height, bool normal, ID2D1LinearGradientBrush** pbr)
{
	D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES linearGradientBrushProperties = D2D1::LinearGradientBrushProperties(
            D2D1::Point2F(0,0),
            D2D1::Point2F(0,height));

	ComPTR<ID2D1GradientStopCollection> pGradientStops;

	auto clr = (normal ? D2D1::ColorF::LightGray : D2D1::ColorF::LightSlateGray);

	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::White, 1);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(clr, 1);
	gradientStops[1].position = 1.0f;

	THR((*cxt)->CreateGradientStopCollection(gradientStops, 2,&pGradientStops));
		
	THR((*cxt)->CreateLinearGradientBrush(linearGradientBrushProperties,pGradientStops,pbr));

}

// ///////////////////////////////////////////////////////////////////////////////////

void InnerMessageBox::ModalShow(LPCWSTR text,LPCWSTR title )
{
	APP.SetCapture(this);

	msg_ = text;
	title_ = title;
	mode_ = 0;
}
void InnerMessageBox::Draw(D2DContext& cxt)
{
	if ( IsVisible())
	{
		

		cxt.DFillRect(rc_, ColorF::Orange);

		D2DMatrix mat(*cxt);

		mat_ = mat.PushTransform();
		mat.Offset( rc_ );

		FRectF rc = rc_.ZeroRect();
		
		rc.Offset(20,0);
		(*cxt)->DrawText(title_.c_str(), title_.length(), cxt.textformat_, rc, cxt.white_ );

		rc.SetRect(rc_.ZeroRect());

		rc.Offset(0,20);
		rc.Inflate(-10,-10);
		rc.bottom -= 20;

		cxt.DFillRect(rc, ColorF::LightGray);

		rc.Inflate(-5,-10);

		//cxt.DFillRect(rc,ColorF::Black);
		(*cxt)->DrawText(msg_.c_str(), msg_.length(), cxt.textformat_, rc, cxt.black_ );

		mat.PopTransform();
	}

}
LRESULT InnerMessageBox::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch( message )
	{
		case WM_D2D_CREATE :
		{
			return 1;
		}
		break;
		case WM_KEYDOWN:
		{
			auto key = 0xff & wParam;

			if ( key == VK_ESCAPE && APP.IsCapture(this) )
			{
				APP.ReleaseCapture();

				DestroyControl();
				return 1;
			}
		}
		break;
		case WM_LBUTTONDOWN:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);
			mode_ = 0;
			if ( rc_.PtInRect(pt))
			{
				mode_ = 1;				
			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);
			auto pt2 = mat_.DPtoLP(pm.ptprv);

			if ( mode_ == 1 )
			{
				rc_.Offset(pt.x-pt2.x, pt.y-pt2.y);
				b.bRedraw = true;
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			mode_ = 0;

		}
		break;
	}
	return ( APP.IsCapture(this) ? 1 : 0 );
}

int D2DWindow::MessageBox(const FRectF& rc, LPCWSTR text, LPCWSTR title)
{
	auto msgbox = std::make_shared<InnerMessageBox>();
	
	msgbox->CreateControl(this, top_control_.get(), rc, STAT_DEFAULT|STAT_MODAL, L"msgbox" );

	top_control_->Add( msgbox );

	msgbox->ModalShow(text,title);
	
	return 0;
}

// ///////////////////////////////////////////////////////////////////////////////////

void InnerFloatingMenu::ModalShow(LPVOID sender,std::vector<MenuItem>& ar )
{
	APP.SetCapture(this);

	items_ = ar;
	floating_idx_ = -1;
	sender_ = sender;
}
void InnerFloatingMenu::Draw(D2DContext& cxt)
{
	if ( IsVisible())
	{
		float cy = 0;
		for(auto& it : items_ )
			cy += (it.typ == 0 ? 20 : 10);
		
		FRectF xrc(rc_);
		xrc.bottom = xrc.top + cy;
		
		(*cxt)->DrawRectangle( xrc, cxt.black_ );
		(*cxt)->FillRectangle( xrc, cxt.white_ );

		D2DMatrix mat(*cxt);

		mat.PushTransform();
		mat_ = mat.Offset(rc_);

		FRectF rc(0,0, rc_.Width(), 20); 

		int idx = 0;
		itemrc_.resize(items_.size());

		for(auto& it : items_ )
		{
			if ( it.typ == 0 )
			{
				if ( idx == floating_idx_ )
					cxt.DFillRect(rc, D2RGBA(0,200,0,100));


				(*cxt)->DrawText(it.str, wcslen(it.str), cxt.textformat_, rc, cxt.black_ );
				itemrc_[idx] = rc;

				idx++;
				rc.Offset(0,20);

				
			}
			else if ( it.typ == 1 )
			{
				float yline = (rc.top+rc.bottom)/2;
				FRectF rc1(rc.left,yline,rc.Width(),yline+1);
				(*cxt)->FillRectangle(rc1, cxt.black_ );
				rc.Offset(0,10);
			}
		}
		mat.PopTransform();
	}

}
LRESULT InnerFloatingMenu::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch( message )
	{
		case WM_D2D_CREATE :
		{
			return 1;
		}
		break;
		case WM_KEYDOWN:
		{
			auto key = 0xff & wParam;

			if ( key == VK_ESCAPE && APP.IsCapture(this) )
			{
				APP.ReleaseCapture();

				DestroyControl();
				return 1;
			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			if ( APP.IsCapture(this) )
			{
				MouseParam mp = *(MouseParam*)lParam;

				auto pt = mat_.DPtoLP(mp.pt);
				int idx = 0;
				for(auto& rc1 : itemrc_ )
				{
					if (rc1.PtInRect(pt))
					{
						
						floating_idx_ = idx;
						
						break;
					}

					idx++;
				}

				b.bRedraw = true;

				return 1;
			}

		}
		break;
		case WM_LBUTTONDOWN:
		{
			if ( APP.IsCapture(this) )
			{
				
				MouseParam mp = *(MouseParam*)lParam;

				auto pt = mat_.DPtoLP(mp.pt);
				int idx = 0, floating_idx = -1;
				for(auto& rc1 : itemrc_ )
				{
					if (rc1.PtInRect(pt))
					{						
						floating_idx = idx;						
						break;
					}
					idx++;
				}
				APP.ReleaseCapture();
				


				if ( floating_idx == floating_idx_ && floating_idx > -1)
				{
					D2DNMHDR d;
					d.code = EVENTID_CLICK;
					d.sender.p = this;
					d.sender.typ = TYP_BUTTON;
					d.prm1 = items_[floating_idx_].id;
					d.sender_parent = sender_;

					auto ret = GetParentControls()->SendMesage(WM_NOTIFY, id_, (LPARAM)&d );

					if ( ret == 0 )
						parent_window_->SendMessage( WM_NOTIFY, id_, (LPARAM)&d );

				}
				DestroyControl();

				return 1;
			}

		}
		break;



	}
	return ( APP.IsCapture(this) ? 1 : 0 );
}

int D2DWindow::FloatingMenu(LPVOID sender, const FRectF& rc, std::vector<MenuItem>& items)
{
	auto FloatingMenu = std::make_shared<InnerFloatingMenu>();
	
	FloatingMenu->CreateControl(this, top_control_.get(), rc, STAT_DEFAULT, L"floatingmenu", ID_FLOATING_MENU );

	top_control_->Add( FloatingMenu );

	FloatingMenu->ModalShow(sender,items);

	return 0;
}


std::vector<std::wstring> SplitW( LPCWSTR str, LPCWSTR split )
{
    std::vector<std::wstring> ar;
    int splen = lstrlenW(split);
    int len = lstrlenW(str);
    _ASSERT( 0 < splen && splen <= 2  );

    int si = 0;
    for( int i = 0; i <= len; i++ )
    {
        if ( str[i] == split[0] || (i == len && 0 < len) )
        {
            if (splen == 1 || (splen == 2 && (i == len || str[i+1] == split[1] )) )
            {
                std::wstring s( &str[si], i-si );
                ar.push_back( std::wstring(s.c_str()));
                si = i + splen;
            }
        }       
    }
    return ar;
}