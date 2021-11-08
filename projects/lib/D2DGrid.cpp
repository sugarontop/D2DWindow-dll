#include "pch.h"
#include "D2DGrid.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())


D2DGrid::D2DGrid()
{ 

}

void D2DGrid::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id )
{
	D2DControls::CreateControl(parent, pacontrol,rc,stat,name,local_id);

	rc_ = rc;
}
HRESULT D2DGrid::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = 0;

	switch(message)
	{
		case WM_D2D_CREATE:
			hr = 1;
		break;



	}
	return hr;

}
void D2DGrid::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();
	mat.Offset(rc_);
	auto rc = rc_.ZeroRect();

	cxt.DFillRect(rc, theBlack);


	int colc = 3;
	int colr = 3;


	float w = (rc.Width() - (colc+1))/ (float)colc;
	float h = (rc.Height() - (colr+1))/(float)colr;


	for(int ir = 0; ir < colr; ir++ )
	{
		mat.PushTransform();
		for(int ic = 0; ic < colc; ic++ )
		{
			FRectF rc(1,1,1+w,1+h);
			cxt.DFillRect(rc, theWhite);
			
			mat.Offset(1+w,0);
		}

		mat.PopTransform();


		mat.Offset(0,1+h);
	}



	mat.PopTransform();
}