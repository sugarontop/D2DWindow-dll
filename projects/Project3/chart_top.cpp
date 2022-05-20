#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"
////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"
#include "chart_top.h"
#include "D2DColor.h"
#include "imagetool.h"
using namespace V6;
#define  APP (D2DApp::GetInstance())

#define WM_MYAPP_INET_COMPLETE	(WM_D2D_USER_FIRST)

//bitcoin: https://www.blockchain.com/charts/market-price

void yahooDraw(D2DContext& cxt, std::wstring cd, InternetInfo* info, FSizeF vsz, std::vector<Rousoku>& adj_values);
bool CreateRousokuFromtStream(LPCSTR cd, bool bSave, IStream* ism,std::vector<Rousoku>& adj_values, std::vector<std::string>& dates );



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
			auto leftw = td->GetRect().Size().width;
			auto leftw2 = ls->GetRect().Size().width;
			auto rightw3 = tr->GetRect().Size().width;

			float w = rc_.Width();
			w -= (leftw+3 + leftw2+5+rightw3);

			auto rc = chart->GetRect();
			rc.SetWidth(w);
			chart->SetRect(rc);
		}
		break;
	}

	if ( r == 0 )
		r = D2DControls::DefWndProc(b,message,wParam,lParam);

	return r;
}

void TDBase::Draw(D2DContext& cxt) 
{
	if ( BITFLG(STAT_VISIBLE) )
	{
		D2DMatrix mat(*cxt);
		mat_ = mat.PushTransform();

		D2DRectFilter fil(cxt, rc_);

		//mat.Offset(25,0);
		mat.Offset(rc_);
		cxt.DFillRect(rc_.ZeroRect(), ColorF::LightGray);


		td->Draw(cxt);
		mat.Offset(td->GetRect().Size().width, 0);

		mat.Offset(3, 0);
		top->Draw(cxt);

		txt->Draw(cxt);

		mat.Offset(0, top->GetRect().bottom);

		//mat.PushTransform();
		//{
		//	mat.Offset(rc_.Size().width - tr->GetRect().Size().width, 0);

		//	(*cxt)->SetTransform(mat);

		//	tr->Draw(cxt);

		//	mat.Offset(-ls->GetRect().Size().width, 0);
		//	ls->Draw(cxt);
		//}
		//mat.PopTransform();

		mat.Offset(0, 1);
		chart->Draw(cxt);

		mat.PopTransform();
	}
}

// //////////////////////////////////////////////////////////////////////////////////////////////////////
TDChart::TDChart(std::wstring cd):info_(nullptr),cd_(cd)
{

}
TDChart::~TDChart()
{
	Clear();
}

void TDChart::Clear()
{

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


	switch( message )
	{
		case WM_RBUTTONDOWN:
		{
			MouseParam& pm = *(MouseParam*)lParam;

			auto pt = mat_.DPtoLP(pm.pt);

			if ( rc_.PtInRect(pt))
			{
				//GenerateBitmap();
				r = 1;
			}
		}
		break;
		case WM_MYAPP_INET_COMPLETE:
		{
			if ((UINT_PTR)this == (UINT_PTR)wParam)
			{
				InternetInfo* info = (InternetInfo*)lParam;

				TDChart* ch = this; //dynamic_cast<TDChart*>(chart_);
				_ASSERT(ch);

				if ( info->throwerror!=0 )
				{
					WCHAR cb[256];
					StringCbPrintf(cb,256,L"throwed %d", info->throwerror);

					ch->Error(cb) ;

				}
				else
				{
					std::shared_ptr<InternetInfo> ainfo(info, [](InternetInfo* p){ DeleteInternetInfo(p); });

					ch->SetInfo(ainfo);
					ch->GenerateGraph();


					// page2‚Ìchat‚É‚à...
					//UIHandleWin hw={};
					//hw.p = parent_window_;
					//auto h1 = D2DGetControlFromName(hw, L"#page2_chart");
					//auto ch2 = dynamic_cast<TDChart*>( (D2DControl*)h1.p );
					//if ( ch2 )
					//{
					//	ch2->SetInfo(ainfo);
					//	ch2->GenerateGraph();

					//	//ch2->Detach();
					//}

				}

				b.bRedraw = true;		
				//running_mode_ = 0;
				r = 1;
			}
		}
		break;
	}
	return r;
}



void TDChart::Draw(D2DContext& cxt) 
{
	D2DMatrix mat(*cxt);
	mat_ = mat.PushTransform();
	mat.Offset(rc_);
	auto rc = rc_.ZeroRect();

	cxt.DFillRect(rc, ColorF::White);

	
	cxt.DText( rc.LeftTop(), memo_.c_str());
	
	rc.left = rc.right-55.0f;

	if ( error_.length() > 0)
		cxt.DText( rc_.ZeroRect().LeftTop(), error_);	
	else
	{
		if ( info_.get() && info_.get()->result == 200 )
			yahooDraw(cxt, cd_, nullptr,rc_.Size(), rousoku_ar_);
		else if ( info_.get() == nullptr )
			yahooDraw(cxt, cd_, nullptr,rc_.Size(), rousoku_ar_);
	}



	mat.PopTransform();
}
void TDChart::Error(std::wstring err)
{
	error_ = err;

}


void TDChart::SetInfo(std::shared_ptr<InternetInfo> info)
{
	Clear();
	info_ = info;

	rousoku_ar_.clear();

}

void TDChart::GenerateGraph()
{
	WCHAR cb[256];

	if ( info_->url == nullptr )
		StringCbPrintf(cb,256, L"%s", L"sample, \"stock.bin\"");
	else
		StringCbPrintf(cb,256, L"%d: %s", info_->result, (LPCWSTR)info_->url );

	
	std::vector<std::string> dates;


	char cd1[1024]={};
	::WideCharToMultiByte(CP_ACP,0,cd_.c_str(), cd_.length(), cd1, 1024,nullptr,nullptr);
	
	CreateRousokuFromtStream(cd1,true, info_->pstream,rousoku_ar_,dates);

	//WriteData(L"stock.bin", info_->pstream );

	memo_ = cb;

	parent_control_->SendMesage(WM_D2D_SET_SIZE,3,0);

}
bool DbReadStockData( LPCSTR cd, IStream** ppout);
std::string W2A(const std::wstring& s);

void TDChart::GenerateGraph2(LPCWSTR cd)
{
	auto acd = W2A(cd);

	cd_ = cd;

	std::vector<std::string> dates;

	ComPTR<IStream> sm;
	if ( DbReadStockData( acd.c_str(), &sm))
		CreateRousokuFromtStream(acd.c_str(),false, sm, rousoku_ar_, dates);
	
	memo_ = L"from db";
	parent_control_->SendMesage(WM_D2D_SET_SIZE,3,0);
}

void TDChart::GenerateBitmap()
{
	//auto& cxt = this->GetParent()->GetContext();

	//auto rc1 = mat_.LPtoDP(rc_);
	//
	//D2D1_RECT_U rc;
	//rc.left = (UINT)rc1.left;
	//rc.top = (UINT)rc1.top;
	//rc.right = (UINT)rc1.right;
	//rc.bottom = (UINT)rc1.bottom;

	//mini_bmp_ = nullptr;

	//RenderTargetToBitmp(*cxt, rc, &mini_bmp_);





}

// //////////////////////////////////////////////////////////////////////////////////////////////////////

void inetStockDataDownload(std::wstring cd, std::function<void(InternetInfo*)> compfunc);


LPCWSTR NR(LPCWSTR s, std::wstring k)
{
	static std::wstring str;
	str = s;
	str += k;
	return str.c_str();

}
std::wstring GetGlobalName(UIHandle t, LPCWSTR localnm)
{
	std::wstring nm = D2DGetName(D2DGetParent(t));
	nm += L"@";
	nm += localnm;
	return nm;
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
				if ( GetLocalName() == NR(L"td_top_bar",key_))
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

				auto ch = dynamic_cast<TDChart*>(chart_);
				_ASSERT(ch);

				if ( info->throwerror!=0 )
				{
					WCHAR cb[256];
					StringCbPrintf(cb,256,L"throwed %d", info->throwerror);

					ch->Error(cb) ;

				}
				else
				{
					std::shared_ptr<InternetInfo> ainfo(info, [](InternetInfo* p){ DeleteInternetInfo(p); });


					ch->SetInfo(ainfo);
					ch->GenerateGraph();


					// page2‚Ìchat‚É‚à...
					UIHandleWin hw={};
					hw.p = parent_window_;
					auto h1 = D2DGetControlFromName(hw, L"#page2_chart");
					auto ch2 = dynamic_cast<TDChart*>( (D2DControl*)h1.p );
					if ( ch2 )
					{
						ch2->SetInfo(ainfo);
						ch2->GenerateGraph();

						//ch2->Detach();
					}

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

	if ( GetLocalName() == NR(L"td_top_bar",key_))
	{
		cxt.DText(btnar_[1].LeftTop(0,5), L"*Seek*");
		cxt.DText(btnar_[2].LeftTop(0,5), L"*DbSeek*");
	}


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
		LoadDataInternet();
	}
	else if ( mode == 2 )
	{
		// The Data load from Database.
		LoadDataFromDB();
	}
}

bool DbReadStockData( LPCSTR cd, IStream** ppout);
std::string W2A(std::wstring s);

void TDChartButtons::LoadDataFromDB()
{
	UIHandle t;
	t.p = this;
	UIHandleWin hw={};
	hw.p = parent_window_;

	std::wstring nm = GetGlobalName(t,NR(L"td_stock_cd",key_));
	auto tx = D2DGetControlFromName(hw, nm.c_str());
	BSTR cd = D2DGetText(tx, true );


	((TDChart*)chart_)->GenerateGraph2(cd);

}
void TDChartButtons::LoadDataInternet()
{
	auto complete = [this](InternetInfo* info)
	{
		parent_window_->PostMessage(WM_MYAPP_INET_COMPLETE, (WPARAM)this, (LPARAM)info);
	};

	UIHandleWin hw={};
	hw.p = parent_window_;

	UIHandle t;
	t.p = this;

	std::wstring nm = GetGlobalName(t,NR(L"td_stock_cd",key_));

	auto tx = D2DGetControlFromName(hw, nm.c_str());
	BSTR cd = D2DGetText(tx, true );

	inetStockDataDownload( cd, complete );

	::SysFreeString(cd);

	running_mode_ = 1;
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

D2DControls* CreateSolidStockChart(D2DControls* ctrl, std::wstring cd, FRectF rc, LPCWSTR nm )
{
	UIHandle h={};
	h.p = ctrl;
	auto hchart = D2DCreateControlsWithScrollbar(h,rc, STAT_DEFAULT, NR(L"page2_td_chart_sc",nm));
	D2DControls* ctrl2 = (D2DControls*)hchart.p;

	auto chart = std::make_shared<TDChart>( cd );
	chart->CreateControl( ctrl2, rc, STAT_DEFAULT, L"#page2_chart" );
	ctrl2->Add(chart);
	return chart.get();
}

D2DControls* CreateSolidStockChart2(D2DControls* ctrl, std::wstring cd, FRectF rc, LPCWSTR nm )
{
	auto chart = std::make_shared<TDChart>( cd );
	chart->CreateControl( ctrl, rc, STAT_DEFAULT, nm );
	ctrl->Add(chart);

	

	return chart.get();
}


D2DControls* CreateStockChart(D2DControls* ctrl,  FSizeF size, LPCWSTR nm )
{
	float variable = size.width;

	DWORD stat =  STAT_DEFAULT;

	auto base = std::make_shared<TDBase>();
	base->CreateControl( ctrl, FRectF(0,0, size), stat, nm );
	ctrl->Add(base);

	ctrl = base.get();

	{
		auto left_bar = std::make_shared<TDChartButtons>();
		left_bar->CreateControl( ctrl, FRectF(0,0, FSizeF(55, variable)), STAT_DEFAULT, NR(L"td_left_bar",nm),-1, ColorF::White );
		ctrl->Add(left_bar);
		left_bar->key_ = nm;
		base->td = left_bar.get();

		auto top_bar = std::make_shared<TDChartButtons>();
		top_bar->CreateControl( ctrl, FRectF(0,0, FSizeF(variable,40)), STAT_DEFAULT, NR(L"td_top_bar",nm),-1,	D2RGBA(0,0,0,0) );
		ctrl->Add(top_bar);
		top_bar->key_ = nm;
		base->top = top_bar.get();


		float ws [] = {100,40,40,75,145,130,100,100};
		auto rc = FRectF(0,0,FSizeF(0,40));
		for(int i= 0; i <_countof(ws); i++)
		{	
			rc.SetWidth(ws[i]);
			top_bar->btnar_.push_back(rc);
			rc.Offset(ws[i]+1, 0);
		}

		{
			
			UIHandle h={};
			h.p = ctrl;
			auto hchart = D2DCreateControlsWithScrollbar(h,FRectF(0,0, FSizeF(1000,500)), STAT_DEFAULT|STAT_IGNORE_SIZE, NR(L"td_chart_sc11",nm));
			D2DControls* ctrl2 = (D2DControls*)hchart.p;

			auto cd = nm;
			auto chart = std::make_shared<TDChart>(cd);
			chart->CreateControl( ctrl2, FRectF(0,0, FSizeF(1000,500)), STAT_DEFAULT, NR(L"td_chart",nm) );
			ctrl2->Add(chart);
			
			base->chart = ctrl2;

			left_bar->chart_ = chart.get();
			top_bar->chart_ = chart.get();


			D2DSendMessage(hchart, WM_D2D_SET_SIZE, 3,0);
			
		}




		/*auto right_list = std::make_shared<TDList>();
		right_list->CreateControl( ctrl, FRectF(0,0, FSizeF(267,variable)), STAT_DEFAULT, NR(L"td_list",nm) );
		ctrl->Add(right_list);
		base->ls = right_list.get();

		auto right_bar = std::make_shared<TDChartButtons>();
		right_bar->CreateControl( ctrl, FRectF(0,0, FSizeF(45,variable)), STAT_DEFAULT, NR(L"td_right_bar",nm), -1, ColorF::White );
		ctrl->Add(right_bar);
		base->tr = right_bar.get();*/


		UIHandle hctrls={};
		hctrls.p = ctrl;
		auto txt = D2DCreateTextbox(hctrls, FRectF(5,5,FSizeF(93,30)), false,STAT_DEFAULT, NR(L"td_stock_cd",nm));
		D2DSetColor(txt, ColorF::White,ColorF::Black,D2RGBA(0,0,0,0));
		D2DSetText(txt, nm );
		D2DSendMessage(txt, WM_D2D_COMMAND_SET, (WPARAM)txt.p,(LPARAM)L"weight=600");
		
		base->txt = (D2DControl*)txt.p;

	}
	
	return base.get();

}


D2DControls* CreateWealthNaviStockChart(D2DControls* ctrl,  FSizeF size, LPCWSTR nm )
{
	LPCWSTR cd[] = {L"VTI",L"VEA",L"VWO",L"GLD",L"IYR",L"AGG" };

	FSizeF sz(500,300);
	FRectF rc(0,0, sz);

	UIHandle h={};
	h.p = ctrl;
	auto hchart = D2DCreateControlsWithScrollbar(h, FRectF(0,0,size), STAT_DEFAULT|STAT_AUTOFIT_CHILDWIN, NR(L"page1_td_chart_sc_wb",nm));

	auto ht = D2DCreateControls(hchart, FRectF(0,0,FSizeF(505*3, size.height )), STAT_DEFAULT, NONAME);



	D2DControls* ctrl2 = (D2DControls*)ht.p;


	for(int i = 0; i < _countof(cd); i++ )
	{
		auto p = CreateSolidStockChart2(ctrl2, cd[i], rc, nm );
		

		auto complete = [ctrl, p](InternetInfo* info)
		{
			ctrl->GetParent()->PostMessage(WM_MYAPP_INET_COMPLETE, (WPARAM)p, (LPARAM)info);
		};

		inetStockDataDownload( cd[i], complete );


		if ( i == 2 )
			rc = FRectF(0,305,sz);
		else
			rc.Offset(505,0);
	}

	auto pt = rc.RightBottom();
	//D2DSetRect(ht, FRectF(0,0, pt.x-505, pt.y ));



	D2DSendMessage(hchart, WM_D2D_SET_SIZE, 3,0);


	return nullptr;

}




//
//
//void WriteData( LPCWSTR fnm, IStream* psm)
//{
//	_ASSERT(psm);
//    auto h = ::CreateFile(fnm, GENERIC_WRITE,0,nullptr,CREATE_ALWAYS,0,nullptr);
//
//	if ( h != INVALID_HANDLE_VALUE )
//	{
//		char cb[256];
//		psm->Seek({0},STREAM_SEEK_SET, nullptr);
//
//		ULONG len;
//		while ( S_OK == psm->Read(cb,256,&len) && len > 0)
//		{
//			DWORD d;
//		    ::WriteFile(h, cb, len, &d, 0 );
//		}
//		::CloseHandle(h);
//	}
//}
//
//
//
//bool ReadData( LPCWSTR fnm, IStream** psm)
//{
//	ComPTR<IStream> sm;
//	HRESULT hr = ::CreateStreamOnHGlobal(NULL,TRUE, &sm);
//	auto h = ::CreateFile(fnm, GENERIC_READ,0,nullptr,OPEN_EXISTING,0,nullptr);
//	if ( h != INVALID_HANDLE_VALUE )
//	{
//		char cb[256];
//		DWORD len;
//		while( ::ReadFile(h,cb,256, &len,nullptr) && len > 0 )
//		{
//			sm->Write(cb, len, nullptr);
//		}
//		::CloseHandle(h);
//
//		sm.AddRef();
//		*psm = sm;
//		return true;
//	}
//	return false;
//}