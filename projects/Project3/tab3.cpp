#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"

using namespace V6;

#include "CToolboxControls.h"


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
	UIHandleWin hwin;
	ID2D1Bitmap* bmp1;

	WhiteBoard wbbox;
	FRectF rcwbbox;
	UIHandle wbboxhandle;
	CToolboxControls* toolbox;
};
void CreateControl2test(UIHandleWin hwin, UIHandle hcs)
{

}

void Initialwbbox( WhiteBoard& wb );
void CreateControl2(UIHandleWin hwin, UIHandle hcs )
{


    static CaptureObj1 obj;
    obj.active_idx = 0;
    obj.wboard.typ = 0;
	obj.hwin = hwin;
	obj.bmp1 = nullptr;
    
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


		if ( obj->bmp1)
		{
			FRectF rcbmp(200,0,FSizeF(20,14));
			FRectF rcS(1000,1000,FSizeF(20,14));

			static int gk = 0;
			static FRectF* gp = nullptr;
			if ( gk == 0 && gp == nullptr )
			{
				 gp = new FRectF[100];
				RectAnimation(rcS, rcbmp, gp, 100, 0);
				gk++;
			}

			if ( gk == 100)
			{
				(*cxt)->DrawBitmap(obj->bmp1, rcbmp );
				delete [] gp;
				gp = nullptr;
			}
			else if ( gp )
			{
				(*cxt)->DrawBitmap(obj->bmp1, gp[gk-1] );
				gk++;

				cxt.Redraw();
			}

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

				D2DSetText( b3, L"Šg’£‹@”\");



				obj->wbboxhandle = D2DCreateWhiteControls(obj, obj->wbbox.drawFunc, obj->wbbox.procFunc, win, ctrl, rc, STAT_VISIBLE | STAT_ENABLE, L"xbox", 215);



				return 1;
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
					obj->toolbox->SwShow();

				}
				else if ( wParam == 211 )
				{
					void work(IStream** ppJpg);

					ComPTR<IStream> p;
					work(&p);

					if ( p )
					{
						void Stream2Bitmap( IStream* sm, ID2D1RenderTarget* target, ID2D1Bitmap** bmp);

						

						auto cxt = D2DGetDeviceContext(obj->hwin);

						Stream2Bitmap( p, cxt, &obj->bmp1);

					}


				}



			}
			break;
		}
    
        return r;
    };
    

	Initialwbbox( obj.wbbox );

    
    FRectF rc(0, 50, FSizeF(200, 200));

    obj.rc = rc;
    auto hcs2 = D2DCreateWhiteControls(&obj, obj.wboard.drawFunc, obj.wboard.procFunc, hwin, hcs, rc, STAT_VISIBLE | STAT_ENABLE, L"whb21", 210);


	static CToolboxControls toolbox;
	toolbox.CreateControl(hwin, hcs, CToolboxControls::TYP::RIGHT_SIDE, L"test");
	obj.toolbox = &toolbox;
}

void Initialwbbox( WhiteBoard& wb )
{
	
	wb.drawFunc = [](LPVOID captureobj, D2DContext& cxt) {
		
		CaptureObj1* obj = (CaptureObj1*)captureobj;

		cxt.DDrawRect(obj->rcwbbox, D2RGB(255,0,0),D2RGB(255,0,0) );

		std::wstring s = L"movable rect by mouse";
		FRectF rc1 = obj->rcwbbox;

		(*cxt)->DrawText( s.c_str(), s.length(), cxt.textformat_, rc1, cxt.white_ );
			
	};


	wb.procFunc = [](LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)->HRESULT {

		CaptureObj1* obj = (CaptureObj1*)captureobj;
		HRESULT r = 0;
		FRectF& rcbox = obj->rcwbbox;

		switch( message )
		{
			case WM_D2D_CREATE:
			{
				rcbox.SetRect(200,200, FSizeF(100,100));
				r = 1;
			}
			break;
			case WM_LBUTTONDOWN:
			{
				MouseParam* mp = (MouseParam*)lParam;
				auto pt = obj->mat.DPtoLP(mp->pt);

				if ( rcbox.PtInRect(pt))
				{
					auto h = obj->wbboxhandle;

					D2DSetCapture(h);
					int a = 0;
					

					r = 1;
				}
			}
			break;
			case WM_MOUSEMOVE:
			{
				auto h1 = D2DGetCapture();

				if (h1.p == obj->wbboxhandle.p )
				{
					MouseParam* mp = (MouseParam*)lParam;
					auto pt = obj->mat.DPtoLP(mp->pt);
					auto ptprv = obj->mat.DPtoLP(mp->ptprv);

					rcbox.Offset( pt.x - ptprv.x, pt.y - ptprv.y );
					b.bRedraw = true;
					r = 1;
				}

			}
			break;
			case WM_LBUTTONUP:
			{
				auto h1 = D2DGetCapture();

				if (h1.p == obj->wbboxhandle.p )
				{
					D2DReleaseCapture();
					r = 1;
				}

			}
			break;



		}

		return r;
	};

}