
#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"


#include "D2DMDIControls.h"
		  

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

	frame->CreateControl((D2DWindow*)hwin.p, root_controls, FRectF(0,0,rc1.right,rc1.bottom),  STAT_VISIBLE | STAT_ENABLE, L"MDIFrame", 110);
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




void CreateMDISplitControl(HWND hWnd)
{
	hwin = D2DCreateMainHWnd(hWnd, 14);

	auto root = D2DGetRootControls(hwin);

	auto frame = std::make_shared<D2DMDISplitFrame>();

	D2DControls* root_controls = (D2DControls*)root.p;

	RECT rc1;
	GetClientRect(hWnd, &rc1);

	frame->CreateControl((D2DWindow*)hwin.p, root_controls, FRectF(0,0,rc1.right,rc1.bottom),  STAT_DEFAULT, L"MDIFrame", 110);
	root_controls->Add(frame);
	
	frame->CreateChildView(0);

	auto left_side_controls = frame->GetControl(L"LEFT");

	auto right_side_controls = frame->GetControl(L"RIGHT");


	

	UIHandle hcs3;
	hcs3.p = left_side_controls;
	hcs3.typ = 0;
	D2DCreateSquarePaper(hwin,hcs3, FRectF(0,0,600,3000),  STAT_DEFAULT, L"DEBUG", 190);



	UIHandle hcs4;
	hcs4.p = right_side_controls;
	hcs4.typ = 0;
	auto ha = D2DCreateSquarePaper(hwin,hcs4, FRectF(0,0,5000,2000),  STAT_DEFAULT, L"DEBUG1", 191);



	/*D2DControls* dc = (D2DControls*)dynamic_cast<D2DControls*>((D2DControls*)ha.p);
	auto test = std::make_shared<D2DControls>();
	test->CreateControl((D2DWindow*)hwin.p, dc, FRectF(315,115,FSizeF(600,600)),  STAT_DEBUG, L"DEBUG3", 192);
	dc->Add(test);*/
	
	//D2DCreateSquarePaper(hwin,ha, FRectF(140,140, FSizeF(300,300)),  STAT_DEFAULT, L"DEBUG1", 192);

	//D2DCreateWhiteWindow(hwin,ha, FRectF(250,250, FSizeF(900,900)),  STAT_DEFAULT, L"AAAAA", 192);
	

}