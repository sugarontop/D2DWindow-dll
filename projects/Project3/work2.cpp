
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
	CustomBtn()
	{

	}

	FRectF* prc;
	FRectF orgrc;
	D2DMat mat;
	UIHandle hme;
	std::wstring nm;
	D2DColor clr;


	static bool df1(LPVOID captureobj, D2DContext& cxt);
	static LRESULT df2(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);

};

bool CustomBtn::df1(LPVOID captureobj, D2DContext& cxt)
{
	CustomBtn* m = (CustomBtn*)captureobj;
	D2DMatrix mat(*cxt);

	m->mat = mat.PushTransform();
	FRectF& rc = *(m->prc);
	D2DRectFilter fil(cxt, rc);	
	mat.Offset(rc);	
	cxt.DFillRect(rc.ZeroRect(), m->clr);

	cxt.DText(FPointF(20,50), m->nm, cxt.white_ );
	


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
			m->clr = ColorF::Blue;
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

			// �㑤
			auto hP1 = D2DCreateControlsWithScrollbar(m->hme, rc, STAT_DEFAULT|STAT_IGNORE_VSIZE, L"P1" );
			m->hp1 = hP1;
				auto hP1c = D2DCreateSquarePaper(hP1,FRectF(0,0,3000,3000), STAT_DEFAULT|STAT_IGNORE_SIZE,L"P1C");


			
			// ����
			rc.top = rc.bottom;

			rc.SetHeight(height);
			auto hP2 = D2DCreateControlsWithScrollbar(m->hme, rc, STAT_DEFAULT|STAT_IGNORE_VSIZE, L"P2" );
				//auto hP2c = D2DCreateSquarePaper(hP2,FRectF(0,0,3000,1000), STAT_DEFAULT|STAT_IGNORE_SIZE,L"P2C");
				auto hP2c = D2DCreateXXXControls(hP2,FRectF(0,0,3000,1000), STAT_DEFAULT|STAT_IGNORE_SIZE,L"P2C");


				D2DColor clr(D2RGB(190,190,190));
				LPCWSTR cb = _strformat(L"mode=1&bkcolor=%d", clr.ToInt());
				D2DSendMessage(hP2c, WM_D2D_COMMAND_SET, (WPARAM)hP2c.p, (LPARAM)cb);



				LPCWSTR nm[] = {L"VTI",L"MSFT",L"GOOG",L"AAPL",L"AMZN"};

				FRectF rcs(50,80,FSizeF(150,100));

				for(int i = 0; i < _countof(nm); i++ )
				{
					// �J�X�^�}�C�Y���ꂽ�{�^��

					CustomBtn* btn = new CustomBtn();
					D2DCreateWhiteControls((LPVOID)btn,CustomBtn::df1,CustomBtn::df2, hP2c, rcs,STAT_DEFAULT, nm[i], 100+i);


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
			// �S�̉�ʂ�
			FRectF rc = *(FRectF*)lParam;
			*(m->prc) = rc;			
		}
		break;


	}

	if ( r == 0 )
		r = D2DDefControlProc(m->hme,b,message,wParam,lParam);

	return r;



}


