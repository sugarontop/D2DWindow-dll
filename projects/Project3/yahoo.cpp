#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"
#include "yahoo.h"
#include "D2DColor.h"
#include "sqlite3pp.h"


using namespace V6;

#define YAHOO 


// SQLite3 /////////////////////////////////////////////////////////////////////
#pragma comment (lib,"sqlite3")

bool DbUpdateDaillyData(std::vector<std::string>& days);
bool DbUpdateStockData( LPCSTR cd, std::vector<Rousoku>& stock);
bool DbReadStockData( LPCSTR cd, IStream** ppout);
///////////////////////////////////////////////////////////////////////////////



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

ULONG yahoo_unix_time(int yyyy, int mm, int dd )
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

float CalcPlotStepLine( float fmax, float fmin, float* calc_fstart, float* calc_fend)
{
	float step =  (fmax-fmin)/10;

	*calc_fstart = floor(fmin-step);
	*calc_fend = floor(fmax+step+1);

	step =  floor((*calc_fend-*calc_fstart)/10);


//	_ASSERT(step > 10 );

	int pw = (int)log10(step); // 端数をcut

	step = pow(10,pw);

	*calc_fstart = floor(fmin-step);
	*calc_fend = floor(fmax+step+1);


	int cn = (int)(*calc_fend- *calc_fstart) / step;

	while ( cn > 10 )
	{
		step = step * 2;
		
		*calc_fstart = floor(fmin-step);
		*calc_fend = floor(fmax+step+1);

		cn = (int)(*calc_fend- *calc_fstart) / step;
	}

	return max(1.0f,step);
}


ULONG yahoo_chart_period(int yyyy, int mm, int dd );
bool ReadData( LPCWSTR fnm, IStream** psm);

std::string W2A(std::wstring s);
std::wstring A2W(std::string s);

void inetStockDataDownload(std::wstring cd, std::function<void(InternetInfo*)> complete_func)
{
	
	InternetInfo* info = CreateInternetInfo();

	info->bGet = true;

#ifdef YAHOO
	WCHAR cb[256];
		
	auto dds = yahoo_unix_time(2021,4,1);
	auto now = yahoo_unix_time(0,0,0);


	StringCbPrintf(cb,256,L"https://query1.finance.yahoo.com/v7/finance/download/%s?period1=%d&period2=%d&interval=1d&events=history&includeAdjustedClose=true",cd.c_str(),dds, now);
	info->url = ::SysAllocString(cb);
	info->complete = complete_func;					

	DWORD dw;
	::CreateThread(0,0,InetAsync, info, 0, &dw);

#else
	//IStream* sm = nullptr;

	//if ( ReadData( L"stock.bin", &sm) && sm)
	//{
	//	info->url = nullptr;
	//	info->result = 200;
	//	info->pstream = sm;

	//	complete_func(info);
	//}

	IStream* sm2 = nullptr;
	if (DbReadStockData( W2A(cd).c_str(), &sm2))
	{
		info->url = nullptr;
		info->result = 200;
		info->pstream = sm2;

		complete_func(info);
	}


#endif
}




struct ChartInfo
{
	float fmax, fmin;
	float step;
	FSizeF vsz;
	WORD first_idx;
	WORD last_idx;


};


void CalcAverage(UINT idx,UINT period, std::vector<Rousoku>& ar);

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

	if ( adj_values[adj_values.size()-1].average[0] <= 0 )
	{		
		CalcAverage(0, 7, adj_values);
	}
	if ( adj_values[adj_values.size()-1].average[1] <= 0 )
	{		
		CalcAverage(1, 120, adj_values);
	}



	auto& ar = adj_values;
	const float Roudoku_width = 4.0f;

	float h =  height;
	float fmin,fmax;
	GetMaxMin(ar,&fmax,&fmin);

	float fd = fmax-fmin;
	float step = 0.8f*h / fd;

	FRectF rc(0,0,0,0);
	
	float x = vsz.width - ar.size()*Roudoku_width; // start point
	float first_x = -1;

	ComPTR<ID2D1SolidColorBrush> bred,bblue;
	(*cxt)->CreateSolidColorBrush(D2RGB(255,56,47),&bred);
	(*cxt)->CreateSolidColorBrush(D2RGB(76,201,145),&bblue);

	WORD k = 0, kcnt=0;

	std::function<float(float)> ffff;

	for(const auto& y : ar )
	{				
		if ( 0 <= x )
		{		
			if ( y.ystart < y.yend )
			{				
				// 上げ　で終了
				float y1max = h- (y.yend-fmin)*step;
				float y1min = h- (y.ystart-fmin)*step;
				rc = FRectF(x, y1min, x+3, y1max);

				auto br = bblue; 
				auto br1 = bblue;

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
				// 下げ　で終了
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

			
			
			for(int i = 0; i < 2; i++ )
			{
				float avg_ypos = h- (y.average[i]-fmin)*step;
				rc = FRectF(x+1, avg_ypos-1.0f, x+2, avg_ypos+1.0f);
				(*cxt)->FillRectangle(rc, cxt.black_);
			}

			


			kcnt++;
		}
		else
			k++;

		x += Roudoku_width;
	}

	/// /////////////////////////////////////////////////////////////////////////////////

	ChartInfo info;
	info.fmax = fmax;
	info.fmin = fmin;
	info.step = step;
	info.vsz = vsz;
	info.first_idx = k;
	info.last_idx = k + kcnt;

	return info;
}
static void yahooDrawErrorMsgt(D2DContext& cxt, long result)
{
	WCHAR cb[256];
	StringCbPrintf(cb,256,L"internet err: %d", result );
	cxt.DText(FPointF(0,0), cb );


}

static std::wstring CStr(float v)
{
	WCHAR cb[64];
	StringCbPrintf(cb,64,L"%0.0f", v);
	return cb;
}

static void yahooDrawLine(D2DContext& cxt,ChartInfo& chinfo, std::vector<Rousoku>& adj_values)
{
	float fstart,fend;
	float plotstep = CalcPlotStepLine(chinfo.fmax, chinfo.fmin, &fstart, &fend);

	float futosa = 1.0f;

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

	for(float yvalue=fstart; yvalue < fend; yvalue+=plotstep )
	{			
		float ploty = chinfo.vsz.height - (yvalue-chinfo.fmin)*chinfo.step;

		FPointF pt1(0,ploty);
		FPointF pt2(chinfo.vsz.width,ploty);

		(*cxt)->DrawLine(pt1,pt2,cxt.black_, futosa, dotline_);
		
		cxt.DText(FPointF(0,ploty), CStr(yvalue) );
	}
	
	//yvalue = -(ploty - chinfo.vsz.height)/chinfo.step + chinfo.fmin;



}

void xDraw( D2DContext& cxt, std::wstring txt, LPCWSTR fontnm, float fontheight, ColorF clr, FPointF pt)
{	
	ComPTR<IDWriteFactory> pfact;
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**) &pfact);

	ComPTR<IDWriteTextFormat> textFormat;
	std::wstring fontName(fontnm);
		(pfact->CreateTextFormat(fontName.c_str(), NULL, DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontheight, L"en-us", &textFormat));


	
	FRectF rc(pt.x, pt.y, FSizeF(1000,1000));
	ComPTR<ID2D1SolidColorBrush> br;

	(*cxt)->CreateSolidColorBrush(clr, &br);

	(*cxt)->DrawText(txt.c_str(), txt.length(), textFormat, rc, br );


}


static void yahooDrawString(D2DContext& cxt,std::wstring cd, ChartInfo& chi, std::vector<Rousoku>& adj_values)
{
	
	xDraw(cxt, cd, L"Arial",36, D2RGBA(170,171,170,200), FPointF(20,20));



}


void CalcAverage(UINT idx,UINT period, std::vector<Rousoku>& ar)
{
	_ASSERT(idx==0 || idx==1);
	_ASSERT(period != 0);
	float sum=0;

	for(UINT i=0; i < period; i++ )
	{
		sum += ar[i].yend;

		ar[i].average[idx] = -1;
	}

	for(int i = period; i < ar.size(); i++ )
	{
		ar[i].average[idx] = sum/period;
		
		sum += ar[i].yend;
		sum -= ar[i-period].yend;
		
	}
}



bool CreateRousokuFromtStream(LPCSTR cd, bool bSave, IStream* ism,std::vector<Rousoku>& adj_values, std::vector<std::string>& dates )
{
	ULONG len;
	LRESULT hr=0;
	char cb[64];

	adj_values.clear();
	dates.clear();

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

		if (i++ > 0 &&  !cols.empty())
		{
			auto kstr = r.c_str();

			Rousoku r={};
			r.ystart = myatof(kstr, cols[1]);
			r.yend = myatof(kstr, cols[4]);
			r.ymin = myatof(kstr, cols[3]);
			r.ymax = myatof(kstr, cols[2]);

			lstrcpyA(r.date, mysubstr(kstr, cols[0]).c_str());

			r.xpos = i-1;

			adj_values.push_back(r);
			dates.push_back(mysubstr(kstr, cols[0]));
		}
	}
	
	if ( bSave )
	{
		// SQLLite3 database
		DbUpdateStockData(cd,  adj_values);
		DbUpdateDaillyData(dates);
	}

	return true;
}

void yahooDraw(D2DContext& cxt, std::wstring cd, InternetInfo* info, FSizeF vsz, std::vector<Rousoku>& adj_values)
{
	if ( !adj_values.empty())
	{
		D2DRectFilter f(cxt,FRectF(0,0, vsz));

		auto chi = yahooDrawChart(cxt,vsz,adj_values);

			
		//yahooDrawAverageLine(cxt,0,chi,adj_values);

		yahooDrawLine(cxt,chi,adj_values);

		// yvalue = -(ploty - chinfo.vsz.height)/chinfo.step + chinfo.fmin;

		yahooDrawString(cxt,cd,chi,adj_values);
	}
	else
	{
		yahooDrawErrorMsgt(cxt, -1);			
	}

}


bool DbUpdateDaillyData(std::vector<std::string>& days)
{
	try
	{
		sqlite3pp::database db("stock.db");
		sqlite3pp::transaction xct(db);
		
		sqlite3pp::command cmd(
			db, "insert into daily_data(nox,dt) select ?1, ?2 where not exists (select * from daily_data where dt=?2)");
		
		for(auto& it : days )
		{
			sqlite3pp::query qry(db, "SELECT max(nox) FROM daily_data");
			sqlite3pp::query::iterator i = qry.begin();
			int nox = (*i).get<int>(0);

			cmd.bind(1, nox+1);
			cmd.bind(2, it.c_str());
			

			cmd.execute();

			cmd.reset();
		}
		
		xct.commit();

	}
	catch( sqlite3pp::database_error str)
	{
		return false;
	}

	return true;
}
bool DbUpdateStockData( LPCSTR cd, std::vector<Rousoku>& stock)
{
	try
	{
		sqlite3pp::database db("stock.db");
		sqlite3pp::transaction xct(db);
		
		sqlite3pp::command cmd(
			db, "insert into kabu_data(dt,cd,m1,m2,m3,m4,vol) select ?1,?2,?3,?4,?5,?6,?7 where not exists (select * from kabu_data where dt=?1 and cd=?2)"); 
		
		for(auto& it : stock )
		{
			cmd.bind(1, it.date);
			cmd.bind(2, cd);
			cmd.bind(3, it.ystart);
			cmd.bind(4, it.ymax);
			cmd.bind(5, it.ymin);
			cmd.bind(6, it.yend);
			cmd.bind(7, 0);
			

			cmd.execute();

			cmd.reset();
		}
		
		xct.commit();
	}
	catch( sqlite3pp::database_error str)
	{
		auto s = str;

		return false;
	}	

	return true;


}
bool DbReadStockData( LPCSTR cd, IStream** ppout)
{
	try {
	
		sqlite3pp::database db("stock.db");


		std::string sql = "select dt,m1,m2,m3,m4 from kabu_data where cd='";
		sql += cd;
		sql += "'";

		sqlite3pp::query qry(db, sql.c_str());


		std::stringstream sm;

		sm << 'date' << ',' << 'm1' << ',' << 'm2' << ',' << 'm3' << ',' << 'm4' << '\n';
	
		for(auto i = qry.begin(); i != qry.end(); i++ )
		{
			auto dte = (*i).get<std::string>(0);
			auto q1 = (*i).get<float>(1);
			auto q2 = (*i).get<float>(2);
			auto q3 = (*i).get<float>(3);
			auto q4 = (*i).get<float>(4);

			sm << dte << ',' << q1 << ',' << q2 << ',' << q3 << ',' << q4 << '\n';

		}

		ComPTR<IStream> is;
		auto r = CreateStreamOnHGlobal(NULL,FALSE,&is );

		auto s = sm.str();
		auto slen = s.length();
		ULONG dw;

		is->Write(s.c_str(), slen,&dw);

		is->AddRef();
		*ppout = is;

	}
	catch( sqlite3pp::database_error str)
	{
		auto s = str;

		return false;
	}	


	return true;
}