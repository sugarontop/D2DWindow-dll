#include "pch.h"
#include "D2DSimpleListbox.h"
#include "D2DDropdownListbox.h"
#include "D2DColor.h"
using namespace V6;
#define  APP (D2DApp::GetInstance())

#define BUTTON_WIDTH BARW
#define ROW_HEIGHT 22.0f
#define _min_thum_height 24.0f

D2DSimpleListbox::D2DSimpleListbox()
{
                
}
float D2DSimpleListbox::RowHeight() const
{
	if ( !items_.empty())
	{
		return items_[0]->RowHeight();
	}

	return ROW_HEIGHT;
}

void D2DSimpleListbox::Draw(D2DContext& cxt)
{
    if ( BITFLG(STAT_VISIBLE))
    {
        D2DMatrix mat(*cxt);
        mat.PushTransform();

		ComPTR<ID2D1SolidColorBrush> br,br2;
		border_clr_.Create(*cxt, &br);

        (*cxt)->DrawRectangle(rc_, br);

        D2DRectFilter fil(cxt, rc_ );

		back_clr_.Create(*cxt, &br2);
        (*cxt)->FillRectangle(rc_, br2);

        mat_ = mat.Offset(rc_);
        
        int j = 0;

		const float h = RowHeight();
        mat.PushTransform();
        mat.Offset(-offbar_x_, -offbar_y_* scbai_);
        for(auto& it : items_ )
        {
            float w = rc_.Width()+hscWidth_;

            // draw content
			if (it->Draw(cxt, w, h))
			{
				// scrollbar
				hscWidth_ = max(hscWidth_, it->ItemWidth());
			}
			br2 = nullptr;

			// draw select or floating area
			if (selected_idx_ == j )
            {
				select_clr_.Create(*cxt, &br2);
				(*cxt)->FillRectangle(FRectF(w, h), br2);

                //cxt.DFillRect(FRectF(w, h), D2RGBA(0, 200, 200, 150)); select_clr_ = D2RGBA(0, 200, 200, 150)
            }
            else if (float_idx_ == j)
            {
				float_clr_.Create(*cxt, &br2);
				(*cxt)->FillRectangle(FRectF(w, h), br2);

                //cxt.DFillRect(FRectF(w, h), D2RGBA(0, 200, 200, 80)); // float_clr_ = D2RGBA(0, 200, 200, 80)
            }

            mat.Offset(0, h);
            j++;
        }
        mat.PopTransform();

        // V Scrollbar
		if ( sc_barTotalHeight() < sc_dataHeight())
		{
			mat.PushTransform();
			{
				mat.Offset(0.0f, offbar_y_);
				float overflow = max(0, sc_dataHeight() - sc_barTotalHeight());
				scbarThumbHeight_ = sc_barTotalHeight() - overflow;
				const float min_thum_height = _min_thum_height;
				scbai_ = 1.0f;
				if (scbarThumbHeight_ < min_thum_height)
				{
					scbarThumbHeight_ = min_thum_height;
					scbai_ = (sc_dataHeight() - sc_barTotalHeight()) / (sc_barTotalHeight() - scbarThumbHeight_); 
				}

				FRectF scbar(rc_.Size().width - BARW, 0, rc_.Size().width, scbarThumbHeight_);
				cxt.DFillRect(scbar, D2GRAY);
			}
			mat.PopTransform();
		}

		// W Scrollbar
		if ( hscWidth_ )
		{
			mat.PushTransform();		
			mat.Offset(offbar_x_, rc_.Height());

			{
				float wsc_barTotalHeight = rc_.Size().width;
				float overflow = max(0, (hscWidth_) - wsc_barTotalHeight);
				float wscbarThumbHeight = wsc_barTotalHeight - overflow;
				const float min_thum_height = _min_thum_height;
				float wscbai = 1.0f;
				if (wscbarThumbHeight < min_thum_height)
				{
					wscbarThumbHeight = min_thum_height;
					wscbai = (hscWidth_ - wsc_barTotalHeight) / (wsc_barTotalHeight - wscbarThumbHeight); 	
				}

				FRectF scbar(0,-BARW, wscbarThumbHeight, 0); 
				cxt.DFillRect(scbar, D2GRAY);
			}
		
			mat.PopTransform();
		}

        mat.PopTransform();
    }

}

void D2DSimpleListbox::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id) 
{
    D2DControl::CreateControl(parent, pacontrol, rc, stat, name, local_id);

    SetRect(rc);

    selected_idx_ = -1;
    float_idx_ = -1;
    offbar_y_ = offbar_x_ = 0;
    scbarThumbHeight_ = scdata_ = 0;
    scbai_=1.0f;
	typ_ = 0;
	hscWidth_ = 0;

	back_clr_ = D2RGB(240,240,240);
	float_clr_ = D2RGBA(0,200,200,80);
	select_clr_ = D2RGBA(0,200,200,150);

    auto ls =  dynamic_cast<D2DDropdownListbox*>(pacontrol);
    if ( ls )
    {
        APP.SetCapture(this);
    }
}

void D2DSimpleListbox::Clear()
{
	items_.clear();

	selected_idx_ = -1;
    float_idx_ = -1;
    offbar_y_ = offbar_x_ = 0;
    scbarThumbHeight_ = scdata_ = 0;
    scbai_=1.0f;
	typ_ = 0;
	hscWidth_ = 0;

}

static FPointF ptold;
bool D2DSimpleListbox::sc_MouseMove(FPointF& pt)
{
    offbar_y_ = max(0, offbar_y_ + (pt.y - ptold.y));

    offbar_y_ = min(sc_barTotalHeight()-sc_barThumbHeight()+BARW, offbar_y_);

    ptold = pt;

    return false;
}
bool D2DSimpleListbox::wsc_MouseMove(FPointF& pt)
{
    offbar_x_ = max(0, offbar_x_ + (pt.x - ptold.x));
	float wsc_barTotalHeight = rc_.Size().width;
	float overflow = max(0, hscWidth_ - wsc_barTotalHeight);
	float wscbarThumbHeight = wsc_barTotalHeight - overflow;

    offbar_x_ = min(wsc_barTotalHeight-wscbarThumbHeight+BARW, offbar_x_);

    ptold = pt;

    return false;
}

LRESULT D2DSimpleListbox::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (typ_ == 0)
		return WndProcNormal(b,message,wParam,lParam);
	

	return WndProcForControl(b,message,wParam,lParam);

}

LRESULT D2DSimpleListbox::WndProcForControl(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT ret = 0;

    auto InnerRect =[](FRectF rc)->FRectF
    {
        rc.right -= 30;
        return rc;
    };

	const auto rowheight = RowHeight();

    switch( message )
    {
        case WM_LBUTTONDOWN:
        {
            MouseParam* mp = (MouseParam*)lParam;
            auto pt = mat_.DPtoLP(mp->pt);
			FRectF rcz = rc_.ZeroRect();		
			
            if (rcz.PtInRect(pt))
            {
                ptold = pt;

                if ( rcz.right - BARW < pt.x && pt.x < rcz.right)
                {
                    APP.SetCapture(this);
                    scstat_ = 3;
                }
				
                else
                    mouse_stat_ = 1;


                auto y = pt.y + offbar_y_* scbai_;
                int idx = (int)(y / rowheight);

                if (InnerRect(rc_).ZeroRect().PtInRect(pt) )
                    selected_idx_ = idx;

				

				if (-1< selected_idx_ && selected_idx_ < (int)items_.size())
				{
					APP.ReleaseCapture();
					auto a = items_[selected_idx_];
					auto a1 = dynamic_cast<D2DListboxItemControl*>(a.get());
					auto a2 = a1->Control();

					APP.SetCapture(a2.get());
				}
                ret = 1;
            }
 
        }
        break;
        case WM_LBUTTONUP:
        {            
            scstat_ = 0;

			if ( APP.IsCapture(this))
			{
				APP.ReleaseCapture();
				ret = 1;
			}

            break;
        }
		break;
        case WM_MOUSEMOVE:
        {
            MouseParam* mp = (MouseParam*)lParam;
            auto pt = mat_.DPtoLP(mp->pt);
            auto md = (rc_.ZeroRect().PtInRect(pt) ? 1: 0 );

            if (mouse_stat_ != md )
            {
                mouse_stat_ = md;
                b.bRedraw = true;

            }

            if ( md == 1 )
            {
                auto y = pt.y + offbar_y_ * scbai_;
                int idx = (int)(y / rowheight);

                if (rc_.ZeroRect().PtInRect(pt))
                {
                    b.bRedraw = true;
                    if ( idx != float_idx_  && -1 < idx  && InnerRect(rc_).ZeroRect().PtInRect(pt))
                    {
                        float_idx_ = idx;
                    }
                }

				if ( APP.IsCapture(this))
				{
					if ( scstat_ == 3)
					{
						float_idx_ = -1;
						sc_MouseMove(pt);
					}
					else if ( scstat_ == 4)
					{						
						wsc_MouseMove(pt);
					}
				}

                ret = 1;
            }
            else
                float_idx_ = -1;
        }
        break;
        case WM_MOUSEWHEEL :
        {
            MouseParam* mp = (MouseParam*)lParam;
            auto pt = mat_.DPtoLP(mp->pt);
            auto md = (rc_.ZeroRect().PtInRect(pt) ? 1 : 0);

            if ( md == 1 )
            {
                float a = 0;
                if (mp->zDelta > 0)
                    a = -rowheight;
                if (mp->zDelta < 0)
                     a= rowheight;

                offbar_y_ = max(0,min(sc_dataHeight(), offbar_y_+a ));


                if ( offbar_y_ + sc_barThumbHeight() > rc_.Height())
                    offbar_y_ = rc_.Height()- sc_barThumbHeight();

                ret = 1;
            }



        }
        break;
        case WM_KEYDOWN:
        {
			auto key = 0xff & wParam;
            if (key == VK_ESCAPE ) 
            {
                if ( OnEscape() )
                    ret = 1;
            }

        }
        break;
		case WM_D2D_LISTBOX_ADD_ITEM:
		{
			int typ = (int)wParam;

			if ( typ == 1 )
			{			
				ID2D1Bitmap* cs = (ID2D1Bitmap*)lParam;
				typ_ = 0;

				ComPTR<ID2D1Bitmap> t(cs);
				
				auto i = items_.size();
				items_.push_back( std::make_shared<D2DListboxItemImage>(i, t)); 
	
			}
			else if ( typ == 2 )
			{
				D2DControls* cs = (D2DControls*)lParam;
				typ_ = 1;

				for(UINT i = 0; i < cs->ChildCount(); i++ ) 
				{
					auto ch = cs->GetItem(i);	
					items_.push_back( std::make_shared<D2DListboxItemControl>(i, ch)); 
				}
			}
			ret = 1;
		}
		break;
		case WM_D2D_COMMAND_SET:
		{
			if ( (UINT_PTR)this == (UINT_PTR)wParam)
			{
				LPCWSTR cmdstr = (LPCWSTR)lParam;

				auto ar = SplitW(cmdstr,L"&");
				std::wstring cmd;
				

				for(auto& it : ar)
				{
					auto ar2 = SplitW(it.c_str(), L"=");

					if ( ar2.size() == 1 )
					{
						cmd = ar2[0];

					}
					else if ( ar2.size() == 2)
					{
						if ( cmd==L"add" && (ar2[0] == L"str" || ar2[0] == L"text") )
						{
							AddItem( ar2[1]);
						}	
						else if ( ar2[0] == L"no" && cmd==L"select")
						{
							int idx = _wtoi(ar2[1].c_str());
							
							selected_idx_ = idx;

							//OnClick();
							//OnCloseListbox(idx);


						}
					}

				}
				ret = 1;
			}
		}
		break;
		case WM_D2D_SET_COLOR:
		{
			switch(wParam)
			{
				case COLOR_IDX_BORDER:
					border_clr_ = *(D2DColor*)lParam;
				break;
				case COLOR_IDX_BACK:
					back_clr_ = *(D2DColor*)lParam;
				break;
				case COLOR_IDX_FLOAT:
					float_clr_ = *(D2DColor*)lParam;
				break;
				case COLOR_IDX_SELECT:
					select_clr_ = *(D2DColor*)lParam;
				break;
			}
			ret = 1;
		}
		break;
    }

    return ret;
}


LRESULT D2DSimpleListbox::WndProcNormal(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT ret = 0;

    auto InnerRect =[](FRectF rc)->FRectF
    {
        rc.right -= 30;
        return rc;
    };

	const auto rowheight = RowHeight();

    switch( message )
    {
        case WM_LBUTTONDOWN:
        {
            MouseParam* mp = (MouseParam*)lParam;
            auto pt = mat_.DPtoLP(mp->pt);
			const auto rcz = rc_.ZeroRect();
            if (rcz.PtInRect(pt))
            {
                ptold = pt;

                if ( rcz.right - BARW < pt.x && pt.x < rcz.right)
                {
                    APP.SetCapture(this);
                    scstat_ = 3;

                }
				else if ( rcz.bottom - BARW < pt.y && pt.y < rcz.bottom)
                {
                    APP.SetCapture(this);
                    scstat_ = 4;
                }
                else
                    mouse_stat_ = 1;


                auto y = pt.y + offbar_y_* scbai_;
                int idx = (int)(y / rowheight);

                if (InnerRect(rc_).ZeroRect().PtInRect(pt) )
                    selected_idx_ = idx;


                return 1;
            }
            
            
            if (!rcz.PtInRect(pt))
            {
                if ( OnEscape() )
                    ret = 0;
            }
        }
        break;
        case WM_LBUTTONUP:
        {
            MouseParam* mp = (MouseParam*)lParam;
            auto pt = mat_.DPtoLP(mp->pt);

            if (InnerRect(rc_).ZeroRect().PtInRect(pt) && scstat_ != 3 && scstat_ != 4)
            {            
                OnClick(); // ‚±‚±‚ÅReleaseCapture‚·‚é
				ret = 1;
            }

            scstat_ = 0;            
        }
		break;
        case WM_MOUSEMOVE:
        {
            MouseParam* mp = (MouseParam*)lParam;
            auto pt = mat_.DPtoLP(mp->pt);

            auto md = (rc_.ZeroRect().PtInRect(pt) ? 1: 0 );

            if (mouse_stat_ != md )
            {
                mouse_stat_ = md;
                b.bRedraw = true;
            }

            if ( md == 1 )
            {

                auto y = pt.y + offbar_y_ * scbai_;
                int idx = (int)(y / rowheight);


                if (rc_.ZeroRect().PtInRect(pt))
                {
                    b.bRedraw = true;
                    if ( idx != float_idx_  && -1 < idx  && InnerRect(rc_).ZeroRect().PtInRect(pt))
                    {
                        float_idx_ = idx;
                    }
                }

            
				if ( APP.IsCapture(this))
				{
					if ( scstat_ == 3)
					{
						float_idx_ = -1;
						sc_MouseMove(pt);
					}
					else if ( scstat_ == 4)
					{
						wsc_MouseMove(pt);
					}
				}

                ret = 1;
            }
            else
                float_idx_ = -1;
        }
        break;
        case WM_MOUSEWHEEL :
        {
            MouseParam* mp = (MouseParam*)lParam;
            auto pt = mat_.DPtoLP(mp->pt);
            auto md = (rc_.ZeroRect().PtInRect(pt) ? 1 : 0);

            if ( md == 1 )
            {
                float a = 0;
                if (mp->zDelta > 0)
                    a = -rowheight;
                if (mp->zDelta < 0)
                     a= rowheight;

                offbar_y_ = max(0,min(sc_dataHeight(), offbar_y_+a ));


                if ( offbar_y_ + sc_barThumbHeight() > rc_.Height())
                    offbar_y_ = rc_.Height()- sc_barThumbHeight();

                ret = 1;
            }
        }
        break;
        case WM_KEYDOWN:
        {
			auto key = 0xff & wParam;
            if (key == VK_ESCAPE ) 
            {
                if ( OnEscape() )
                    ret = 1;
            }
        }
        break;
		case WM_D2D_LISTBOX_ADD_ITEM:
		{
			int typ = (int)wParam;

			if ( typ == 1 )
			{			
				ID2D1Bitmap* cs = (ID2D1Bitmap*)lParam;
				typ_ = 0;

				ComPTR<ID2D1Bitmap> t(cs);
				
				auto i = items_.size();
				items_.push_back( std::make_shared<D2DListboxItemImage>(i, t)); 
	
			}
			else if ( typ == 2 )
			{
				D2DControls* cs = (D2DControls*)lParam;
				typ_ = 1;

				for(UINT i = 0; i < cs->ChildCount(); i++ ) 
				{
					auto ch = cs->GetItem(i);	
					items_.push_back( std::make_shared<D2DListboxItemControl>(i, ch)); 
				}
			}
			ret = 1;
		}
		break;

		case WM_D2D_SELECTED_ITEM:
		case WM_D2D_CB_GETSELECT:
		{			
			int* r = (int*)lParam;
			
			*r = selected_idx_;

			ret=1;

		}
		break;
		case WM_D2D_COMMAND_SET:
		{
			if ( (UINT_PTR)this == (UINT_PTR)wParam)
			{
				LPCWSTR cmdstr = (LPCWSTR)lParam;

				auto ar = SplitW(cmdstr,L"&");
				std::wstring cmd;
				

				for(auto& it : ar)
				{
					auto ar2 = SplitW(it.c_str(), L"=");

					if ( ar2.size() == 1 )
					{
						cmd = ar2[0];

					}
					else if ( ar2.size() == 2)
					{
						if ( ar2[0] == L"str" && cmd==L"add")
						{
							AddItem( ar2[1]);
						}	
						else if ( ar2[0] == L"no" && cmd==L"select")
						{
							int idx = _wtoi(ar2[1].c_str());
							selected_idx_ = idx;
							OnClick();
							


						}
					}

				}
				ret = 1;
			}
		}
		break;
		case WM_D2D_SET_COLOR:
		{
			switch(wParam)
			{
				case COLOR_IDX_BORDER:
					border_clr_ = *(D2DColor*)lParam;
				break;
				case COLOR_IDX_BACK:
					back_clr_ = *(D2DColor*)lParam;
				break;
				case COLOR_IDX_FLOAT:
					float_clr_ = *(D2DColor*)lParam;
				break;
				case COLOR_IDX_SELECT:
					select_clr_ = *(D2DColor*)lParam;
				break;
			}
			ret = 1;
		}
		break;
    }

    return ret;
}
void D2DSimpleListbox::AddItem(const std::wstring& str)
{
	int idx = (int)items_.size();
	items_.push_back( std::make_shared<D2DListboxItemString>(idx, str)); 
}
void D2DSimpleListbox::AddBitmapItem(ID2D1Bitmap* bmp)
{
	int idx = (int)items_.size();
	items_.push_back( std::make_shared<D2DListboxItemImage>(idx, bmp)); 
}


bool D2DSimpleListbox::OnEscape()
{
    
    //if ( APP.IsCapture(this))
    {                
        selected_idx_ = -1;
		float_idx_ = -1;

        OnClick();
        return true;
    }
    return false;
}
void D2DSimpleListbox::OnClick()
{
    if (APP.IsCapture(this))
    {
        APP.ReleaseCapture();

        auto ls = dynamic_cast<D2DDropdownListbox*>(parent_control_);

        if (ls)
        {
            stat_ &= ~STAT_VISIBLE;
            ls->OnCloseListbox(selected_idx_);
        }

    }

}
float D2DSimpleListbox::sc_barThumbHeight()
{
    return scbarThumbHeight_;
}
float D2DSimpleListbox::sc_barTotalHeight()
{
    return rc_.Height();
}
float D2DSimpleListbox::sc_dataHeight()
{
    return items_.size() * RowHeight();
}
std::wstring D2DSimpleListbox::GetTreeTyp(USHORT* typ)
{ 
	*typ=2; 
	return L"D2DSimpleListbox";
}

// /////////////////////////////////////////////////////////////////////////////////////
bool D2DListboxItemString::Draw(D2DContext& cxt, float width, float height)
{	
	bool bl = false;
	if ( layout_ == nullptr )
	{
		if (SOK(cxt.wfactory_->CreateTextLayout(title_.c_str(), title_.length(), cxt.textformat_, 1000,1000, &layout_ )))
		{
			DWRITE_TEXT_METRICS t;
			layout_->GetMetrics(&t);

			width_ = t.width;

			bl = (width < width_);
		}
	}

	(*cxt)->DrawTextLayout(FPointF(), layout_, cxt.black_, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_CLIP);
	return bl;
}
float D2DListboxItemString::RowHeight()
{
	return ROW_HEIGHT;
}
void D2DListboxItemString::Clear()
{
	layout_ = nullptr;
}
// /////////////////////////////////////////////////////////////////////////////////////
bool D2DListboxItemImage::Draw(D2DContext& cxt, float width, float height)
{
   (*cxt)->DrawImage(img_, FPointF());
   return false;
}
float D2DListboxItemImage::RowHeight()
{
	return img_->GetSize().height;
}
// /////////////////////////////////////////////////////////////////////////////////////
bool D2DListboxItemControl::Draw(D2DContext& cxt, float width, float height)
{
	ctrl_->Draw(cxt);
	return false;
}
float D2DListboxItemControl::RowHeight()
{
	auto h =  ctrl_->GetRect().Height();
	return h;
}

