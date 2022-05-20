#include "pch.h"
#include "D2DSqlDataGrid.h"
#include "D2DColor.h"

using namespace V6;
using namespace V6::TOOL;

#define NO_WIDTH 40.0f
#define ROW_HEIGHT 24.0f
#define HEADER_HEIGHT ROW_HEIGHT
#define ALL_CELL -1

static std::wstring CNo(UINT no);
static std::vector<std::wstring> Split(const std::wstring& str, LPCWSTR split);
static void DrawCell(D2DContext& cxt, const std::wstring& s,float xpos, float width, ID2D1SolidColorBrush* text,ID2D1SolidColorBrush* back, ID2D1SolidColorBrush* line);


void D2DSqlDataGrid::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControl::InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	fore_ = ColorF::Black;
	back_ = ColorF::White;	
	rc_ = rc;

	FRectF rc1(0,0,BARW,100); 
	auto scV = std::make_shared<D2DScrollbar>();
	scV->CreateControl(parent, this, rc1,0, NONAME );
	this->Add(scV);
	scv_ = scV;
	vrow_top_ = 0;
	vscroll_x_ = 0;
	select_row_col_ = {};

	scV->SetMaxSize(rc_.Height()-HEADER_HEIGHT);
	
}
void D2DSqlDataGrid::Clear()
{
	colnms_.clear();
	rows_.clear();
	select_row_col_ = {};
	vrow_top_ = 0;
}
void D2DSqlDataGrid::Draw(D2DContext& cxt)
{
	if (BITFLG(STAT_VISIBLE))
    {	
		D2DRectFilter f(cxt, rc_);
		(*cxt)->FillRectangle(rc_,cxt.white_);
		
		D2DMatrix mat(*cxt);
		mat_ = mat.PushTransform();
		mat.Offset(rc_);
		{		
			mat.PushTransform();

				DrawTitle(cxt);

				mat.Offset(0,ROW_HEIGHT);

				vrow_top_ =  (int)(scv_->LogicalOffset() / ROW_HEIGHT);
			
				DrawRowData(cxt);

			mat.PopTransform();
		

			mat.PushTransform();	
				mat.Offset(vscroll_x_, HEADER_HEIGHT );
				scv_->Draw2(cxt);
			mat.PopTransform();
		}
		mat.PopTransform();
	}
}

void D2DSqlDataGrid::SetViewMaxSize(FSizeF sz)
{				
	//_ASSERT((0 < sz.width) && (0 < sz.height));
	_ASSERT((0 < sz.height));

	vscroll_x_ = rc_.Width()-BARW;
	//hscroll_x_ = 0;

	scv_->SetStat(STAT_DEFAULT);
	//sch_->SetStat(STAT_DEFAULT);

	scv_->SetMaxSize( sz.height );
	//sch_->SetMaxSize( sz.width );
	//sch_->SetSize(rc_.Size());


	FSizeF tsz(rc_.Size());
	tsz.height -= HEADER_HEIGHT;
	scv_->SetSize(tsz);

	//if (sz.width <= rc_.Size().width)
	//	sch_->SetStat(0);
	if (sz.height <= rc_.Size().height-HEADER_HEIGHT)
		scv_->SetStat(0);
	
}



void D2DSqlDataGrid::DrawTitle(D2DContext& cxt)
{
	FPointF pt(NO_WIDTH,0);

	auto gray = mk_color(D2RGB(200,200,200));
	auto gray2 = mk_color(D2RGB(240,240,240));

	
	for(auto& it : colnms_ )
	{		
		if ( it.width == 0 )
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
	if ( rows_.empty()) return;

	D2DMatrix mat(*cxt);

	FPointF pt(0,0);

	auto gray = mk_color(D2RGB(200,200,200));
	auto gray2 = mk_color(D2RGB(240,240,240));

	auto cnt = colnms_.size();
	UINT ir = vrow_top_;

	auto it = rows_.begin() + vrow_top_;

	auto limit = it->no + (UINT)(rc_.Height() / ROW_HEIGHT);

	for( ; it != rows_.end(); it++ )
	{
		std::vector<std::wstring> cols = Split( (*it).row, L"\t");

		if (cnt <= cols.size())
		{
			DrawCell(cxt, CNo((*it).no), pt.x, NO_WIDTH, cxt.black_, gray2, gray);
			pt.x += NO_WIDTH;

			UINT ci = 0;
			for(auto& c : cols )
			{
				auto width = colnms_[ci].width;
				DrawCell(cxt, c, pt.x, width, cxt.black_, cxt.white_, gray);

				if (IsSelectedCell( it->no, ci ))
				{
					auto br = mk_color(D2RGBA(124,156,200,80));
					(*cxt)->FillRectangle(FRectF(pt.x,0,pt.x+width,ROW_HEIGHT), br);
				}

				pt.x += width;
				ci++;
			}
			mat.Offset(0,ROW_HEIGHT);
			pt.x = 0;
		}

		if ( ir > limit )
			break;

		ir++;
	}
}

bool D2DSqlDataGrid::IsSelectedCell(int no, int col)
{
	if ( select_row_col_.r == no && select_row_col_.c == ALL_CELL )
		return true;
	else if ( select_row_col_.r == no && select_row_col_.c == col )
		return true;
	else if ( select_row_col_.r == ALL_CELL && select_row_col_.c == col ) 
		return true;
	else if ( select_row_col_.r == ALL_CELL && select_row_col_.c == ALL_CELL )
		return true;

	return false;
}

void D2DSqlDataGrid::SetData(LPCWSTR str)
{

	auto rows = Split(str, L"\n");

	if ( !rows.empty() )
	{
		auto titles = rows[0];

		auto ar = Split(titles, L"\t");
		
		for(auto& it : ar )
			AddTitle(it);

		rows.erase( rows.begin()); // ヘッダー行を削除
		vrow_top_ = 0;

		rows_.clear();
		UINT no = 1;
		for(auto& it : rows )
		{
			RowData d;
			d.no = no++;
			d.row = it;
			rows_.push_back(d);
		}



		FSizeF sz;
		sz.height = rows_.size() * ROW_HEIGHT;
		SetViewMaxSize(sz);
		
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

		case WM_MOUSEWHEEL:
		{
			MouseParam* mp = (MouseParam*)lParam;
            auto pt = mat_.DPtoLP(mp->pt);
            auto md = (rc_.PtInRect(pt) ? 1 : 0);

            if ( md == 1 )
            {
				r = InnerWndProc(b,message,wParam,lParam);

				if( r == 0)
				{
					float a = 0;
					if (mp->zDelta > 0)
						a = -ROW_HEIGHT;
					else if (mp->zDelta < 0)
						 a= ROW_HEIGHT;// 10.0f;

					scv_->Offset(a);
				}
				r = 1;
            }
		}
		break;
		case WM_LBUTTONDOWN:
		{
			MouseParam* mp = (MouseParam*)lParam;
            auto pt = mat_.DPtoLP(mp->pt);

			FRectF rc(rc_);
			rc.right -= BARW;

            if (rc.PtInRect(pt) )
			{
				if ( pt.x < rc_.left + NO_WIDTH )
				{
					select_row_col_.c = ALL_CELL;
					select_row_col_.r = (int)( (pt.y-rc.top) / ROW_HEIGHT) + vrow_top_;
				}
				else if ( pt.y < rc_.top + HEADER_HEIGHT )
				{
					select_row_col_.r = ALL_CELL;

					auto x = pt.x - rc.left - NO_WIDTH;
					int c = 0;
					for(auto& it : colnms_ )
					{
						if ( x < it.width )
							break;
						x -= it.width;
						c++;
					}
					select_row_col_.c = ( c < colnms_.size() ? c : 0);
				}
				else
				{
					select_row_col_.r = (int)( (pt.y-rc.top) / ROW_HEIGHT) + vrow_top_;

					auto x = pt.x - rc.left - NO_WIDTH;
					int c = 0;
					for(auto& it : colnms_ )
					{
						if ( x < it.width )
							break;
						x -= it.width;
						c++;
					}
					select_row_col_.c = ( c < colnms_.size() ? c : 0);

				}



				r = 1;
			}
		}
		break;
		case WM_KEYDOWN:
		{
			auto key = 0xff & wParam;

			if ( key == VK_ESCAPE )
			{
				select_row_col_ = {};
				r = 1;
			}

		}
		break;


	}

	if ( r == 0 ) //&& bl )
	{
		r = InnerWndProc(b,message,wParam,lParam);
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
	auto no = rows_.rbegin()->no;
	
	RowData d;
	d.no = no+1;
	d.row = row;

	rows_.push_back(d);
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

