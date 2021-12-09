#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"
#include "D2DChildWindow.h"


using namespace V6;


void CreateControl0(UIHandleWin hwin, UIHandle hcs)
{
	auto ch = std::make_shared<D2DChildWidow>();

	ch->CreateControl((D2DWindow*)hwin.p, (D2DControls*)hcs.p, FRectF(50,250,FSizeF(400,300)), STAT_DEFAULT, L"KOWAI" );

	((D2DControls*)hcs.p)->Add(ch);


}