
#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"


#include "D2DMDIControls.h"
#include "D2DClassMap.h"
		  
#include "D2DMDISplitControls.h"
#include "D2DWhiteWindowControl.h"
#include "D2DGrid.h"
//#include "yahoo.h"

using namespace V6;

extern UIHandleWin hwin;
void CreateControl2(UIHandleWin hwin, UIHandle hcs ); // tab3.cpp
void CreateControlTest(UIHandleWin hwin, UIHandle hcs );
bool LoadTextFile( LPCWSTR fnm, std::wstring* str, bool utf8 );


void CreateMDISplitControl(HWND hWnd)
{
	hwin = D2DCreateMainHWnd(hWnd, 14);

	auto root = D2DGetRootControls(hwin);


	RECT rc1;
	GetClientRect(hWnd, &rc1);

	auto frame = D2DCreateMDISplitFrame(root, FRectF(0.0f,0.0f,(float)rc1.right,(float)rc1.bottom), STAT_DEFAULT, L"MDIFrame", 110);

	//auto frame = std::make_shared<D2DMDISplitFrame>();

	//D2DControls* root_controls = (D2DControls*)root.p;
//	frame->CreateControl((D2DWindow*)hwin.p, root_controls, FRectF(0.0f,0.0f,(float)rc1.right,(float)rc1.bottom),  STAT_DEFAULT, L"MDIFrame", 110);
	//root_controls->Add(frame);
	
	//frame->CreateChildView(1);
	D2DMDICreateChildView(frame, 1);

	//auto left_side_controls =(D2DControls*) frame->GetControl(L"LEFT");
	//auto right_side_controls =(D2DControls*) frame->GetControl(L"RIGHT");





	UIHandle hcs4 = D2DMDIGetControl(frame, L"LEFT");
	//hcs4.p = left_side_controls;
	//hcs4.typ = 0;

	
	auto hcs3 = D2DCreateEmptyControls(hcs4,FRectF(0,0,100,1000),  STAT_DEFAULT, L"LeftEmpty", 180);


	
	UIHandle hcsLEFT = D2DCreateSquarePaper(hcs3, FRectF(0,0,600,3000),  STAT_DEFAULT, L"LeftEmptySQ", 190);

	ColorF clr(D2RGB(200,200,200));
	D2DSendMessage(hcsLEFT,WM_D2D_SET_COLOR, 0, (LPARAM)&clr);


	auto img = D2DCreateImage( hcsLEFT, FRectF(100,200,FSizeF(48,48)), STAT_DEFAULT, L"test image");
	D2DSendMessage(img,WM_D2D_SET_RESOURCE, 0, (LPARAM)L"star.png");

	auto img2 = D2DCreateImage( hcsLEFT, FRectF(100,450,FSizeF(48,48)), STAT_DEFAULT, L"test image");
	D2DSendMessage(img2,WM_D2D_SET_RESOURCE, 0, (LPARAM)L"splash_new_rose.png");


	auto b1 = D2DCreateButton(hcsLEFT, FRectF(50,100,FSizeF(100,20)),STAT_VISIBLE,L"LeftEmptySQb1", 191);


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


	UIHandle h1= D2DMDIGetControl(frame, L"RIGHT");
	
	
	auto tabs = D2DCreateTabControls( h1, FRectF(0,0,0,0),  STAT_DEFAULT, L"RIGHT_TAB", 191);


	auto hcs5 = D2DGetControlFromIdx(tabs, 0);

	
	auto v1 = D2DCreateEmptyControls(hcs5, FRectF(0,0,2001,2001), STAT_DEFAULT,L"Control map parent", 201);

	auto maps = std::make_shared<D2DClassMap>();
	maps->CreateControl((D2DWindow*)hwin.p, (D2DControls*)v1.p, FRectF(0,0,0,0),  STAT_DEFAULT, L"Control map", 200);
	((D2DControls*)v1.p)->Add(maps);


	UIHandle hcs_right=hcs5;
	
	for(int i = 0; i < 3; i++ )
	{
		WCHAR nm[64];
		wsprintf(nm,L"aNAME_%d", i);

		UIHandle hcs_right={};


		hcs_right = D2DAddNewTab(tabs, nm );

		wsprintf(nm,L"aNAME_%d_ch", i);

		if (i==1)
		{			
			auto ha = D2DCreateSquarePaper(hcs_right, FRectF(0,0,6000,9000),  STAT_DEFAULT, nm,-1);

			auto clr = D2RGB(37,37,38);
			D2DSendMessage(ha,WM_D2D_SET_COLOR, 0,(LPARAM)&clr);


			D2DWindow* parent = (D2DWindow*)hwin.p;

			/*for(int ij = 0; ij <1; ij++ )
			{
				auto yf = std::make_shared<yahoo_finance>();
				yf->CreateControl(parent, (D2DControls*)ha.p, FRectF(50+ij*100,100+ij*100,FSizeF(1000,500)), STAT_DEFAULT, L"yahoo_finance" );
				((D2DControls*)ha.p)->Add(yf);

			}*/

		}
		else if ( i==0)
		{
			auto kr = D2DCreateSquarePaper(hcs_right, FRectF(0,0,6000,9000),  STAT_DEFAULT, nm,-1 );



			auto tx = D2DCreateTextbox(kr, FRectF(100,100,1000,600), true, STAT_DEFAULT, L"TX1",-1,-1);

			std::wstring str;

			if (LoadTextFile(L"tab3.cpp", &str, false ))
			{
				D2DSetFont(tx, L"‚l‚r –¾’©", 14);
				D2DSetText( tx, str.c_str()); 
				D2DSetColor(tx, ColorF::Gray, ColorF::White, ColorF::Gray);
				

			}

		}
		else
		{
			auto kr = D2DCreateSquarePaper(hcs_right, FRectF(0,0,6000,9000),  STAT_DEFAULT, nm,-1 );

		}

	}

}