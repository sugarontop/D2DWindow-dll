#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"
#include "yahoo.h"
#include "D2DTabControls.h"
#include "D2DControls_with_Scrollbar.h"

using namespace V6;

#define ID_BUTTON 1911

yahoo_finance::~yahoo_finance()
{
	DeleteInternetInfo(info_);
}

void yahoo_finance::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rc_ = rc;

	/////////////////////////////////////////////////////////////////////////
	auto tabs = std::make_shared<D2DTabControls>();
	tabs->CreateControl(parent_window_, this, FRectF(0,0,rc_.Size()),  STAT_DEFAULT, L"yahoo_tabcontrol", 191);

	tabs->SizeFix();

	this->Add(tabs);

	auto tab1 = dynamic_cast<D2DControls*>(tabs->GetControlFromIdx(0));

	FSizeF sz = rc_.Size();

	tab1->SetRect(FRectF(0,0,sz));

	auto chart = std::make_shared<yahoo_chart>();
	chart->CreateControl(parent_window_, tab1, FRectF(0,0,FSizeF(2222,1000)),  STAT_DEFAULT, L"yahoochart", 1910);
	tab1->Add(chart);


	//tab1->Add(xxxxx);

	chart->finance_ = this;

	UIHandleWin w={};
	w.p = parent;
	UIHandle c={};
	c.p = this;

	D2DCreateButton(w,c , FRectF(500,0,FSizeF(150,20)), STAT_DEFAULT, L"msgbox", ID_BUTTON );


}
	

void yahoo_finance::Draw(D2DContext& cxt)
{
TRACE( L"yf=%x name= %s\n", this, parent_control_->GetName().c_str());


	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();


	cxt.DFillRect(rc_,theWhite);

	//mat.Offset(rc_);

	D2DControls::Draw(cxt);

	mat.PopTransform();

}

void yahoo_finance::InetComplete(InternetInfo* )
{
	ConvCsv(info_->pstream);

	//this->parent_window_->cxt.Redraw();

	InvalidateRect(parent_window_->GetHwnd(),NULL,FALSE);

}
void yahoo_finance::StartDownload()
{
	if ( info_ == nullptr )
	{
		info_ = CreateInternetInfo();

		info_->bGet = true;
		//info_->url = ::SysAllocString(L"https://query1.finance.yahoo.com/v7/finance/download/AAPL?period1=1605085785&period2=1636621785&interval=1d&events=history&includeAdjustedClose=true");
		info_->url = ::SysAllocString(L"https://192.168.10.65/zaimu/XLE.csv");
					
		info_->complete = std::bind(&yahoo_finance::InetComplete, this, _1);
					
		DWORD dw;
		::CreateThread(0,0,InetAsync, info_, 0, &dw);
	}					
}


HRESULT yahoo_finance::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	HRESULT hr = 0;

	switch(message )
	{
		case WM_LBUTTONDOWN:
		{
			
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
					else if ( idx == 3 )
					{
						auto ctrls = dynamic_cast<D2DControls*>(parent_window_->name_map_[L"MySquarePaper"]);

						auto moveobj = std::make_shared<D2DControls_with_Move>();
												
						moveobj->CreateControl(parent_window_, ctrls, rc_, STAT_DEFAULT, NONAME);

						ctrls->Add(moveobj);

						moveobj->prv_controls_ = this->GetParentControls();
						moveobj->target_ = this;
						
						this->SetNewParent(moveobj.get());

						

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
	HRESULT hr=0;
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
yahoo_chart::yahoo_chart()
{

}

void yahoo_chart::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();

	//cxt.DFillRect(rc_,theGray1);

	mat.Offset(rc_);


	WCHAR cb[256];
	swprintf_s(cb,L" rc_ = (%f, %f, %f, %f)", rc_.left,rc_.top, rc_.right, rc_.bottom); 
	cxt.DText(FPointF(0,50), cb, theBlack);

	auto rcparent = parent_control_->GetRect();
	swprintf_s(cb,L" rc = (%f, %f, %f, %f)", rcparent.left,rcparent.top, rcparent.right, rcparent.bottom); 
	cxt.DText(FPointF(0,80), cb, theBlack);


	auto info = finance_->info_;


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
		}
	}
	D2DControls::Draw(cxt);


	mat.PopTransform();
}
HRESULT yahoo_chart::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = 0;

	switch( message )
	{
		case WM_D2D_SET_SIZE:
		{
			FRectF& rc = *(FRectF*)lParam;
		}
		break;
		case WM_RBUTTONDOWN :
		{
			MouseParam pm = *(MouseParam*)lParam;

			FPointF pt = mat_.DPtoLP(pm.pt);
			auto rc = GetParentControls()->GetRect();
			_ASSERT(rc.top == 0 && rc.left == 0);

			if ( rc.PtInRect(pt))
			{
				
				UIHandleWin hwin;
				hwin.p = this->GetParent();
				MenuItem items[5];

				items[0].id=1; items[0].str=L"test1"; items[0].typ = 0;
				items[1].id=2; items[1].str=L"test2"; items[1].typ = 1;
				items[2].id=3; items[2].str=L"test3"; items[2].typ = 0;
				items[3].id=4; items[3].str=L"test4"; items[3].typ = 0;
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

				//rc = mat_.LPtoDP(rc);


				D2DMessageBox(hwin, rc, L"sample",  L"this is **** message **** ");



				hr = 1;
			}
			else if ( wParam == ID_FLOATING_MENU )
			{
				h = *(D2DNMHDR*)lParam;


				if ( h.sender_parent == this )
				{
				
					int idx = h.prm1;

					if ( idx == 1 )
						finance_->StartDownload();

					hr = 1;
				}
			}



		}
		break;
	}

	if ( hr == 0 )
		hr = D2DControls::WndProc(b,message,wParam,lParam);


	return hr;
}
void yahoo_chart::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rc_ = rc;


	//auto btn = std::make_shared<D2DButton>();
	//btn->CreateControl(parent,this, FRectF(100,10,FSizeF(150,20)), STAT_DEFAULT, NONAME );


}