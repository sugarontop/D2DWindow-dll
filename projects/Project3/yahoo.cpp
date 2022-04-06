#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"
#include "yahoo.h"
#include "D2DColor.h"


using namespace V6;

#define YAHOO 


struct PosLenStruct
{
	USHORT pos;
	USHORT len;
};
#pragma region YAHOO_TIME

#define _SECOND ((__int64) 10000000)
#define _MINUTE (60 * _SECOND)
#define _HOUR   (60 * _MINUTE)
#define _DAY    (24 * _HOUR)

ULONG yahoo_chart_period(int yyyy, int mm, int dd )
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

#pragma endregion

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

std::vector<PosLenStruct> SplitEx( LPCSTR str, LPCSTR split )
{
    std::vector<PosLenStruct> ar;
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

				PosLenStruct ss;
				ss.pos = (USHORT)si;
				ss.len = (USHORT)i-si;
                ar.push_back(ss);
                si = i + splen;
            }
        }       
    }
    return ar;
}

float myatof( LPCSTR str, const PosLenStruct& pos)
{
	LPSTR ss = (LPSTR)str;
	
	char temp = str[pos.pos + pos.len];

	ss[pos.pos + pos.len]=0;

	float a = (float)atof(ss+pos.pos);

	ss[pos.pos + pos.len] = temp;
	return a;
}

std::string mysubstr(LPCSTR str, const PosLenStruct& pos)
{
	return std::string( str+pos.pos, str+pos.pos+pos.len);
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


ULONG yahoo_chart_period(int yyyy, int mm, int dd );
bool ReadData( LPCWSTR fnm, IStream** psm);


void inetStockDataDownload(std::wstring cd, std::function<void(InternetInfo*)> complete_func)
{
	
	InternetInfo* info = CreateInternetInfo();

	info->bGet = true;

#ifdef YAHOO
	WCHAR cb[256];
		
	auto dds = yahoo_chart_period(2021,3,1);
	auto now = yahoo_chart_period(0,0,0);


	StringCbPrintf(cb,256,L"https://query1.finance.yahoo.com/v7/finance/download/%s?period1=%d&period2=%d&interval=1d&events=history&includeAdjustedClose=true",cd.c_str(),dds, now);
	info->url = ::SysAllocString(cb);
	info->complete = complete_func;					

	DWORD dw;
	::CreateThread(0,0,InetAsync, info, 0, &dw);

#else
	IStream* sm = nullptr;

	if ( ReadData( L"stock.bin", &sm) && sm)
	{
		info->url = nullptr;
		info->result = 200;
		info->pstream = sm;

		complete_func(info);
	}
#endif
}


struct ChartInfo
{
	float fmax, fmin;
	float step;
	FSizeF vsz;

};


static ChartInfo yahooDrawChart(D2DContext& cxt,FSizeF vsz, std::vector<Rousoku>& adj_values)
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

	float height = vsz.height;

	auto& ar = adj_values;

	float h =  height;
	float fmin,fmax;
	GetMaxMin(ar,&fmax,&fmin);

	float fd = fmax-fmin;
	float step = 0.8f*h / fd;

	FRectF rc;
	float x = 0;

	ComPTR<ID2D1SolidColorBrush> bred,bblue;
	(*cxt)->CreateSolidColorBrush(D2RGB(255,56,47),&bred);
	(*cxt)->CreateSolidColorBrush(D2RGB(76,201,145),&bblue);

	for(const auto& y : ar )
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

	/// /////////////////////////////////////////////////////////////////////////////////

	ChartInfo info;
	info.fmax = fmax;
	info.fmin = fmin;
	info.step = step;
	info.vsz = vsz;


	return info;
}
static void yahooDrawErrorMsgt(D2DContext& cxt, long result)
{
	WCHAR cb[256];
	StringCbPrintf(cb,256,L"internet err: %d", result );
	cxt.DText(FPointF(0,0), cb );


}


static void yahooDrawLine(D2DContext& cxt,ChartInfo chinfo, std::vector<Rousoku>& adj_values)
{
	float fstart,fend;
	float plotstep = CalcPlotSetpLine(chinfo.fmax, chinfo.fmin, &fstart, &fend);
			

	//(*cxt)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);


	float futosa = 1.0f; //1.0f/mat._11;

	ComPTR<ID2D1Factory> factory;
	ComPTR<ID2D1StrokeStyle> dotline_;
		
	(*cxt)->GetFactory(&factory);

	float dashes[] = {2.0f};

	factory->CreateStrokeStyle(
	D2D1::StrokeStyleProperties(
		D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_ROUND, D2D1_LINE_JOIN_MITER,
		10.0f,
		D2D1_DASH_STYLE_CUSTOM,
		0.0f),
		dashes, ARRAYSIZE(dashes),
		&dotline_
	);



	float h = chinfo.vsz.height;
	for(float yyy=fstart; yyy < fend; yyy+=plotstep )
	{			
		float ploty = h - (yyy-chinfo.fmin)*chinfo.step;


		FPointF pt1(0,ploty);
		FPointF pt2(chinfo.vsz.width,ploty);

		(*cxt)->DrawLine(pt1,pt2,cxt.black_, futosa, dotline_);
	}

	//(*cxt)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
}
static void yahooDrawString(D2DContext& cxt,ChartInfo chi, std::vector<Rousoku>& adj_values)
{

}


void yahooDraw(D2DContext& cxt, InternetInfo* info, FSizeF vsz, std::vector<Rousoku>& adj_values)
{
	if ( info )
	{
		if ( info->result == 200 )
		{
			D2DRectFilter f(cxt,FRectF(0,0, vsz));


			auto chi = yahooDrawChart(cxt,vsz,adj_values);
			
			yahooDrawLine(cxt,chi,adj_values);

			yahooDrawString(cxt,chi,adj_values);
		}
		else
		{
			yahooDrawErrorMsgt(cxt, info->result);			
		}
	}
}

bool CreateRousokuFromtStream(IStream* ism,std::vector<Rousoku>& adj_values, std::vector<std::string>& dates )
{
	ULONG len;
	LRESULT hr=0;
	char cb[64];

	std::stringstream sm;

	
	ism->Seek({0},STREAM_SEEK_SET, nullptr);

	do
	{
		hr = ism->Read(cb, 64, &len);

		if ( len > 0 )
			sm.write(cb,len);
	}
	while( hr ==0 && len > 0 );

	std::string src = sm.str();
	
	
	auto rows = Split( src.c_str(), "\n");

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
	
	return true;


}
