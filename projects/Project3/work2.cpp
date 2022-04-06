
#include "pch.h"
#include "D2DContext.h"
#include "D2DMisc.h"
#include "D2D1UI_1.h"
#include "D2DColor.h"
#include "D2DApp.h"
#include "D2DMessage.h"

using namespace V6;

struct BobInstance
{
	BobInstance()
	{
		clr[0] = ColorF::AliceBlue;
		clr[1] = ColorF::AliceBlue;
		clr[2] = ColorF::AliceBlue;
		
		hme={};
		hprv={};
		hactive={};
		prc = nullptr;
		child = nullptr;
	};

	FRectF* prc;
	FRectF orgrc;
	D2DColor clr[3];
	D2DMat mat;
	UIHandle hme;
	UIHandle hprv;

	UIHandle hactive;
	D2DControls* child;
	FSizeF voff;

	UIHandle hp1;
	static bool df1(LPVOID captureobj, D2DContext& cxt);
	static LRESULT df2(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
};



UIHandleWin CreateControl(HWND hWnd)
{
	UIHandleWin hwin = D2DCreateMainHWnd(hWnd, 14,0);    
    UIHandle root = D2DGetRootControls(hwin);

	RECT rc;
	::GetClientRect(hWnd, &rc);
	
	auto bob = new BobInstance();
	D2DCreateWhiteControls((LPVOID)bob,BobInstance::df1,BobInstance::df2, root, FRectF(0,0,rc.right,rc.bottom),STAT_DEFAULT,L"bitcoin",1);

	return hwin;
}



bool BobInstance::df1(LPVOID captureobj, D2DContext& cxt)
{
	BobInstance* m = (BobInstance*)captureobj;
	D2DMatrix mat(*cxt);

	m->mat = mat.PushTransform();
	FRectF& rc = *(m->prc);
	D2DRectFilter fil(cxt, rc);	
	mat.Offset(rc);	
	cxt.DFillRect(rc.ZeroRect(), m->clr[0]);


	D2DInnerDraw(m->hme, (LPVOID)&cxt);


	mat.PopTransform();

	return false;
}


LPCWSTR _strformat( LPCWSTR expression, ... )
{
	static WCHAR cb[256];
	va_list args;
    va_start( args, expression );
	StringCchVPrintf(cb, 256, expression, args );
	va_end(args);
	return cb;
}


struct CustomBtn
{
	FRectF* prc;
	FRectF orgrc;
	D2DMat mat;
	UIHandle hme;
	std::wstring nm;
	D2DColor clr;

	std::function<void(std::wstring)> click_;


	static bool df1(LPVOID captureobj, D2DContext& cxt);
	static LRESULT df2(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);

};



static void DrawRect1(D2DContext& cxt, FRectF rc, ColorF back, ColorF line)
{
	cxt.DFillRect(rc, back);
	rc.InflateRect(-3,-3);
	cxt.DDrawRect(rc, line, back);
}

static ID2D1SolidColorBrush* CreateBrush(D2DContext& cxt, ColorF clr)
{
	if ( clr.r==0 && clr.g==0 && clr.b==0 )
		return cxt.black_ ;
	else if ( clr.r==1 && clr.g==1 && clr.b==1 )
		return cxt.white_;
	else 
	{
		static ComPTR<ID2D1SolidColorBrush> br;
		
		br = nullptr;

		(*cxt)->CreateSolidColorBrush(clr, &br);
		
		return br;
	}
}

void DrawCenterText(D2DContext& cxt, FRectF rc, LPCWSTR text, ColorF font)
{
	ComPTR<IDWriteTextLayout> layout;
	
	if (SOK(cxt.wfactory_->CreateTextLayout(text, wcslen(text), cxt.textformat_, rc.Width(), rc.Height(), &layout )))
	{
		DWRITE_TEXT_METRICS t;
		layout->GetMetrics(&t);

		FPointF pt( (rc.Width()-t.width)/2.0f, (rc.Height()-t.height)/2.0f );

		auto br = CreateBrush(cxt, font);

		(*cxt)->DrawTextLayout(pt, layout, br );
	}
}




bool CustomBtn::df1(LPVOID captureobj, D2DContext& cxt)
{
	CustomBtn* m = (CustomBtn*)captureobj;
	D2DMatrix mat(*cxt);

	m->mat = mat.PushTransform();
	FRectF& rc = *(m->prc);
	D2DRectFilter fil(cxt, rc);	
	mat.Offset(rc);	

	FRectF rca(0,0,rc.Width(),40);

	DrawRect1(cxt, rca, m->clr, D2RGB(255,255,255));
	DrawCenterText(cxt, rca, m->nm.c_str(), D2RGB(255,255,255));

	mat.PopTransform();

	return false;

}
LRESULT CustomBtn::df2(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;

	CustomBtn* m = (CustomBtn*)captureobj;

	switch(message )
	{
		case WM_D2D_CREATE:
		{
			m->hme = *(UIHandle*)lParam;						
			m->orgrc = D2DGetRect(m->hme);
			m->prc = (FRectF*)D2DGetRect2(m->hme);
			m->nm = D2DGetLocalName(m->hme);
			D2DColor c1(0x998a9e);

			m->clr = c1;
		}
		break;		
		case WM_D2D_DESTROY:
			delete m;
			ret = 1;
		break;
		case WM_LBUTTONDOWN:
		{
			MouseParam& pm = *(MouseParam*)lParam;

			auto pt = m->mat.DPtoLP(pm.pt);

			if ( m->orgrc.PtInRect(pt) )
			{
				
				if ( m->click_ )
					m->click_(m->nm);


				D2DSmoothRect(100,100,m->hme,m->prc, m->orgrc);

				ret = 1;
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			MouseParam& pm = *(MouseParam*)lParam;

			auto pt = m->mat.DPtoLP(pm.pt);

			if ( m->orgrc.PtInRect(pt) )
			{
				
				
				ret= 1;
			}
		}
		break;


	}


	if ( ret == 0 )
		ret = D2DDefControlProc(m->hme,b,message,wParam,lParam);


	return ret;
}

D2DControls* CreateStockChart(D2DControls* ctrl,  FSizeF size, LPCWSTR nm );

LRESULT BobInstance::df2(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;
	BobInstance* m = (BobInstance*)captureobj;
	switch (message)
	{
		case WM_D2D_CREATE:
		{
			m->hme = *(UIHandle*)lParam;						
			m->orgrc = D2DGetRect(m->hme);
			m->prc = (FRectF*)D2DGetRect2(m->hme);

			FRectF rc = *(m->prc);

			float height = rc.Height() / 3.0f;

			rc.bottom = rc.top + height*2;

			// 上側
			auto hP1 = D2DCreateControlsWithScrollbar(m->hme, rc, STAT_DEFAULT|STAT_IGNORE_VSIZE, L"P1" );
			m->hp1 = hP1;
				auto hP1c = D2DCreateSquarePaper(hP1,FRectF(0,0,3000,3000), STAT_DEFAULT|STAT_IGNORE_SIZE,L"P1C");


			
			// 下側
			rc.top = rc.bottom;

			rc.SetHeight(height);
			auto hP2 = D2DCreateControlsWithScrollbar(m->hme, rc, STAT_DEFAULT|STAT_IGNORE_VSIZE, L"P2" );
				//auto hP2c = D2DCreateSquarePaper(hP2,FRectF(0,0,3000,1000), STAT_DEFAULT|STAT_IGNORE_SIZE,L"P2C");
				auto hP2c = D2DCreateXXXControls(hP2,FRectF(0,0,3000,1000), STAT_DEFAULT|STAT_IGNORE_SIZE,L"P2C");


				D2DColor clr(D2RGB(190,190,190));
				LPCWSTR cb = _strformat(L"mode=1&bkcolor=%d", clr.ToInt());
				D2DSendMessage(hP2c, WM_D2D_COMMAND_SET, (WPARAM)hP2c.p, (LPARAM)cb);



				LPCWSTR nm[] = {L"VTI",L"MSFT",L"GOOG",L"AAPL",L"AMZN"};

				FRectF rcs(50,80,FSizeF(150,40));

				for(int i = 0; i < _countof(nm); i++ )
				{
					// カスタマイズされたボタン

					CustomBtn* btn = new CustomBtn();
					D2DCreateWhiteControls((LPVOID)btn,CustomBtn::df1,CustomBtn::df2, hP2c, rcs,STAT_DEFAULT, nm[i], 100+i);

					LPCWSTR titlenm = nm[i];

					btn->click_ = [hP1c, titlenm](std::wstring){
					
						auto h1 = D2DCreateChildWindow(hP1c, FRectF(200,150,FSizeF(1200,650)), STAT_DEFAULT, L"ChildWin_chart" );
						auto h2 = D2DCreateControlsWithScrollbar(h1,FRectF(0,0,FSizeF(0,0)),STAT_DEFAULT|STAT_IGNORE_SIZE,NONAME);


						D2DControls* x = CreateStockChart((D2DControls*)h2.p,  FSizeF(1200,680), titlenm );

						D2DColor clr(D2RGB(250,250,250));
						LPCWSTR cb = _strformat(L"mode=1&title=%s&bkcolor=%d",  titlenm, clr.ToInt());
						D2DSendMessage(h2, WM_D2D_COMMAND_SET, (WPARAM)h2.p,(LPARAM)cb);
						D2DSendMessage(h2, WM_D2D_SET_SIZE, 4,0);
						D2DSendMessage(h1, WM_D2D_SET_SIZE,0,0);


						/*auto h2 = D2DCreateControlsWithScrollbar(hP1c,FRectF(200,100,FSizeF(1300,680)),STAT_DEFAULT,NONAME);
						D2DControls* x = CreateStockChart((D2DControls*)h2.p,  FSizeF(1300,680), titlenm );
						D2DSendMessage(h2, WM_D2D_SET_SIZE, 3,0);*/

					};

					rcs.Offset( 200, 0);


				}

			r = 1;
		}
		break;
		case WM_D2D_DESTROY:
			delete m;
			r = 1;
		break;
		case WM_SIZE:
		{						
			// 全体画面へ
			FRectF rc = *(FRectF*)lParam;
			*(m->prc) = rc;			
		}
		break;


	}

	if ( r == 0 )
		r = D2DDefControlProc(m->hme,b,message,wParam,lParam);

	return r;



}


