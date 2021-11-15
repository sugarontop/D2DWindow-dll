#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"
#include "yahoo.h"
#include "D2DTabControls.h"

using namespace V6;

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



}
	

void yahoo_finance::Draw(D2DContext& cxt)
{

	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();


	cxt.DFillRect(rc_,theWhite);

	mat.Offset(rc_);

	D2DControls::Draw(cxt);

	//if ( info_ )
	//{
	//	WCHAR cb[64];
	//	swprintf_s(cb, L"result=%d", info_->result);


	//	if ( info_->result == 200 && y1_.empty() )
	//	{
	//		ConvCsv(info_->pstream);
	//	}
	//	if ( !y1_.empty())
	//	{
	//		for(auto y : y1_)
	//		{
	//			swprintf_s(cb,L"%f", y);

	//			FRectF rc(0,0,200,20);
	//			(*cxt)->DrawText(cb, wcslen(cb), cxt.tsf_text_format_, rc, cxt.black_);

	//			mat.Offset(0,20.0f);

	//		}


	//	}
	//}


	mat.PopTransform();

}

void yahoo_finance::InetComplete(InternetInfo* )
{
	ConvCsv(info_->pstream);

	this->parent_window_->cxt.Redraw();

}
void yahoo_finance::StartDownload()
{
	if ( info_ == nullptr )
	{
		info_ = CreateInternetInfo();

		info_->bGet = true;
		//info_->url = ::SysAllocString(L"https://query1.finance.yahoo.com/v7/finance/download/XLF?period1=1605085785&period2=1636621785&interval=1d&events=history&includeAdjustedClose=true");
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

	std::vector<float> adj_values;
	std::vector<std::string> dates;

	int i = 0;
	for(auto& r : rows )
	{
		auto cols = SplitEx(r.c_str(), ",");

		if (i++ > 0)
		{
			auto kstr = r.c_str();
			adj_values.push_back( myatof(kstr, cols[5] ));
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




	auto rc = parent_control_->GetRect();
	swprintf_s(cb,L" rc = (%f, %f, %f, %f)", rc.left,rc.top, rc.right, rc.bottom); 
	cxt.DText(FPointF(0,100), cb, theBlack);


	auto info = finance_->info_;


	if ( info )
	{
		if ( info->result == 200 )
		{
			auto ar = finance_->y1_;

			FRectF rc;
			float x = 0;
			for(auto y : ar )
			{
				y = 500 - y*5;
				rc = FRectF(x, y, x+3, y+3);
				(*cxt)->FillRectangle(rc, cxt.black_);
				x=rc.right;
			}

		}
	}



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
		case WM_LBUTTONDOWN :
		{
			MouseParam pm = *(MouseParam*)lParam;

			FPointF pt = mat_.DPtoLP(pm.pt);

			if ( rc_.PtInRect(pt))
			{
				finance_->StartDownload();


				hr = 1;
			}
		}
		break;


	}

	return hr;
}
void yahoo_chart::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rc_ = rc;




}
