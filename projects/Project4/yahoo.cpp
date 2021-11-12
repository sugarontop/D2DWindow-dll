#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"


#include "yahoo.h"

using namespace V6;

yahoo_finance::~yahoo_finance()
{
	DeleteInternetInfo(info_);
}

void yahoo_finance::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rc_ = rc;
}
	

void yahoo_finance::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();


	cxt.DFillRect(rc_,theWhite);

	mat.Offset(rc_);


	if ( info_ )
	{
		WCHAR cb[64];
		swprintf_s(cb, L"result=%d", info_->result);


		if ( info_->result == 200 && y1_.empty() )
		{
			ConvCsv(info_->pstream);
		}
		if ( !y1_.empty())
		{
			for(auto y : y1_)
			{
				swprintf_s(cb,L"%f", y);

				FRectF rc(0,0,200,20);
				(*cxt)->DrawText(cb, wcslen(cb), cxt.tsf_text_format_, rc, cxt.black_);

				mat.Offset(0,20.0f);

			}


		}


		//FRectF rc(10,10,200,200);
		//(*cxt)->DrawText(cb, wcslen(cb), cxt.tsf_text_format_, rc, cxt.black_);


	}


	mat.PopTransform();

}
HRESULT yahoo_finance::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	HRESULT hr = 0;

	switch(message )
	{
		case WM_LBUTTONDOWN:
		{
			//if ( info_ )
			{
				MouseParam pm = *(MouseParam*)lParam;

				auto pt = mat_.DPtoLP(pm.pt);

				if ( rc_.PtInRect(pt))
				{
					info_ = CreateInternetInfo();

					info_->bGet = true;
					//info_->url = ::SysAllocString(L"https://query1.finance.yahoo.com/v7/finance/download/XLF?period1=1605085785&period2=1636621785&interval=1d&events=history&includeAdjustedClose=true");
					info_->url = ::SysAllocString(L"https://192.168.10.65/zaimu/XLE.csv");
					
					
					DWORD dw;
					::CreateThread(0,0,InetAsync, info_, 0, &dw);
					
					
				}
			}
			
		}
		break;



	}
	return hr;


}

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

	float a = atof(ss+pos.pos);

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