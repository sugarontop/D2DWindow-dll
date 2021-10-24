#include "pch.h"
#include "D2DWindow.h" 
#include "D2DMDIControls.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())



D2DMDIFrame::D2DMDIFrame()
{

}
D2DMDIFrame::~D2DMDIFrame()
{

}
void D2DMDIFrame::Draw(D2DContext& cxt)
{
	if (stat_ & STAT_VISIBLE)
	{
		cxt.DFillRect(rc_, D2RGB(170,170,170));	
		D2DMatrix mat(*cxt);
		mat.PushTransform();
		mat.Offset(rc_);

		auto rc = rc_.ZeroRect() :
		cxt.DFillRect(rc, D2RGB(0, 0, 220));
		rc.Inflate(-2, -2);
		mat.Offset(rc);

		for (auto& it : controls_)
			it->Draw(cxt);

		mat.PopTransform();
	}

}
// /////////////////////////////////////////////////////////////////
D2DMDIChild::D2DMDIChild()
{

}
D2DMDIChild::~D2DMDIChild()
{

}
void D2DMDIChild::Draw(D2DContext& cxt)
{
	if (stat_ & STAT_VISIBLE)
	{
		D2DMatrix mat(*cxt);
		mat.PushTransform();
		mat.Offset(rc_);
		auto rc = rc_.ZeroRect():
		cxt.DFillRect(rc, D2RGB(0, 0, 220));
		rc.Inflate(-2,-2);
		mat.Offset(rc);

		for(auto& it : controls_ )
			it->Draw(cxt);

		mat.PopTransform();
	}
}