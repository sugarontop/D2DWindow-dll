#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"
////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"
#include "chart_top.h"
#include "D2DColor.h"
using namespace V6;
#define  APP (D2DApp::GetInstance())
TDBase::TDBase()
{

}

 void TDBase::CreateControl( D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id )
 {
	InnerCreateWindow(pacontrol->GetParent(), pacontrol, stat,name,local_id);
	rc_ = rc;
	
 }
LRESULT TDBase::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) 
{
	LRESULT r = 0;

	switch (message )
	{
		case WM_SIZE:
		{
			

			float w = rc_.Width();
			w -= (55+3 + 267+5+45);

			auto ch = parent_window_->name_map_[L"td_chart"];
			auto rc = ch->GetRect();
			rc.SetWidth(w);
			ch->SetRect(rc);

		/*	rc.right -= (55+1+45+1);
			auto chart = parent_window_->name_map_[L"td_chart"];
			auto rcc = chart->GetRect();
			rcc.SetWidth(rc.Width());
			chart->SetRect(rcc);


			rc = rc_;
			auto td_right_bar = parent_window_->name_map_[L"td_right_bar"];
			rcc = td_right_bar->GetRect();

			rcc.right = rc.right;
			rcc.left = rcc.right-45;
			td_right_bar->SetRect(rcc);*/


		}
		break;




	}

	if ( r == 0)
		r = D2DControls::WndProc(b,message,wParam,lParam);

	return r;
}
void TDBase::Draw(D2DContext& cxt) 
{
	D2DMatrix mat(*cxt);
	mat_ = mat.PushTransform();

	D2DRectFilter fil(cxt, rc_);
	mat.Offset(rc_);
	cxt.DFillRect(rc_.ZeroRect(), ColorF::LightGray);

	
	auto td = parent_window_->name_map_[L"td_left_bar"];
	td->Draw(cxt);
	mat.Offset(td->GetRect().Size().width, 0);

	mat.Offset(3, 0);
	auto top = parent_window_->name_map_[L"td_top_bar"];
	top->Draw(cxt);

	auto txt = parent_window_->name_map_[L"td_stock_cd"];
	txt->Draw(cxt);

	mat.Offset(0, top->GetRect().bottom);

	mat.PushTransform();
	{
		auto tr = parent_window_->name_map_[L"td_right_bar"];
		mat._31 = rc_.Size().width-tr->GetRect().Size().width;
		mat._32 = 0; 

		(*cxt)->SetTransform(mat);

		tr->Draw(cxt);

		auto ls = parent_window_->name_map_[L"td_list"];
		mat.Offset(-ls->GetRect().Size().width, 0);
		ls->Draw(cxt);
	}
	mat.PopTransform();

	mat.Offset(0, 1);
	auto chart = parent_window_->name_map_[L"td_chart"];
	chart->Draw(cxt);

	mat.PopTransform();
}


TDChart::TDChart()
{

}
TDChart::~TDChart()
{
}
 void TDChart::CreateControl( D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id )
 {
	InnerCreateWindow(pacontrol->GetParent(), pacontrol, stat,name,local_id);
	rc_ = rc;
	
 }
LRESULT TDChart::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) 
{
	LRESULT r = 0;



	return r;
}
void TDChart::Draw(D2DContext& cxt) 
{
	D2DMatrix mat(*cxt);
	mat_ = mat.PushTransform();
	mat.Offset(rc_);
	auto rc = rc_.ZeroRect();

	cxt.DFillRect(rc, ColorF::White);

	rc.left = rc.right-55.0f;

	cxt.DText( rc.LeftTop(), L"480.00");

	mat.PopTransform();
}
// //////////////////////////////////////////////////////////////////////////////////////////////////////
TDChartButtons::TDChartButtons()
{

}
 void TDChartButtons::CreateControl(D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id , D2DColor clr)
 {
	InnerCreateWindow(pacontrol->GetParent(), pacontrol, stat,name,local_id);
	rc_ = rc;
	bkclr_ = clr;
 }
LRESULT TDChartButtons::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) 
{
	LRESULT r = 0;
	static int s_btn_mode = -1;
	switch( message )
	{
		case WM_LBUTTONDOWN:
		{
			MouseParam& mp = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(mp.pt);
			if ( rc_.PtInRect(pt))
			{
				if ( name_ == L"td_top_bar")
				{
					s_btn_mode = -1;
					int k = 0;
					for(auto& rc :  btnar_)
					{						
						if ( rc.PtInRect(pt ))
						{
							APP.SetCapture(this);
							r = 1;

							s_btn_mode = k;
							break;
						}
						k++;
					}
				}

			}
		}
		break;
		case WM_LBUTTONUP:
		{
			if ( APP.IsCapture(this))
			{
				APP.ReleaseCapture();

				if ( btn_mode_ > -1 )
					OnClick(s_btn_mode);

				r = 1;
			}
		}
		break;
	}

	return r;
}
void TDChartButtons::Draw(D2DContext& cxt) 
{
	D2DMatrix mat(*cxt);
	mat_ = mat.PushTransform();
	mat.Offset(rc_);


	cxt.DFillRect(rc_.ZeroRect(), bkclr_);

	for(auto& it : btnar_)
	{
		cxt.DFillRect(it, ColorF::White);
	}

	if ( name_ == L"td_top_bar")
		cxt.DText(btnar_[1].LeftTop(), L"Z");


	mat.PopTransform();
}

void TDChartButtons::OnClick(int mode)
{

}

// //////////////////////////////////////////////////////////////////////////////////////////////////////
TDList::TDList()
{

}
 void TDList::CreateControl(D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id )
 {
	InnerCreateWindow(pacontrol->GetParent(), pacontrol, stat,name,local_id);
	rc_ = rc;
 }
LRESULT TDList::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) 
{
	LRESULT r = 0;



	return r;
}
void TDList::Draw(D2DContext& cxt) 
{
	D2DMatrix mat(*cxt);
	mat_ = mat.PushTransform();
	mat.Offset(rc_);


	cxt.DFillRect(rc_.ZeroRect(), D2RGB(220,220,220));

	
	mat.PopTransform();
}


// //////////////////////////////////////////////////////////////////////////////////////////////////////
void V6::CreateStockChart(D2DControls* ctrl,  FSizeF size )
{
	float variable = size.width;

	auto base = std::make_shared<TDBase>();
	base->CreateControl( ctrl, FRectF(0,0, size), STAT_DEFAULT, NONAME );
	ctrl->Add(base);

	ctrl = base.get();
	auto left_bar = std::make_shared<TDChartButtons>();
	left_bar->CreateControl( ctrl, FRectF(0,0, FSizeF(55, variable)), STAT_DEFAULT, L"td_left_bar",-1, ColorF::LightBlue );
	ctrl->Add(left_bar);

	auto top_bar = std::make_shared<TDChartButtons>();
	top_bar->CreateControl( ctrl, FRectF(0,0, FSizeF(variable,40)), STAT_DEFAULT, L"td_top_bar",-1,	D2RGBA(0,0,0,0) );
	ctrl->Add(top_bar);

	float ws [] = {100,40,40,75,145,130,100,100};
	auto rc = FRectF(0,0,FSizeF(0,40));
	for(int i= 0; i <_countof(ws); i++)
	{	
		rc.SetWidth(ws[i]);
		top_bar->btnar_.push_back(rc);
		rc.Offset(ws[i]+1, 0);
	}

	auto chart = std::make_shared<TDChart>();
	chart->CreateControl( ctrl, FRectF(0,0, FSizeF(1000-45,500)), STAT_DEFAULT, L"td_chart" );
	ctrl->Add(chart);

	auto right_list = std::make_shared<TDList>();
	right_list->CreateControl( ctrl, FRectF(0,0, FSizeF(267,variable)), STAT_DEFAULT, L"td_list" );
	ctrl->Add(right_list);


	auto right_bar = std::make_shared<TDChartButtons>();
	right_bar->CreateControl( ctrl, FRectF(0,0, FSizeF(45,variable)), STAT_DEFAULT, L"td_right_bar", -1, D2RGB(240,243,250) );
	ctrl->Add(right_bar);


	UIHandle hctrls={};
	hctrls.p = ctrl;
	auto txt = D2DCreateTextbox(hctrls, FRectF(5,5,FSizeF(93,30)), false,STAT_DEFAULT, L"td_stock_cd");
	D2DSetColor(txt, ColorF::White,ColorF::Black,D2RGBA(0,0,0,0));

}