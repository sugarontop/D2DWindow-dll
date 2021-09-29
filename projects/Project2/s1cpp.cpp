#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"

using namespace V6;

void CreateControl2test(UIHandleWin hwin, UIHandle hcs)
{

}
void CreateControl2(UIHandleWin hwin, UIHandle hcs )
{
    struct WhiteBoard
    {
        DelegateDrawFunc drawFunc;
        DelegateProcFunc procFunc;
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
    
    obj.wboard.drawFunc = [](LPVOID captureobj, D2DContext& cxt) {

        CaptureObj1* obj = (CaptureObj1*)captureobj;
        WhiteBoard& wb = obj->wboard;

        D2DMatrix mat(*cxt);
        mat.PushTransform();

        auto rc = obj->rc;
        
        wb.mat = mat.Offset(rc);
        obj->mat = wb.mat;
        {
            auto time = ::GetTickCount64() / 250;


            if ( time % 4 == 0 )
                cxt.DDrawRect(rc.ZeroRect(), D2RGB(170, 255, 255), D2RGB(170, 255, 255));
            else if ( time % 4 == 1 )
                cxt.DDrawRect(rc.ZeroRect(), D2RGB(170, 255, 170), D2RGB(170, 255, 170));
            else if ( time % 4 == 2 )
                cxt.DDrawRect(rc.ZeroRect(), D2RGB(255, 255, 170), D2RGB(255, 255, 170));
            else if ( time % 4 == 3 )
                cxt.DDrawRect(rc.ZeroRect(), D2RGB(255, 170, 170), D2RGB(255, 170, 170));

            cxt.bRedraw_ = true;
        }
        mat.PopTransform();
    };
    obj.wboard.procFunc = [](LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)->HRESULT {

        CaptureObj1* obj = (CaptureObj1*)captureobj;
        HRESULT r = 0;
        switch (message)
        {
            case WM_D2D_CREATE:
            {
				UIHandleWin&win = *(UIHandleWin*)wParam;
				UIHandle& ctrl = *(UIHandle*)lParam;
				
				FRectF rc(10, 300, FSizeF(200,30));
				D2DCreateButton(win, ctrl, rc, STAT_VISIBLE | STAT_ENABLE, L"whb21-b1", 211);

				rc.Offset(0,35);
				D2DCreateButton(win, ctrl, rc, STAT_VISIBLE | STAT_ENABLE, L"whb21-b2", 212);

				rc.Offset(0,35);
				auto b3 = D2DCreateButton(win, ctrl, rc, STAT_VISIBLE | STAT_ENABLE, L"whb21-b3", 213);

				D2DSetText( b3, L"this is button three");


				return 0;
			}
            break;
        
            case WM_LBUTTONDOWN:
			{

			}
            break;
			case WM_NOTIFY:
			{
				if ( wParam == 213 )
				{
					int a = 0;
				}
			



			}
			break;
		}
    
        return r;
    };
    
    
    FRectF rc(0, 50, FSizeF(200, 200));

    obj.rc = rc;
    auto whb2 = D2DCreateWhiteControls(&obj, obj.wboard.drawFunc, obj.wboard.procFunc, hwin, hcs, rc, STAT_VISIBLE | STAT_ENABLE, L"whb21", 210);





}