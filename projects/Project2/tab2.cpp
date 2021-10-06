#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"



using namespace V6;

void CreateControl1(UIHandleWin hwin, UIHandle hcs )
{


	FRectF rc2(10, 40, FSizeF(400, 600));
	UIHandle x2 = D2DCreateTextbox(hwin, hcs, rc2, true, STAT_VISIBLE | STAT_ENABLE, L"textbox200");

	
	D2DSetText(x2, L"Hello\nworld");

	FRectF rc1( rc2.left, rc2.bottom+5, FSizeF(100,26));
	auto btn1 = D2DCreateButton(hwin,hcs, rc1, STAT_VISIBLE | STAT_ENABLE, L"Load", 400);
	rc1.Offset(200, 0);
	auto btn2 = D2DCreateButton(hwin,hcs, rc1, STAT_VISIBLE | STAT_ENABLE, L"Save", 401);


}