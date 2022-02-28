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
			//auto td = parent_window_->name_map_[L"td_left_bar"];
			auto leftw = td->GetRect().Size().width;

			//ls = parent_window_->name_map_[L"td_list"];
			auto leftw2 = ls->GetRect().Size().width;

			//tr = parent_window_->name_map_[L"td_right_bar"];
			auto rightw3 = tr->GetRect().Size().width;

			float w = rc_.Width();
			w -= (leftw+3 + leftw2+5+rightw3);

			//auto ch = parent_window_->name_map_[L"td_chart"];
			auto rc = chart->GetRect();
			rc.SetWidth(w);
			chart->SetRect(rc);

		


		}
		break;




	}


	bool b1 = BITFLG(STAT_ENABLE);

	if ( r == 0 )
		r = D2DControls::WndProc(b,message,wParam,lParam);

	return r;
}





void TDBase::Draw(D2DContext& cxt) 
{
	if ( BITFLG(STAT_VISIBLE) )
	{
		D2DMatrix mat(*cxt);
		mat_ = mat.PushTransform();

		D2DRectFilter fil(cxt, rc_);

		mat.Offset(25,0);
		mat.Offset(rc_);
		cxt.DFillRect(rc_.ZeroRect(), ColorF::LightGray);

	
	
		//auto td = parent_window_->name_map_[L"td_left_bar"];
		td->Draw(cxt);
		mat.Offset(td->GetRect().Size().width, 0);

		mat.Offset(3, 0);
		//auto top = parent_window_->name_map_[L"td_top_bar"];
		top->Draw(cxt);

		//auto txt = parent_window_->name_map_[L"td_stock_cd"];
		txt->Draw(cxt);

		mat.Offset(0, top->GetRect().bottom);

		mat.PushTransform();
		{
			//auto tr = parent_window_->name_map_[L"td_right_bar"];
			mat._31 = rc_.Size().width-tr->GetRect().Size().width;
			mat._32 = 0; 

			(*cxt)->SetTransform(mat);

			tr->Draw(cxt);

			//auto ls = parent_window_->name_map_[L"td_list"];
			mat.Offset(-ls->GetRect().Size().width, 0);
			ls->Draw(cxt);
		}
		mat.PopTransform();

		mat.Offset(0, 1);
		//auto chart = parent_window_->name_map_[L"td_chart"];
		chart->Draw(cxt);

		mat.PopTransform();
	}
}

// //////////////////////////////////////////////////////////////////////////////////////////////////////
TDChart::TDChart():info_(nullptr)
{

}
TDChart::~TDChart()
{
	Clear();
}

void TDChart::Clear()
{
	if ( info_ )
		DeleteInternetInfo(info_);
	info_ = nullptr;
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

void yahooDraw(D2DContext& cxt, InternetInfo* info, float height, std::vector<Rousoku>& adj_values);

void TDChart::Draw(D2DContext& cxt) 
{
	D2DMatrix mat(*cxt);
	mat_ = mat.PushTransform();
	mat.Offset(rc_);
	auto rc = rc_.ZeroRect();

	cxt.DFillRect(rc, ColorF::White);

	
	cxt.DText( rc.LeftTop(), memo_.c_str());
	
	rc.left = rc.right-55.0f;

	cxt.DText( rc.LeftTop(), L"480.00");


	if ( error_.length() > 0)
		cxt.DText( rc_.ZeroRect().LeftTop(), error_);	
	else
		yahooDraw(cxt,info_,rc.Height(), rousoku_ar_);


	mat.PopTransform();
}
void TDChart::Error(std::wstring err)
{
	error_ = err;

}
void TDChart::SetInfo(InternetInfo* info)
{
	Clear();
	info_ = info;

	rousoku_ar_.clear();

}
bool ConvCsv(IStream* ism,std::vector<Rousoku>& adj_values, std::vector<std::string>& dates );



void TDChart::GenerateGraph()
{
	WCHAR cb[256];
	StringCbPrintf(cb,256, L"%d: %s", info_->result, (LPCWSTR)info_->url );

	
	std::vector<std::string> dates;
	
	ConvCsv(info_->pstream,rousoku_ar_,dates);


	
	memo_ = cb;
}



// //////////////////////////////////////////////////////////////////////////////////////////////////////

void inetStockDataDownload(std::wstring cd, std::function<void(InternetInfo*)> compfunc);
#define WM_MYAPP_INET_COMPLETE	(WM_D2D_USER_FIRST)

LPCWSTR NR(LPCWSTR s, std::wstring k)
{
	static std::wstring str;
	str = s;
	str += k;
	return str.c_str();

}


TDChartButtons::TDChartButtons()
{

}
 void TDChartButtons::CreateControl(D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id , D2DColor clr)
 {
	InnerCreateWindow(pacontrol->GetParent(), pacontrol, stat,name,local_id);
	rc_ = rc;
	bkclr_ = clr;
	running_mode_ = 0;
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
				if ( name_ == NR(L"td_top_bar",key_))
				{
					s_btn_mode = -1;
					int k = 0;
					for(auto& rc :  btnar_)
					{						
						if ( k ==0)
						{
							// textbox‚È‚Ì‚Å‚±‚±‚Å‚Íˆ—‚µ‚È‚¢
							
						}						
						else if ( rc.PtInRect(pt ))
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

				if ( s_btn_mode > -1 )
					OnClick(s_btn_mode);

				r = 1;
			}
		}
		break;
		case WM_MYAPP_INET_COMPLETE:
		{
			if ((UINT_PTR)this == (UINT_PTR)wParam)
			{
				InternetInfo* info = (InternetInfo*)lParam;

				auto ch = (TDChart*)parent_window_->name_map_[NR(L"td_chart", key_)];

				if ( info->throwerror!=0 )
				{
					WCHAR cb[256];
					StringCbPrintf(cb,256,L"throwed %d", info->throwerror);

					ch->Error(cb) ;

				}
				else
				{
					ch->SetInfo(info);
					ch->GenerateGraph();
				}

				b.bRedraw = true;		
				running_mode_ = 0;
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

	if ( name_ == NR(L"td_top_bar",key_))
		cxt.DText(btnar_[1].LeftTop(), NR(L"seek\nbtn", key_));

	if ( running_mode_ == 1 )
	{
		int pos = (::GetTickCount64() / 100)%10;
		auto rc = rc_.ZeroRect();
		auto w = 10.0f;
		auto h = rc.Height();

		FRectF xrc(0,0,FSizeF(w,10));
		xrc.Offset((w+5)*pos+100, 0);

		cxt.DFillRect( xrc, ColorF::Blue);

		cxt.Redraw();
	}

	mat.PopTransform();
}



void TDChartButtons::OnClick(int mode)
{
	if ( mode == 1 )
	{
		// seek

		auto complete = [this](InternetInfo* info)
		{
			
			parent_window_->PostMessage(WM_MYAPP_INET_COMPLETE, (WPARAM)this, (LPARAM)info);




		};

		
		//auto cd = (TDChart*)parent_window_->name_map_[L"td_stock_cd"];

		UIHandleWin hw={};
		hw.p = parent_window_;

		auto tx = D2DGetControlFromName(hw, NR(L"td_stock_cd", key_));
		

		BSTR cd = D2DGetText(tx, true );

		inetStockDataDownload( cd, complete );


		::SysFreeString(cd);

		running_mode_ = 1;
	}
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




D2DControls* V6::CreateStockChart(D2DControls* ctrl,  FSizeF size, LPCWSTR k )
{
	float variable = size.width;

	DWORD stat = 0; // STAT_DEFAULT

	auto base = std::make_shared<TDBase>();
	base->CreateControl( ctrl, FRectF(0,0, size), stat, NONAME );
	ctrl->Add(base);

	ctrl = base.get();
	{
		auto left_bar = std::make_shared<TDChartButtons>();
		left_bar->CreateControl( ctrl, FRectF(0,0, FSizeF(55, variable)), STAT_DEFAULT, NR(L"td_left_bar",k),-1, ColorF::White );
		ctrl->Add(left_bar);
		left_bar->key_ = k;

		auto top_bar = std::make_shared<TDChartButtons>();
		top_bar->CreateControl( ctrl, FRectF(0,0, FSizeF(variable,40)), STAT_DEFAULT, NR(L"td_top_bar",k),-1,	D2RGBA(0,0,0,0) );
		ctrl->Add(top_bar);
		top_bar->key_ = k;



		float ws [] = {100,40,40,75,145,130,100,100};
		auto rc = FRectF(0,0,FSizeF(0,40));
		for(int i= 0; i <_countof(ws); i++)
		{	
			rc.SetWidth(ws[i]);
			top_bar->btnar_.push_back(rc);
			rc.Offset(ws[i]+1, 0);
		}

		auto chart = std::make_shared<TDChart>();
		chart->CreateControl( ctrl, FRectF(0,0, FSizeF(1000-45,500)), STAT_DEFAULT, NR(L"td_chart",k) );
		ctrl->Add(chart);

		auto right_list = std::make_shared<TDList>();
		right_list->CreateControl( ctrl, FRectF(0,0, FSizeF(267,variable)), STAT_DEFAULT, NR(L"td_list",k) );
		ctrl->Add(right_list);


		auto right_bar = std::make_shared<TDChartButtons>();
		right_bar->CreateControl( ctrl, FRectF(0,0, FSizeF(45,variable)), STAT_DEFAULT, NR(L"td_right_bar",k), -1, ColorF::White );
		ctrl->Add(right_bar);


		UIHandle hctrls={};
		hctrls.p = ctrl;
		auto txt = D2DCreateTextbox(hctrls, FRectF(5,5,FSizeF(93,30)), false,STAT_DEFAULT, NR(L"td_stock_cd",k));
		D2DSetColor(txt, ColorF::White,ColorF::Black,D2RGBA(0,0,0,0));
		D2DSetText(txt, L"spy" ); // spy: SP500



		base->td = ctrl->GetParent()->name_map_[NR(L"td_left_bar",k)];
		base->top = ctrl->GetParent()->name_map_[NR(L"td_top_bar",k)];
		base->txt = ctrl->GetParent()->name_map_[NR(L"td_stock_cd",k)];
		base->tr = ctrl->GetParent()->name_map_[NR(L"td_right_bar",k)];
		base->ls = ctrl->GetParent()->name_map_[NR(L"td_list",k)];
		base->chart = ctrl->GetParent()->name_map_[NR(L"td_chart",k)];
	}
	
	return base.get();

}