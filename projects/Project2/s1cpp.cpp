#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"

using namespace V6;

void CreateControl2(UIHandleWin hwin, UIHandle hcs)
{

}
void CreateControl2test(UIHandleWin hwin, UIHandle hcs )
{
    struct WhiteBoard
    {
        DelegateDrawFunc f1;
        DelegateProcFunc f2;
        int typ;
        D2DMat mat;
    };
        
    struct CaptureObj1
    {
        FRectF rc;
        UIHandle page[3];
        int active_idx;
        D2DMat mat;
        WhiteBoard wboard;
    };

    static CaptureObj1 obj;
    obj.active_idx = 0;
    obj.wboard.typ = 0;
    
    obj.wboard.f1 = [](LPVOID captureobj, D2DContext& cxt) {

        CaptureObj1* obj = (CaptureObj1*)captureobj;
        WhiteBoard& wb = obj->wboard;

        D2DMatrix mat(*cxt);
        mat.PushTransform();

        auto rc = obj->rc;
        
        wb.mat = mat.Offset(rc);
        obj->mat = wb.mat;
        {
            cxt.DDrawRect(rc.ZeroRect(), D2RGB(170, 255, 255), D2RGB(170, 255, 255));

        }
        mat.PopTransform();
    };
    obj.wboard.f2 = [](LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)->HRESULT {

        CaptureObj1* obj = (CaptureObj1*)captureobj;
        HRESULT r = 0;
        switch (message)
        {
        case WM_LBUTTONDOWN:
        break;
        }

        return r;
    };
    
    
    FRectF rc(0, 50, FSizeF(200, 200));

    obj.rc = rc;
    auto whb2 = D2DCreateWhiteControls(&obj, obj.wboard.f1, obj.wboard.f2, hwin, hcs, rc, STAT_VISIBLE | STAT_ENABLE, L"whb21", 210);





}