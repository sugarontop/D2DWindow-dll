#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"



using namespace V6;


void CreateControl0(UIHandleWin hwin, UIHandle hcs)
{

	    FRectF rccmb(50, 100, FSizeF(120, 26));
	    UIHandle cb1 = D2DCreateDropdownListbox(hwin, hcs, rccmb,  STAT_VISIBLE | STAT_ENABLE, L"comb1", 1100);
	    D2DAddItem(cb1, 0, L"scale1.0");
	    D2DAddItem(cb1, 1, L"scale1.2");
	    D2DAddItem(cb1, 2,L"scale0.8");


}