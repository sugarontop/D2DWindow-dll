
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

	//auto h = D2DCreateStatic(root, FRectF(0,0,100,20), STAT_DEFAULT, L"Hello world", NONAME);
	//D2DSetColor(h,ColorF::Blue, ColorF::Blue, ColorF::Black);


	auto bob = new BobInstance();
	D2DCreateWhiteControls((LPVOID)bob,BobInstance::df1,BobInstance::df2, root, FRectF(0,0,1000,1000),STAT_DEFAULT,L"bitcoin",1);

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
			rc.SetWidth(300);

			auto hP1 = D2DCreateControlsWithScrollbar(m->hme, rc, STAT_DEFAULT|STAT_IGNORE_HSIZE, L"P1" );
			m->hp1 = hP1;

			// スクロール範囲もこの値
			auto hP1c = D2DCreateSquarePaper(hP1,FRectF(0,0,3000,3000), STAT_DEFAULT|STAT_IGNORE_SIZE,L"P1C");

			
			
			rc.left = 300;
			rc.top = 70;
			rc.SetWidth(1000);
			auto hP2 = D2DCreateControlsWithScrollbar(m->hme, rc, STAT_DEFAULT, L"P2" );
			auto hP2c = D2DCreateSquarePaper(hP2,FRectF(0,0,3000,3000), STAT_DEFAULT|STAT_IGNORE_SIZE,L"P2C");




			auto hP3 = D2DCreateXXXControls(m->hme,FRectF(300,0,FSizeF(0,70)),STAT_DEFAULT|STAT_IGNORE_VSIZE,L"P3");
			D2DSendMessage(hP3, WM_D2D_COMMAND_SET, (WPARAM)hP3.p, (LPARAM)L"mode=2");

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