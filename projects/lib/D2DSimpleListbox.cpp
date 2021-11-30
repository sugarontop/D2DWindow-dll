#include "pch.h"
#include "D2DSimpleListbox.h"
#include "D2DDropdownListbox.h"

using namespace V6;
#define  APP (D2DApp::GetInstance())

#define BUTTON_WIDTH BARW
#define ROW_HEIGHT 22.0f

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
    if (stat_ & STAT_VISIBLE)
    {
        D2DMatrix mat(*cxt);
        mat.PushTransform();
        (*cxt)->DrawRectangle(rc_, cxt.black_);

        D2DRectFilter fil(cxt, rc_ );

        cxt.DFillRect(rc_, D2RGB(240, 240, 240));


        //if (mouse_stat_ == 1)
        //    cxt.DFillRect(rc_, D2RGB(220,220,200));
        //else 
        //    //(*cxt)->FillRectangle(rc_, cxt.white_ ); // 

        mat_ = mat.Offset(rc_);
        
        int i = 0;

		const float h = RowHeight();
        mat.PushTransform();
        mat.Offset(0, -offbar_y_* scbai_);
        for(auto& it : items_ )
        {
            float w;
            w = rc_.Width();
            

            
            if (selected_idx_ == i )
            {
                cxt.DFillRect(FRectF(0, 0, w, h), D2RGBA(0, 200, 200, 150));
            }
            else if (float_idx_ == i)
            {
                cxt.DFillRect(FRectF(0, 0, w, h), D2RGBA(0, 200, 200, 80));
            }

            it->Draw(cxt, w, h );

            mat.Offset(0, h);
            i++;
        }
        mat.PopTransform();

        mat.PushTransform();
            mat.Offset(0.0f, offbar_y_);
            float overflow = max(0, sc_dataHeight() - sc_barTotalHeight());
            scbarThumbHeight_ = sc_barTotalHeight() - overflow;
            const float min_thum_height = 8.0f;
            scbai_ = 1.0f;
            if (scbarThumbHeight_ < min_thum_height)
            {
                scbarThumbHeight_ = min_thum_height;
                scbai_ = (sc_dataHeight() - sc_barTotalHeight()) / (sc_barTotalHeight() - scbarThumbHeight_); //());			
            }

            FRectF scbar(rc_.Size().width - BARW, 0, rc_.Size().width, scbarThumbHeight_);
            cxt.DFillRect(scbar, D2GRAY);
        mat.PopTransform();

        mat.PopTransform();
    }

}

void D2DSimpleListbox::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id) 
{
    D2DControl::CreateControl(parent, pacontrol, rc, stat, name, local_id);

    SetRect(rc);

    selected_idx_ = -1;
    float_idx_ = -1;
    offbar_y_ = 0;
    scbarThumbHeight_ = scdata_ = 0;
    scbai_=1.0f;

//#ifdef  _DEBUG
//    items_.push_back( std::shared_ptr<D2DListboxItemString>( new D2DListboxItemString(1, L"item1")));
//    items_.push_back( std::shared_ptr<D2DListboxItemString>( new D2DListboxItemString(2, L"item2")));
//    items_.push_back(std::shared_ptr<D2DListboxItemString>(new D2DListboxItemString(3, L"item3")));
//    items_.push_back(std::shared_ptr<D2DListboxItemString>(new D2DListboxItemString(4, L"item4")));
//    items_.push_back(std::shared_ptr<D2DListboxItemString>(new D2DListboxItemString(5, L"item5")));
//    items_.push_back(std::shared_ptr<D2DListboxItemString>(new D2DListboxItemString(6, L"item6")));
//    items_.push_back(std::shared_ptr<D2DListboxItemString>(new D2DListboxItemString(7, L"item7")));
//    items_.push_back(std::shared_ptr<D2DListboxItemString>(new D2DListboxItemString(8, L"item8")));
//#endif //  _DEBUG

    auto ls =  dynamic_cast<D2DDropdownListbox*>(pacontrol);
    if ( ls )
    {
        APP.SetCapture(this);

    }


}

static FPointF ptold;
bool D2DSimpleListbox::sc_MouseMove(FPointF& pt)
{
    offbar_y_ = max(0, offbar_y_ + (pt.y - ptold.y));

    offbar_y_ = min(sc_barTotalHeight() - sc_barThumbHeight(), offbar_y_);

    ptold = pt;

    return false;
}



HRESULT D2DSimpleListbox::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
    HRESULT ret = 0;

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
          
            if (rc_.ZeroRect().PtInRect(pt))
            {
                ptold = pt;

                if ( rc_.ZeroRect().right - 30 < pt.x && pt.x < rc_.ZeroRect().right)
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


                return 1;
            }
            
            
            if (!rc_.ZeroRect().PtInRect(pt))
            {
                if ( OnEscape() )
                    ret = 1;
            }
        }
        break;
        case WM_LBUTTONUP:
        {
            
            MouseParam* mp = (MouseParam*)lParam;
            auto pt = mat_.DPtoLP(mp->pt);

            if (InnerRect(rc_).ZeroRect().PtInRect(pt) && scstat_ != 3)
            {            
                OnClick();
            }

            scstat_ = 0;
            break;
        }
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

            

                if ( scstat_ == 3 && APP.IsCapture(this))
                {
                    float_idx_ = -1;
                    sc_MouseMove(pt);
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
			int typ = wParam;

			if ( typ == 1 )
			{			
				ID2D1Bitmap* cs = (ID2D1Bitmap*)lParam;

				ComPTR<ID2D1Bitmap> t(cs);
				
				auto i = items_.size();
				items_.push_back( std::shared_ptr<D2DListboxItemImage>(new D2DListboxItemImage(i, t))); 
	
			}
			ret = 1;
		}
		break;
    }

    return ret;
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
    return items_.size() * ROW_HEIGHT;
}

// /////////////////////////////////////////////////////////////////////////////////////
void D2DListboxItemString::Draw(D2DContext& cxt, float width, float height)
{
    (*cxt)->DrawText( title_.c_str(), title_.length(), cxt.textformat_, FRectF(0, 0, width, height) , cxt.black_ );
}
float D2DListboxItemString::RowHeight()
{
	return ROW_HEIGHT;
}

void D2DListboxItemImage::Draw(D2DContext& cxt, float width, float height)
{
   (*cxt)->DrawImage(img_, FPointF());
}
float D2DListboxItemImage::RowHeight()
{
	return img_->GetSize().height;
}