#include "pch.h"
#include "D2DImageControl.h"

using namespace V6;
#define  APP (D2DApp::GetInstance())


void D2DImageControl::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControl::CreateControl(parent, pacontrol, rc, stat, name, local_id);

    SetRect(rc);
}


LRESULT D2DImageControl::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;


	return ret;
}

void D2DImageControl::Draw(D2DContext& cxt)
{
	(*cxt)->DrawImage(img_,FPointF(0,0));

}