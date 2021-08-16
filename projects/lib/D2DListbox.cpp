#include "pch.h"
#include "D2DListbox.h"

using namespace V6;
#define  APP (D2DApp::GetInstance())

D2DListbox::D2DListbox()
{

    

}

void D2DListbox::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id )
{
    D2DControl::CreateControl(parent, pacontrol,rc,stat,name,local_id);

    SetRect(rc);
    gtree.Create(rc.ZeroRect());

    gtree.test();
}
void D2DListbox::Draw(D2DContext& cxt)
{
    if (stat_ & STAT_VISIBLE)
    {
        //(*cxt)->GetTransform(&mat_);
        (*cxt)->FillRectangle(rc_, cxt.white_);
    
        D2DRectFilter fil(cxt, rc_, 0);
        D2DMatrix mat(*cxt);
        mat.PushTransform();
        mat_ = mat.Offset(rc_);

        //mat_ = mat.Copy();

        gtree.Draw(cxt);

        mat.PopTransform();
    }


}

HRESULT D2DListbox::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lp)
{
	HRESULT ret = 0;
    
	switch (message)
	{
        case WM_LBUTTONDOWN:
        {            
            MouseParam* mp = (MouseParam*)lp;            
            auto pt = mp->pt;
            if (gtree.LButtonDown(pt))
            {
                APP.SetCapture(this);
                ret = 1;
            }
        }
        break;
        case WM_MOUSEMOVE:
        {
            if (APP.IsCapture(this))
            {
                MouseParam* mp = (MouseParam*)lp;
                //auto pt = mat_.DPtoLP(mp->pt);
                auto pt = mp->pt;
                gtree.MouseMove(pt);
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
                    gtree.LButtonUp(pt);
                    APP.ReleaseCapture(); 
                    ret = 1;
                }
            }
        }
        break;
    }

    return ret;
}