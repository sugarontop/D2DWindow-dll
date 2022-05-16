#include "pch.h"
#include "D2DSqlDataGrid.h"


using namespace V6;

#define NO_WIDTH 40.0f
#define ROW_HEIGHT 24.0f

static std::wstring CNo(UINT no);
static std::vector<std::wstring> Split(const std::wstring& str, LPCWSTR split);
static void DrawCell(D2DContext& cxt, const std::wstring& s,float xpos, float width, ID2D1SolidColorBrush* text,ID2D1SolidColorBrush* back, ID2D1SolidColorBrush* line);

void D2DSqlDataGrid::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControl::InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	fore_ = ColorF::Black;
	back_ = ColorF::White;
	row_first_ = 0;
	rc_ = rc;
}
void D2DSqlDataGrid::Clear()
{
	colnms_.clear();
	rows_.clear();
}
void D2DSqlDataGrid::Draw(D2DContext& cxt)
{
	if (BITFLG(STAT_VISIBLE))
    {	
		D2DRectFilter f(cxt, rc_);

		(*cxt)->FillRectangle(rc_,cxt.white_);
		
		
		D2DMatrix mat(*cxt);
		mat.PushTransform();
		mat.Offset(rc_);

		DrawTitle(cxt);

		mat.Offset(0,ROW_HEIGHT);

		DrawRowData(cxt);

		mat.PopTransform();
	}
}

void D2DSqlDataGrid::DrawTitle(D2DContext& cxt)
{
	FPointF pt(NO_WIDTH,0);

	ComPTR<ID2D1SolidColorBrush> gray;
	(*cxt)->CreateSolidColorBrush(D2RGB(200,200,200), &gray);
	ComPTR<ID2D1SolidColorBrush> gray2;
	(*cxt)->CreateSolidColorBrush(D2RGB(240,240,240), &gray2);
	
	for(auto& it : colnms_ )
	{		
		if ( it.width ==0 )
		{
			DWRITE_TEXT_METRICS t;

			ComPTR<IDWriteTextLayout> text;
			cxt.tsf_wfactory_->CreateTextLayout(it.title.c_str(), (UINT32)it.title.length(), cxt.textformat_, 1000.0f, ROW_HEIGHT, &text );
			text->GetMetrics(&t);

			it.width = max(t.width, 80.0f);
		}


		DrawCell(cxt, it.title, pt.x, it.width, cxt.black_, gray2, gray);
		pt.x += it.width;
	}
}

void D2DSqlDataGrid::DrawRowData(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);
	UINT no = 1;

	FPointF pt(0,0);

	ComPTR<ID2D1SolidColorBrush> gray;
	(*cxt)->CreateSolidColorBrush(D2RGB(200,200,200), &gray);
	ComPTR<ID2D1SolidColorBrush> gray2;
	(*cxt)->CreateSolidColorBrush(D2RGB(240,240,240), &gray2);

	auto cnt = colnms_.size();
	UINT ir = row_first_;

	auto it = rows_.begin() + row_first_;

	auto limit = 1 + (UINT)(rc_.Height() / ROW_HEIGHT);

	for( ; it != rows_.end(); it++ )
	{
		std::vector<std::wstring> cols = Split(*it, L"\t");

		if (cnt <= cols.size())
		{

			DrawCell(cxt, CNo(no++), pt.x, NO_WIDTH, cxt.black_, gray2, gray);
			pt.x += NO_WIDTH;

			UINT ci = 0;
			for(auto& c : cols )
			{
				auto width = colnms_[ci++].width;
				DrawCell(cxt, c, pt.x, width, cxt.black_, cxt.white_, gray);
				pt.x += width;
			}
			mat.Offset(0,ROW_HEIGHT);
			pt.x = 0;
		}

		if ( ir > limit )
			break;

		ir++;
	}
}

void D2DSqlDataGrid::SetData(LPCWSTR str)
{

	rows_ = Split(str, L"\n");

	if ( !rows_.empty() )
	{
		auto titles = rows_[0];

		auto ar = Split(titles, L"\t");
		
		for(auto& it : ar )
			AddTitle(it);

		rows_.erase( rows_.begin());
	}


}

LRESULT D2DSqlDataGrid::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;
	
	switch(message)
	{
		case WM_D2D_CREATE:

			r = 1;
		break;


	}


	return r;
} 
 
void D2DSqlDataGrid::AddTitle(const std::wstring& title)
{
	ColumTitle c;
	c.title = title;
	c.width = 0;

	colnms_.push_back(c);
}
void D2DSqlDataGrid::AddRow(const std::wstring& row)
{
	rows_.push_back(row);
}

static void DrawCell(D2DContext& cxt, const std::wstring& s,float xpos, float width, ID2D1SolidColorBrush* text,ID2D1SolidColorBrush* back, ID2D1SolidColorBrush* line)
{
	FRectF rc(xpos,0,xpos+width,ROW_HEIGHT);	
	(*cxt)->DrawRectangle(rc,line);
	(*cxt)->FillRectangle(rc,back);

	rc.left +=5.0f;
	(*cxt)->DrawText(s.c_str(),s.length(),cxt.textformat_, rc, text, D2D1_DRAW_TEXT_OPTIONS_CLIP|D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);

}

static std::wstring CNo(UINT no)
{
	WCHAR cno[16];
	StringCbPrintf(cno,16,L"%d", no);
	return cno;
}
static std::vector<std::wstring> Split(const std::wstring& str, LPCWSTR split)
{	
	std::vector<std::wstring> ar;
    int splen = lstrlenW(split);
    int len = str.length();
    _ASSERT( 0 < splen && splen <= 2  );

    int si = 0;
    for( int i = 0; i <= len; i++ )
    {
        if ( str[i] == split[0] || (i == len && 0 < len) )
        {
            if (splen == 1 || (splen == 2 && (i == len || str[i+1] == split[1] )) )
            {
                std::wstring s( &str[si], i-si );
                ar.push_back(s);
                si = i + splen;
            }
        }       
    }
    return ar;
}