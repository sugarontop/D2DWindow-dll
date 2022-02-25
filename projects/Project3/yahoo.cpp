#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"
#include "yahoo.h"
//#include "D2DChildWindow.h"
#include "D2DColor.h"
using namespace V6;

#define ID_BUTTON 1911

#define YAHOO

std::wstring wstr(int v)
{
	WCHAR cb[256];
	swprintf_s(cb,256,L"%d", v);
	return cb;
}

yahoo_finance::~yahoo_finance()
{
	DeleteInternetInfo(info_);
}

void yahoo_finance::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	//InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rc_ = rc;
	parent_window_ = parent;
	parent_control_ = pacontrol;
	/////////////////////////////////////////////////////////////////////////

	UIHandle hctrls={};
	hctrls.p = pacontrol;

	auto dumy = D2DCreateXXXControls(hctrls,  FRectF(0,0,rc_.Size()),  STAT_DEFAULT, L"yahoo_tabcontrol_rap", 190);
	
	auto tabs = D2DCreateTabControls(dumy,  FRectF(0,0,rc_.Size()),  STAT_DEFAULT|STAT_SIMPLE, L"yahoo_tabcontrol", 191);
	
	// TAB : chart
	{
		auto h0 = D2DGetControlFromIdx(tabs, 0);

		auto tab1 = (D2DControls*)h0.p;

		FSizeF sz = rc_.Size();
		tab1->SetRect(FRectF(0,0,sz));
		
		D2DSendMessage(tabs,WM_D2D_SET_TEXT,0, (LPARAM)L"yahoo");




		UIHandle htab1={};
		htab1.p = tab1;

		auto child1 = D2DCreateControlsWithScrollbar(htab1,FRectF(0,0,rc_.Size().width,rc_.Size().height ),  STAT_DEFAULT|STAT_SIMPLE, L"c_with_scrollbar", 1912);


		D2DColor c1(0xEBEBEB);
		D2DSetColor(child1, c1,ColorF::Black,ColorF::Black);


		auto x = (D2DControls*)child1.p;

		auto chart = std::make_shared<yahoo_chart>();
		chart->CreateControl(parent_window_, x, FRectF(0,0,FSizeF(0,0)),  STAT_DEFAULT, L"yahoochart", 1910);
		x->Add(chart);
		


		chart->finance_ = this;
		chart_ = chart.get();

	}

	// TAB : data table
	{
		auto htab1 = D2DAddNewTab(tabs, L"table");// tabs->AddNewTab(L"table");

		auto child1 = D2DCreateControlsWithScrollbar(htab1,FRectF(0,0,rc_.Size().width,rc_.Size().height ),  STAT_DEFAULT|STAT_SIMPLE, L"c_with_scrollbar", 1913);
		auto x = (D2DControls*)child1.p;
	
		auto tbl = std::make_shared<yahoo_table>();
		tbl->CreateControl(parent_window_, x, FRectF(0,0,FSizeF(0,0)),  STAT_DEFAULT, L"yahootable", 1911);
		x->Add(tbl);
		tbl->finance_ = this;
		table_ = tbl.get();

	}


}
	

void yahoo_finance::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();


	//cxt.DFillRect(rc_,theWhite);

	//mat.Offset(rc_);

	D2DControls::Draw(cxt);

	mat.PopTransform();

}

void yahoo_finance::InetComplete(InternetInfo* )
{
	ConvCsv(info_->pstream);

	//this->parent_window_->cxt.Redraw();



	table_->Update( rc_.Width());


	InvalidateRect(parent_window_->GetHwnd(),NULL,FALSE);

}



void yahoo_finance::StartDownload(LPCWSTR cd1)
{
	if ( info_ )
	{
		DeleteInternetInfo(info_);
		info_ = nullptr;
	}
	
	
	if ( info_ == nullptr )
	{
		info_ = CreateInternetInfo();

		info_->bGet = true;

#ifdef YAHOO
		WCHAR cb[256];
		
		auto dds = yahoo_chart::Period(2021,3,1);
		auto now = yahoo_chart::Period(0,0,0);

		std::wstring cd= cd1;


		_snwprintf_s(cb,256,L"https://query1.finance.yahoo.com/v7/finance/download/%s?period1=%d&period2=%d&interval=1d&events=history&includeAdjustedClose=true",cd.c_str(),dds, now);
		info_->url = ::SysAllocString(cb);

		parent_window_->SendMessage(WM_D2D_TABCONTROL_TAB_NAME, (WPARAM)this, (LPARAM)cd.c_str());
		

#else

		info_->url = ::SysAllocString(L"https://192.168.10.65/zaimu/XLE.csv");
#endif
					
		info_->complete = std::bind(&yahoo_finance::InetComplete, this, _1);
					
		DWORD dw;
		::CreateThread(0,0,InetAsync, info_, 0, &dw);
	}					
}


LRESULT yahoo_finance::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	LRESULT hr = 0;

	switch(message )
	{
		case WM_LBUTTONDOWN:
		{
			int a = 0;
		}
		break;
		case WM_D2D_SET_SIZE:
		{
			FRectF& rc = *(FRectF*)lParam;

			
			//rc_.SetSize(rc.Size());

			int a = 0;

		}
		break;
		case WM_NOTIFY:
		{
			D2DNMHDR h;
			if ( wParam == ID_FLOATING_MENU )
			{
				h = *(D2DNMHDR*)lParam;

				auto k = (D2DControl*)h.sender_parent;

				if ( k->GetID() == 1910 )
				{
				
					int idx = h.prm1;

					if ( idx == 2 )
					{
						/*auto ptr = parent_window_->name_map_[L"aaa"];

						auto ptr1 = dynamic_cast<D2DControls_with_Scrollbar*>(ptr);

						auto ncontrols = ptr1->GetMainControls();


						this->SetNewParent(ncontrols);*/



						hr = 1;
					}
					else if ( idx == 3 && h.sender_parent == chart_ )
					{
						auto ctrls = GetParentControls();

						//if (ctrls) 
						//{
						//	//auto moveobj = std::make_shared<D2DControls_with_Move>();												
						//	//moveobj->CreateControl(parent_window_, ctrls, rc_, STAT_DEFAULT, NONAME);
						//	//ctrls->Add(moveobj);



						//	auto hmoveobj = D2DCreateControlsWithMove(ctrls,rc_, STAT_DEFAULT, NONAME);

						//	moveobj->prv_controls_ = this->GetParentControls();
						//	moveobj->target_ = this;
						//
						//	this->SetNewParent(moveobj.get());
						//}


						
						

						hr = 1;
					}

					
				}
			}
			

		}
		break;


	}


	if ( hr == 0 )
		hr = D2DControls::WndProc(b,message,wParam,lParam);

	return hr;
}

#pragma region SplitInet

std::vector<std::string> Split( LPCSTR str, LPCSTR split )
{
    std::vector<std::string> ar;
    int splen = lstrlenA(split);
    int len = lstrlenA(str);
    _ASSERT( 0 < splen && splen <= 2  );

    int si = 0;
    for( int i = 0; i <= len; i++ )
    {
        if ( str[i] == split[0] || (i == len && 0 < len) )
        {
            if (splen == 1 || (splen == 2 && (i == len || str[i+1] == split[1] )) )
            {
                std::string s( &str[si], i-si );
                ar.push_back( std::string(s.c_str()));
                si = i + splen;
            }
        }       
    }
    return ar;
}

struct poslen
{
	USHORT pos;
	USHORT len;
};

std::vector<poslen> SplitEx( LPCSTR str, LPCSTR split )
{
    std::vector<poslen> ar;
    int splen = lstrlenA(split);
    int len = lstrlenA(str);
    _ASSERT( 0 < splen && splen <= 2  );

    int si = 0;
    for( int i = 0; i <= len; i++ )
    {
        if ( str[i] == split[0] || (i == len && 0 < len) )
        {
            if (splen == 1 || (splen == 2 && (i == len || str[i+1] == split[1] )) )
            {
                std::string s( &str[si], i-si );

				poslen ss;
				ss.pos = (USHORT)si;
				ss.len = (USHORT)i-si;
                ar.push_back(ss);
                si = i + splen;
            }
        }       
    }
    return ar;
}

float myatof( LPCSTR str, poslen pos)
{
	LPSTR ss = (LPSTR)str;
	
	char temp = str[pos.pos + pos.len];

	ss[pos.pos + pos.len]=0;

	float a = (float)atof(ss+pos.pos);

	ss[pos.pos + pos.len] = temp;
	return a;
}

std::string mysubstr(LPCSTR str, poslen pos)
{
	return std::string( str+pos.pos, str+pos.pos+pos.len);
}


bool yahoo_finance::ConvCsv(IStream* ism)
{

	ULONG len;
	LRESULT hr=0;
	char cb[64];

	std::stringstream sm;

	do
	{
		hr = ism->Read(cb, 64, &len);

		if ( len > 0 )
			sm.write(cb,len);
	}
	while( hr ==0 && len > 0 );

	std::string src = sm.str();
	
	
	auto rows = Split( src.c_str(), "\n");

	std::vector<Rousoku> adj_values;
	std::vector<std::string> dates;

	int i = 0;
	for(auto& r : rows )
	{
		auto cols = SplitEx(r.c_str(), ",");

		if (i++ > 0)
		{
			auto kstr = r.c_str();

			Rousoku r;
			r.ystart = myatof(kstr, cols[1]);
			r.yend = myatof(kstr, cols[4]);
			r.ymin = myatof(kstr, cols[3]);
			r.ymax = myatof(kstr, cols[2]);

			adj_values.push_back(r);
			dates.push_back(mysubstr(kstr, cols[0]));
		}
	}

	
	dates_ = dates;
	y1_ = adj_values;

	return true;


}
#pragma endregion
/////////////////////////////////////////////////////////////////////////



float CalcPlotSetpLine( float fmax, float fmin, float* calc_fstart, float* calc_fend)
{
	float step =  (fmax-fmin)/10;

	*calc_fstart = floor(fmin-step);
	*calc_fend = floor(fmax+step+1);

	step =  floor((*calc_fend-*calc_fstart)/10);

	return max(1.0f,step);
}


yahoo_chart::yahoo_chart()
{

}
void yahoo_chart::SetParentRect(FRectF* prc)
{
	prc_ = prc;

	rc_.SetSize(prc->GetSize());

}
void yahoo_chart::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();


	mat.Offset(rc_);

//	WCHAR cb[256];
	auto rcparent = parent_control_->GetRect();
//	swprintf_s(cb,L" pt= (%f, %f), rc_=(%f, %f, %f, %f)", mouse_pt_.x, mouse_pt_.y, rc_.left, rc_.top, rc_.right, rc_.bottom ); //.left,rcparent.top, rcparent.right, rcparent.bottom); 
//	cxt.DText(FPointF(0,80), cb, theBlack);

	auto GetMaxMin = [](std::vector<Rousoku>& ar, float* pfmax, float* pfmin)
	{
		float fmax=0, fmin = 9999999;

		for(auto& y : ar )
		{
			fmax = max(y.ymax,fmax);
			fmin = min(y.ymin,fmin);
		}
		*pfmax = fmax;
		*pfmin = fmin;

	};


	if ( finance_ && finance_->info_ )
	{
		if ( finance_->info_->result == 200 )
		{
			auto& ar = finance_->y1_;


			float h =  rcparent.Height();
			float fmin,fmax;
			GetMaxMin(ar,&fmax,&fmin);

			float fd = fmax-fmin;
			float step = 0.8f*h / fd;

			FRectF rc;
			float x = 0;

			ComPTR<ID2D1SolidColorBrush> bred,bblue;
			(*cxt)->CreateSolidColorBrush(D2RGB(255,56,47),&bred);
			(*cxt)->CreateSolidColorBrush(D2RGB(76,201,145),&bblue);

			for(auto y : ar )
			{				
				if ( y.ystart < y.yend )
				{				
					float y1max = h- (y.yend-fmin)*step;
					float y1min = h- (y.ystart-fmin)*step;
					rc = FRectF(x, y1min, x+3, y1max);

					auto br = bblue; 
					auto br1 = cxt.white_;

					y1max = h- (y.ymax-fmin)*step;
					y1min = h- (y.ymin-fmin)*step;

					(*cxt)->DrawRectangle(rc, br);
					//(*cxt)->FillRectangle(rc, br1);

					auto rc2 = FRectF(x+1, y1min, x+2, y1max);

					(*cxt)->FillRectangle(rc2, bblue);
					(*cxt)->FillRectangle(rc, br1);
				}
				else
				{				
					float y1max = h- (y.ystart-fmin)*step;
					float y1min = h- (y.yend-fmin)*step;
					rc = FRectF(x, y1min, x+3, y1max);

					auto br = bred; 
					auto br1 = bred;

					y1max = h- (y.ymax-fmin)*step;
					y1min = h- (y.ymin-fmin)*step;

					(*cxt)->DrawRectangle(rc, br);
					(*cxt)->FillRectangle(rc, br1);

					rc = FRectF(x+1, y1min, x+2, y1max);

					(*cxt)->FillRectangle(rc, br1);
				}

				x += 4;
			}

			float fstart,fend;
			float plotstep = CalcPlotSetpLine(fmax, fmin, &fstart, &fend);


			

			(*cxt)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);


			float futosa = 1.0f/mat._11;

			for(float yyy=fstart; yyy < fend; yyy+=plotstep )
			{			
				float ploty = h - (yyy-fmin)*step;


				FPointF pt1(0,ploty);
				FPointF pt2(rc_.right-rc_.left,ploty);

				(*cxt)->DrawLine(pt1,pt2,cxt.black_, futosa, dotline_);
			}

			(*cxt)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

		}
		else
		{
			WCHAR cb[256];

			std::wstring str = L"internet err:" + wstr(finance_->info_->result);
			cxt.DText(FPointF(0,0), str );

		}
	}
	
	
	this->InnerDraw(cxt);


	mat.PopTransform();
}
LRESULT yahoo_chart::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT hr = 0;

	switch( message )
	{
		case WM_D2D_SET_SIZE:
		{
			FRectF& rc = *(FRectF*)lParam;
		}
		break;
		case WM_D2D_SET_SIZE_SIZE:
		{
			FSizeF& sz = *(FSizeF*)lParam;
			rc_.SetSize(sz);

			D2DControls::WndProc(b,message,wParam,(LPARAM)&sz);
			return 0;

		}
		break;
		case WM_RBUTTONDOWN :
		{
			MouseParam& pm = *(MouseParam*)lParam;
			FPointF pt = mat_.DPtoLP(pm.pt);
			mouse_pt_ = pt;

			if (rc_.PtInRect(pt))
			{				
				UIHandleWin hwin;
				hwin.p = this->GetParent();
				MenuItem items[5];

				items[0].id=1; items[0].str=L"test1"; items[0].typ = 0;
				items[1].id=2; items[1].str=L"test2"; items[1].typ = 1;
				items[2].id=3; items[2].str=L"test3"; items[2].typ = 0;
				items[3].id=4; items[3].str=L"move"; items[3].typ = 0;
				items[4].id=5; items[4].str=L"test5"; items[4].typ = 0;

							

				FRectF xrc(pm.pt, FSizeF(200,60));

				D2DFloatingMenu(hwin, xrc, this, items, 5);

				hr = 1;
			}
		}
		break;
		case WM_NOTIFY:
		{
			D2DNMHDR h;

			if ( wParam == ID_BUTTON )
			{
				
				FRectF rc(100,100,FSizeF(400,150));


				UIHandleWin hwin={};
				hwin.p = parent_window_;

			
				//D2DMessageBox(hwin, rc, L"sample",  L"this is **** message **** ");

				//std::vector<ControlMapItem> ar;
				//parent_window_->ListUp(ar);

				hr = 1;
			}
			else if ( wParam == ID_FLOATING_MENU )
			{
				h = *(D2DNMHDR*)lParam;


				if ( h.sender_parent == this )
				{
				
					int idx = h.prm1;

					if ( idx == 1 )
					{
						D2DSetText(txt_cd_,L"SPY");
						finance_->StartDownload(L"SPY");
					}

					hr = 1;
				}
			}
			else if ( wParam == 3000)
			{
				h = *(D2DNMHDR*)lParam;
				if ( h.sender_parent == this )
				{
					
					BSTR cd = D2DGetText(txt_cd_, true);

					if( wcslen(cd) > 0 )
						finance_->StartDownload(cd);
					else
						finance_->StartDownload(nullptr);


					hr =1;
				}
			}


		}
		break;
		case WM_LBUTTONDOWN:
		{
			int a = 0;

		}
		break;
	}

	if ( hr == 0 )
		//hr = InnerWndProc(b,message,wParam,lParam);
		hr = D2DControls::DefWndProc(b, message,wParam,lParam);


	return hr;
}
void yahoo_chart::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rc_ = rc;

	// input
	{
		UIHandleWin hwin={};
		hwin.p = parent_window_;
		UIHandle hs={};
		hs.p = this;

		auto txt = D2DCreateTextbox( hs, FRectF(10,20,FSizeF(100,20)), false, STAT_DEFAULT, L"input cd", 3001 );

		D2DSendMessage(txt,WM_D2D_SET_TEXT,0,(LPARAM)L"SPY"); // SPY:SP500 ETF, QQQ:NASDAQ 100 ETF

		auto btn = D2DCreateButton( hs, FRectF(120,20,FSizeF(100,20)), STAT_DEFAULT, L"seek", 3000 );

		txt_cd_ = txt;
	}


	float dashes[] = {3.0f, 4.0f };
	{
		auto& cxt = parent->GetContext();
		ComPTR<ID2D1Factory> fac;
		(*cxt)->GetFactory(&fac);
			fac->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_ROUND,
				D2D1_LINE_JOIN_MITER,
				10.0f,
				D2D1_DASH_STYLE_CUSTOM,
				0.0f),
			dashes,
			ARRAYSIZE(dashes),
			&dotline_
			);
	}



}

#define _SECOND ((__int64) 10000000)
#define _MINUTE (60 * _SECOND)
#define _HOUR   (60 * _MINUTE)
#define _DAY    (24 * _HOUR)

ULONG yahoo_chart::Period(int yyyy, int mm, int dd )
{
	if ( yyyy < 1970 )
	{
		struct tm tm_now;
		__time64_t long_time;
		_time64( &long_time );		
		localtime_s(&tm_now, &long_time );

		yyyy = 1900+ tm_now.tm_year;
		mm = 1+ tm_now.tm_mon;
		dd = tm_now.tm_mday;
	}


	const ULONG oneday = 86400; // 86400sec

	SYSTEMTIME st={};
	SYSTEMTIME st2={};

	st.wYear=1970;st.wMonth=1;st.wDay=1;

	st2.wYear=yyyy;st2.wMonth=mm;st2.wDay=dd;

	FILETIME ft,ft2;
	SystemTimeToFileTime(&st,&ft);
	SystemTimeToFileTime(&st2,&ft2);

	ULONGLONG number = (((ULONGLONG)ft.dwHighDateTime) << 32) + ft.dwLowDateTime;
	ULONGLONG number2 = (((ULONGLONG)ft2.dwHighDateTime) << 32) + ft2.dwLowDateTime;

	auto distance = (number2-number)/_DAY;
	auto ret = distance*oneday;


	return (ULONG)ret;

}

/////////////////////////////////////////////////////////////////////////
void yahoo_table::Draw(D2DContext& cxt)
{
	
	D2DMatrix mat(*cxt);
	mat_ = mat.PushTransform();
	
	auto& ar = finance_->y1_;


	float h = 22.0f;

	FRectF rc[] = { FRectF(0,0,FSizeF(100,h)),FRectF(100,0,FSizeF(100,h)),FRectF(200,0,FSizeF(100,h)),FRectF(300,0,FSizeF(100,h)) };

	WCHAR cb[256];
	float row = 0;
	
	size_t len = min(30,finance_->y1_.size());

	for(UINT i=0; i < len; i++)
	{
		auto st = ar[i];

		cxt.DrawBlackLine(rc[0]); _snwprintf_s(cb,256,L"%f", st.ystart);(*cxt)->DrawText(cb, wcslen(cb),cxt.textformat_,rc[0],cxt.black_);
		cxt.DrawBlackLine(rc[1]); _snwprintf_s(cb,256,L"%f", st.ymax);(*cxt)->DrawText(cb, wcslen(cb),cxt.textformat_,rc[1],cxt.black_);
		cxt.DrawBlackLine(rc[2]); _snwprintf_s(cb,256,L"%f", st.ymin);(*cxt)->DrawText(cb, wcslen(cb),cxt.textformat_,rc[2],cxt.black_);
		cxt.DrawBlackLine(rc[3]); _snwprintf_s(cb,256,L"%f", st.yend);(*cxt)->DrawText(cb, wcslen(cb),cxt.textformat_,rc[3],cxt.black_);


		mat.Offset(0,h);

	}

	



	mat.PopTransform();
}

LRESULT yahoo_table::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{

	LRESULT hr = 0;

	switch( message )
	{
		case WM_D2D_SET_SIZE:
		{
			FRectF& rc = *(FRectF*)lParam;
		}
		break;
		case WM_D2D_SET_SIZE_SIZE:
		{
			FSizeF& sz = *(FSizeF*)lParam;
			rc_.SetSize(sz);

			
			return 0;

		}
		break;
	}

	return hr;

}
void yahoo_table::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rc_ = rc;
}
void yahoo_table::Update(float cx)
{

	FRectF rc5(rc_);
	rc5.SetWidth(cx);
	rc5.SetHeight(262*22.0f);

	AppBase b;
	//finance_->sc_control_->WndProc(b,WM_D2D_SET_SIZE,2,(LPARAM)&rc5);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void yahoo_sample::Draw(D2DContext& cxt)
{
	//auto blk1 = D2RGB(37,37,38); // content
	//auto blk0 = D2RGB(61,61,61); // line
	//auto blk2 = D2RGB(46,46,46); // separator
	//auto line = D2RGB(113,96,232);


	auto blk1 = D2RGB(255,255,255);
	auto blk0 = D2RGB(237,237,238);
	
	auto blk2 = D2RGB(246,246,246);
	auto line = D2RGB(113,96,232);

	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();

	auto rc1 = rc_;
	rc1.Offset(0,-60);

	cxt.DFillRect(rc1, blk2);

	auto rc = rc_;

	cxt.DFillRect(rc, blk0);
	rc.Inflate(-1,-1);
	cxt.DFillRect(rc, blk1);
	
	rc.bottom = rc.top+5;
	cxt.DFillRect(rc, line);


	mat.Offset( rc_ );
	InnerDraw(cxt);

	mat.PopTransform();
}

LRESULT yahoo_sample::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT hr = 0;

	switch( message )
	{
		case WM_D2D_SET_SIZE:
		{
			FRectF& rc = *(FRectF*)lParam;
		}
		break;
		case WM_D2D_SET_SIZE_SIZE:
		{
			FSizeF& sz = *(FSizeF*)lParam;
			rc_.SetSize(sz);

		}
		break;
	}


	hr = D2DControls::WndProc(b,message,wParam,lParam);


	return hr;

}
void yahoo_sample::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rc_ = rc;


	UIHandleWin  win={}; win.p = parent;
	UIHandle ctrls={}; ctrls.p=this;

	auto ct = D2DCreateTextbox(ctrls, FRectF(1,5,FSizeF(400,500)), true, STAT_DEFAULT, L"NVIDIA" );

}
	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void inetStockDataDownload(std::wstring cd, std::function<void(InternetInfo*)> compfunc)
{
	
	InternetInfo* info_ = CreateInternetInfo();

	info_->bGet = true;

#ifdef YAHOO
	WCHAR cb[256];
		
	auto dds = yahoo_chart::Period(2021,3,1);
	auto now = yahoo_chart::Period(0,0,0);


	_snwprintf_s(cb,256,L"https://query1.finance.yahoo.com/v7/finance/download/%s?period1=%d&period2=%d&interval=1d&events=history&includeAdjustedClose=true",cd.c_str(),dds, now);
	info_->url = ::SysAllocString(cb);

#else

	info_->url = ::SysAllocString(L"https://192.168.10.65/zaimu/XLE.csv");
#endif
					
	//info_->complete = std::bind(&yahoo_finance::InetComplete, this, _1);
	info_->complete = compfunc;					

	DWORD dw;
	::CreateThread(0,0,InetAsync, info_, 0, &dw);
						
}


void yahooDraw(D2DContext& cxt, InternetInfo* info, float height, std::vector<Rousoku>& adj_values)
{
	auto GetMaxMin = [](std::vector<Rousoku>& ar, float* pfmax, float* pfmin)
	{
		float fmax=0, fmin = 9999999;

		for(auto& y : ar )
		{
			fmax = max(y.ymax,fmax);
			fmin = min(y.ymin,fmin);
		}
		*pfmax = fmax;
		*pfmin = fmin;

	};


	if ( info )
	{
		if ( info->result == 200 )
		{
			auto& ar = adj_values;
			

			float h =  height; //rcparent.Height();
			float fmin,fmax;
			GetMaxMin(ar,&fmax,&fmin);

			float fd = fmax-fmin;
			float step = 0.8f*h / fd;

			FRectF rc;
			float x = 0;

			ComPTR<ID2D1SolidColorBrush> bred,bblue;
			(*cxt)->CreateSolidColorBrush(D2RGB(255,56,47),&bred);
			(*cxt)->CreateSolidColorBrush(D2RGB(76,201,145),&bblue);

			for(auto y : ar )
			{				
				if ( y.ystart < y.yend )
				{				
					float y1max = h- (y.yend-fmin)*step;
					float y1min = h- (y.ystart-fmin)*step;
					rc = FRectF(x, y1min, x+3, y1max);

					auto br = bblue; 
					auto br1 = cxt.white_;

					y1max = h- (y.ymax-fmin)*step;
					y1min = h- (y.ymin-fmin)*step;

					(*cxt)->DrawRectangle(rc, br);
					//(*cxt)->FillRectangle(rc, br1);

					auto rc2 = FRectF(x+1, y1min, x+2, y1max);

					(*cxt)->FillRectangle(rc2, bblue);
					(*cxt)->FillRectangle(rc, br1);
				}
				else
				{				
					float y1max = h- (y.ystart-fmin)*step;
					float y1min = h- (y.yend-fmin)*step;
					rc = FRectF(x, y1min, x+3, y1max);

					auto br = bred; 
					auto br1 = bred;

					y1max = h- (y.ymax-fmin)*step;
					y1min = h- (y.ymin-fmin)*step;

					(*cxt)->DrawRectangle(rc, br);
					(*cxt)->FillRectangle(rc, br1);

					rc = FRectF(x+1, y1min, x+2, y1max);

					(*cxt)->FillRectangle(rc, br1);
				}

				x += 4;
			}

			float fstart,fend;
			float plotstep = CalcPlotSetpLine(fmax, fmin, &fstart, &fend);


			

			/*(*cxt)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);


			float futosa = 1.0f/mat._11;

			for(float yyy=fstart; yyy < fend; yyy+=plotstep )
			{			
				float ploty = h - (yyy-fmin)*step;


				FPointF pt1(0,ploty);
				FPointF pt2(rc_.right-rc_.left,ploty);

				(*cxt)->DrawLine(pt1,pt2,cxt.black_, futosa, dotline_);
			}

			(*cxt)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);*/

		}
		else
		{
			WCHAR cb[256];

			std::wstring str = L"internet err:" + wstr(info->result);
			cxt.DText(FPointF(0,0), str );

		}
	}
}

bool ConvCsv(IStream* ism,std::vector<Rousoku>& adj_values, std::vector<std::string>& dates )
{

	ULONG len;
	LRESULT hr=0;
	char cb[64];

	std::stringstream sm;

	do
	{
		hr = ism->Read(cb, 64, &len);

		if ( len > 0 )
			sm.write(cb,len);
	}
	while( hr ==0 && len > 0 );

	std::string src = sm.str();
	
	
	auto rows = Split( src.c_str(), "\n");

	//std::vector<Rousoku> adj_values;
	//std::vector<std::string> dates;

	int i = 0;
	for(auto& r : rows )
	{
		auto cols = SplitEx(r.c_str(), ",");

		if (i++ > 0)
		{
			auto kstr = r.c_str();

			Rousoku r;
			r.ystart = myatof(kstr, cols[1]);
			r.yend = myatof(kstr, cols[4]);
			r.ymin = myatof(kstr, cols[3]);
			r.ymax = myatof(kstr, cols[2]);

			adj_values.push_back(r);
			dates.push_back(mysubstr(kstr, cols[0]));
		}
	}
	
	//dates_ = dates;
	//y1_ = adj_values;

	return true;


}