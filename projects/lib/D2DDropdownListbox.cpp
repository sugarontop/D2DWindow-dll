#include "pch.h"
#include "D2DDropdownListbox.h"


using namespace V6;
#define  APP (D2DApp::GetInstance())

#define BUTTON_WIDTH 13.0f

D2DDropdownListbox::D2DDropdownListbox()
{
    
}
void D2DDropdownListbox::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
    D2DControl::CreateControl(parent, pacontrol, rc, stat, name, local_id);

    SetRect(rc);

    button_rc_ = rc;
    button_rc_.left = button_rc_.right - BUTTON_WIDTH;
    selected_idx_ = -1;
    listbox_height_ = rc.Height()*3;

    //gtree.Create(rc.ZeroRect());
    //gtree.test();
}
void D2DDropdownListbox::Draw(D2DContext& cxt)
{
    if (stat_ & STAT_VISIBLE)
    {
        (*cxt)->GetTransform(&mat_);

        if (mouse_mode_ ==0)
        {
            (*cxt)->DrawRectangle(rc_, cxt.black_);
            (*cxt)->FillRectangle(rc_, cxt.white_);

            (*cxt)->DrawRectangle(button_rc_, cxt.black_);
        }
        else if (mouse_mode_ == 1)
        {
            ComPTR<ID2D1SolidColorBrush> br;
            (*cxt)->CreateSolidColorBrush(D2RGB(0,0,255), &br);

            (*cxt)->DrawRectangle(rc_, br);
            (*cxt)->FillRectangle(rc_, cxt.white_);

            (*cxt)->DrawRectangle(button_rc_, br);
        }

        D2DMatrix mat(*cxt);
        mat.PushTransform();
        mat.Offset(rc_);

        if (selected_item_)
            selected_item_->Draw(cxt, rc_.Width(), rc_.Height());


        mat.Offset(0, rc_.Height());

        if ( ls_ )
            ls_->Draw(cxt);

        mat.PopTransform();
    }


}

HRESULT D2DDropdownListbox::WndProc(AppBase& b, UINT message, WPARAM wp, LPARAM lp)
{
    HRESULT ret = 0;

    switch (message)
    {
        case WM_LBUTTONDOWN:
        {
            MouseParam* mp = (MouseParam*)lp;
            auto pt = mat_.DPtoLP(mp->pt);
            if (rc_.PtInRect(pt))
            {
                APP.SetCapture(this);
                ret = 1;

                OnClick();
            }
        }
        break;
        case WM_MOUSEMOVE:
        {
            MouseParam* mp = (MouseParam*)lp;
            auto pt = mat_.DPtoLP(mp->pt);
            int pn = mouse_mode_;
            if ( rc_.PtInRect(pt))
                mouse_mode_ = 1;
            else
                mouse_mode_ = 0;

            if ( pn != mouse_mode_ )
                b.bRedraw = true;
            
            if (APP.IsCapture(this))
            {
                ret = 1;
                b.bRedraw = true;
                
            }
        }
        break;
        case WM_LBUTTONUP:
        {
            if (APP.IsCapture(this))
            {
                MouseParam* mp = (MouseParam*)lp;

                auto pt = mp->pt;
                {
                    APP.ReleaseCapture();
                    ret = 1;
                }
            }
        }
        break;
        case WM_D2D_CB_INSERTSTRING:
        {
            // index : WPARAM
            // string : LPARAM
            
            str_items_[(int)wp] = (LPCWSTR)lp;

        }
        break;
        
    }

    return ret;
}

void D2DDropdownListbox::OnClick()
{
    // 右隅のボタンをクリック、itemをリスト分作成する
	
	FRectF rc(0, 0,rc_.Width(), rc_.Height()+50);
    ls_ = std::shared_ptr<D2DSimpleListbox>( new D2DSimpleListbox());
    ls_->CreateControl( parent_window_, this, rc, STAT_VISIBLE|STAT_ENABLE, L"NONAME" );
    Add(ls_);

    for(auto& it : str_items_ )
        ls_->items_.push_back( std::shared_ptr<D2DListboxItemString>( new D2DListboxItemString( it.first, it.second)));

}

void D2DDropdownListbox::OnCloseListbox(int selected_idx)
{
	// リストの中から選択されて上部に表示
	
	_ASSERT( ls_ );	

    int old_item = selected_idx_;

    if (-1 < selected_idx  && selected_idx < (int)ls_->items_.size())
    {
        selected_item_ = ls_->items_[selected_idx];
        selected_idx_ = selected_idx;
    }

    ls_->DestroyControl();
    ls_ = nullptr;

    if ( selected_idx_ != old_item )
    {
        //this->parent_window_->SendMessage( WM_NOTIFY, (WPARAM)GetID(), (LPARAM)L"ONCHANGED" );

        D2DNMHDR nmh = {0};

        UIHandle u;
        u.p = this;
        u.typ = TYP_DROPDOWNLISTBOX;
        nmh.sender = u;
        nmh.code = 1001; // onchanged
        nmh.prm1 = selected_idx_;

        int a = GetID();


        parent_window_->SendMessage( WM_NOTIFY, (WPARAM)a, (LPARAM)&nmh );



    }
}

void D2DDropdownListbox::AddItem( int idx, std::wstring text)
{
    str_items_[idx] = text;
}

void D2DDropdownListbox::xAddItem(int idx, std::wstring text)
{
    str_items_[idx] = text;
}
int D2DDropdownListbox::xItemCount()
{
    return (int)str_items_.size();
}
int D2DDropdownListbox::xGetSelect()
{
    return selected_idx_;
}

bool D2DDropdownListbox::xSetSelect(int idx)
{    
    if ( str_items_.end() != str_items_.find(idx) )
    {
        selected_idx_ = idx;
        selected_item_ =  std::shared_ptr<D2DListboxItemString>( new D2DListboxItemString( idx, str_items_[idx]));
        return true;
    }
    return false;
}
void D2DDropdownListbox::xClear()
{
    str_items_.clear();
    selected_idx_ = -1;
    selected_item_ = nullptr;
}