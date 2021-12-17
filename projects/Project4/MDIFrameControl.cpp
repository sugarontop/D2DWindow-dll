
#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"


#include "D2DMDIControls.h"
#include "D2DClassMap.h"
		  

using namespace V6;

extern UIHandleWin hwin;
void CreateControl2(UIHandleWin hwin, UIHandle hcs ); // tab3.cpp
void CreateControlTest(UIHandleWin hwin, UIHandle hcs );


void CreateMDIControl(HWND hWnd)
{

	hwin = D2DCreateMainHWnd(hWnd, 14);

	auto root = D2DGetRootControls(hwin);

	auto frame = std::make_shared<D2DMDIFrame>();

	D2DControls* root_controls = (D2DControls*)root.p;

	RECT rc1;
	GetClientRect(hWnd, &rc1);

	frame->CreateControl((D2DWindow*)hwin.p, root_controls, FRectF(0.0f,0.0f,(float)rc1.right,(float)rc1.bottom),  STAT_VISIBLE | STAT_ENABLE, L"MDIFrame", 110);
	root_controls->Add(frame);


	auto c1 = frame->Add(FRectF(0,0,FSizeF(400,800)), STAT_VISIBLE | STAT_ENABLE, L"view1", 111  );
	UIHandle hcs;
	hcs.p = c1.get();
	hcs.typ = 0;

	CreateControl2(hwin, hcs);


	auto c2 = frame->Add(FRectF(450,0,FSizeF(400,800)), STAT_VISIBLE | STAT_ENABLE, L"view2", 112  );
	UIHandle hcs2;
	hcs2.p = c2.get();
	hcs2.typ = 0;
	CreateControl2(hwin, hcs2);

	auto c3 = frame->Add(FRectF(200,400,FSizeF(700,800)), STAT_VISIBLE | STAT_ENABLE, L"view3", 113  );

	UIHandle hcs3;
	hcs3.p = c3.get();
	hcs3.typ = 0;
	CreateControlTest(hwin, hcs3);






	
}

#include "D2DMDISplitControls.h"
//#include "D2DSquarePaper.h" 


#include "D2DWhiteWindowControl.h"
#include "D2DGrid.h"
#include "D2DTabControls.h"
#include "yahoo.h"

void CreateMDISplitControl(HWND hWnd)
{
	hwin = D2DCreateMainHWnd(hWnd, 14);

	auto root = D2DGetRootControls(hwin);

	auto frame = std::make_shared<D2DMDISplitFrame>();

	D2DControls* root_controls = (D2DControls*)root.p;

	RECT rc1;
	GetClientRect(hWnd, &rc1);

	frame->CreateControl((D2DWindow*)hwin.p, root_controls, FRectF(0.0f,0.0f,(float)rc1.right,(float)rc1.bottom),  STAT_DEFAULT, L"MDIFrame", 110);
	root_controls->Add(frame);
	
	frame->CreateChildView(1);

	auto left_side_controls =(D2DControls*) frame->GetControl(L"LEFT");
	auto right_side_controls =(D2DControls*) frame->GetControl(L"RIGHT");


	UIHandle hcs4;
	hcs4.p = left_side_controls;
	hcs4.typ = 0;

	//auto dws = std::make_shared<D2DControls_with_Scrollbar>();
	//dws->CreateControl((D2DWindow*)hwin.p,left_side_controls, FRectF(0,0,300,1000),  STAT_DEFAULT, L"aaa", 180);
	//left_side_controls->Add(dws);

	auto hcs3 = D2DCreateEmptyControls(hwin,hcs4,FRectF(0,0,100,1000),  STAT_DEFAULT, L"LeftEmpty", 180);


	
	UIHandle hcsLEFT = D2DCreateSquarePaper(hwin,hcs3, FRectF(0,0,600,3000),  STAT_DEFAULT, L"LeftEmptySQ", 190);

	ColorF clr(D2RGB(200,200,200));
	D2DSendMessage(hcsLEFT,WM_D2D_SET_COLOR, 0, (LPARAM)&clr);


	auto img = D2DCreateImage(hwin, hcsLEFT, FRectF(100,200,FSizeF(48,48)), STAT_DEFAULT, L"test image");
	D2DSendMessage(img,WM_D2D_SET_RESOURCE, 0, (LPARAM)L"star.png");

	auto img2 = D2DCreateImage(hwin, hcsLEFT, FRectF(100,450,FSizeF(48,48)), STAT_DEFAULT, L"test image");
	D2DSendMessage(img2,WM_D2D_SET_RESOURCE, 0, (LPARAM)L"splash_new_rose.png");


	auto b1 = D2DCreateButton(hwin,hcsLEFT, FRectF(50,100,FSizeF(100,20)),STAT_VISIBLE,L"LeftEmptySQb1", 191);


	DelegateProcFunc f = [](LPVOID xp,AppBase& b, UINT message, WPARAM wp, LPARAM lp)->LRESULT
	{
		LRESULT h = 0;

		switch( message )
		{
			case WM_D2D_TAB_CHANGE:
			{
				D2DControl* pc = (D2DControl*)wp;

				auto nm1 = pc->GetName();


				{
					auto idx = lp;
					auto bm = pc->GetParent()->name_map_[L"_b1"];

					if ( bm )
						bm->SetStat((idx==2) ? STAT_DEFAULT: STAT_VISIBLE);
								

					h = 1;
				}
			}
			break;
			case WM_NOTIFY:
			{
				D2DNMHDR& d = *(D2DNMHDR*)lp;

				if(wp == 191)
				{
					


					h = 1;
				}
			}
			break;
		}
		return h;
	};

	D2DSetProcfunction(hcsLEFT, f);


	



#ifdef SAMPLE1
	UIHandle hcs4;
	hcs4.p = right_side_controls;
	hcs4.typ = 0;
	auto ha = D2DCreateSquarePaper(hwin,hcs4, FRectF(0,0,6000,9000),  STAT_DEFAULT, L"DEBUG1", 191);
	auto ha2 = D2DCreateWhiteWindow(hwin,ha, FRectF(0,0, FSizeF(900,900)),  STAT_DEFAULT, L"AAAAA", 192);
#endif

#ifdef SAMPLE2
		UIHandle hcs4;
	hcs4.p = right_side_controls;
	hcs4.typ = 0;
	auto ha = D2DCreateSquarePaper(hwin,hcs4, FRectF(0,0,6000,9000),  STAT_DEFAULT, L"DEBUG1", 191);
	auto ha1 = D2DCreateSquarePaper(hwin,ha, FRectF(145,145, FSizeF(300,300)),  STAT_DEFAULT, L"DEBUG10", 192);
#endif

#ifdef SAMPLE3
	UIHandle hcs4;
	hcs4.p = right_side_controls;
	hcs4.typ = 0;
	auto ha = D2DCreateSquarePaper(hwin,hcs4, FRectF(0,0,6000,9000),  STAT_DEFAULT, L"DEBUG1", 191);
	for(int i=0; i < 10; i++)
	{

		auto win = std::make_shared<D2DWhiteWindowControls>();
		D2DControls* ctrl = (D2DControls*)ha.p;
		win->CreateControl((D2DWindow*)hwin.p, ctrl, FRectF(150+i*10,150+i*10, FSizeF(600,300)), STAT_DEFAULT, L"DEBUG10", 192);
		ctrl->Add(win);

	
	
		auto grid = std::make_shared<D2DGrid>();
		grid->CreateControl((D2DWindow*)hwin.p, win.get(), FRectF(0,0, FSizeF(600,300)), STAT_DEFAULT, L"DEBUG10", 192);
		win->Add(grid);

	}
#endif
	
	auto tabs = std::make_shared<D2DTabControls>();
	tabs->CreateControl((D2DWindow*)hwin.p,right_side_controls, FRectF(0,0,0,0),  STAT_DEFAULT, L"RIGHT_TAB", 191);
	right_side_controls->Add(tabs);

	auto p1 = dynamic_cast<D2DControls*>(tabs->GetControlFromIdx(0));


	UIHandle hcs5={};
	hcs5.p = p1;
	auto v1 = D2DCreateEmptyControls(hwin, hcs5, FRectF(0,0,2001,2001), STAT_DEFAULT,L"Control map parent", 201);

	auto maps = std::make_shared<D2DClassMap>();
	maps->CreateControl((D2DWindow*)hwin.p, (D2DControls*)v1.p, FRectF(0,0,0,0),  STAT_DEFAULT, L"Control map", 200);
	((D2DControls*)v1.p)->Add(maps);


	UIHandle hcs_right={};
	hcs_right.p = p1;

	//D2DCreateSquarePaper(hwin,hcs_right, FRectF(0,0,2000,3000),  STAT_DEFAULT, L"DEBUG", 190);


	//for(int k = 10; k < 3; k++)
	//{
	//	p1 = tabs->AddNewTab(L"page1");
	//	hcs_right.p = p1;
	//	auto kr = D2DCreateSquarePaper(hwin,hcs_right, FRectF(0,0,2000,3000),  STAT_DEFAULT, L"DEBUG2", 191+k);


	//	if (k ==1)
	//	{
	//		ColorF clr(ColorF::SteelBlue);
	//		D2DSendMessage(kr,WM_D2D_SET_COLOR, 0, (LPARAM)&clr);
	//	}
	//	if (k ==2)
	//	{
	//		ColorF clr(ColorF::MidnightBlue);
	//		D2DSendMessage(kr,WM_D2D_SET_COLOR, 0, (LPARAM)&clr);

	//		ColorF clr2(ColorF::Red);
	//		D2DSendMessage(kr,WM_D2D_SET_COLOR, 1, (LPARAM)&clr2);
	//	}

	//}

	for(int i = 0; i < 3; i++ )
	{
		WCHAR nm[64];
		wsprintf(nm,L"aNAME_%d", i);

		UIHandle hcs_right={};
		hcs_right.p = p1 = tabs->AddNewTab(nm);

		//auto page1 = std::make_shared<D2DControls_with_Scrollbar>();
		//page1->CreateControl(parent,this, FRectF(0,0,0,0), STAT_DEFAULT, nm );
		//Add(page1);

		wsprintf(nm,L"aNAME_%d_ch", i);

		if (i==1)
		{			
			auto ha = D2DCreateSquarePaper(hwin,hcs_right, FRectF(0,0,6000,9000),  STAT_DEFAULT, nm,-1);

			D2DWindow* parent = (D2DWindow*)hwin.p;

			for(int ij = 0; ij <2; ij++ )
			{
				yahoo_finance* yf = new yahoo_finance();
				yf->CreateControl(parent, (D2DControls*)ha.p, FRectF(50+ij*100,100+ij*100,FSizeF(1000,500)), STAT_DEFAULT, L"yahoo_finance" );
				((D2DControls*)ha.p)->Add(std::shared_ptr<yahoo_finance>(yf));

			}
		}
		else
		{
			auto kr = D2DCreateSquarePaper(hwin,hcs_right, FRectF(0,0,6000,9000),  STAT_DEFAULT, nm,-1 );
		}

	}

}