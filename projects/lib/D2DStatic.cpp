#include "pch.h"
#include "D2DStatic.h"
#include "D2DTextbox.h"
#include "D2DColor.h"
using namespace V6;

#define  APP (D2DApp::GetInstance())


D2DStatic::D2DStatic():editor_(nullptr),editable_(false),extend_(false)
{
	
}
void D2DStatic::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rc_ = rc;
	editor_ = nullptr;

	editable_  = false;
	txt_clr_ = ColorF::Black;
}

void D2DStatic::Draw(D2DContext& cxt)
{
	_ASSERT(text_);

	if ( !extend_ )
	{
		ComPTR<ID2D1SolidColorBrush> br;
		cxt.CreateBrush(txt_clr_, &br);
		(*cxt)->DrawTextLayout(rc_.LeftTop(), text_, br );
	}
	else
	{
		ComPTR<ID2D1SolidColorBrush> br;
		cxt.CreateBrush(txt_clr_, &br);

		auto pt = rc_.LeftTop();		
		pt.x += offpt_.x;
		pt.y += offpt_.y;
		
		(*cxt)->DrawTextLayout(pt, text_, br );
	}

	if ( editor_ )
		editor_->Draw(cxt);	
	
}
std::wstring D2DStatic::GetTreeTyp(USHORT* typ)
{ 
	*typ=2; 
	return L"D2DStatic";
}

LRESULT D2DStatic::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;
	
	if ( editable_ )
	{
		bool bl = false;
		if ( message == WM_LBUTTONDOWN && editor_ == nullptr )
		{

			editor_ = new D2DTextbox();
			editor_->CreateControl(parent_window_, parent_control_, IBridgeTSFInterface::SINGLELINE, rc_, STAT_DEFAULT, NONAME );
			parent_control_->Add( std::shared_ptr<D2DTextbox>(editor_) );	


			editor_->SetBackColor(D2RGBA(0,0,0,0));
			editor_->SetBorderColor(D2RGBA(0,0,0,0));

			editor_->SetText(str_.c_str(), str_.length() );
		}
		else if ( message == WM_LBUTTONDOWN && editor_ )
			bl = true;
			
		
		if ( editor_ )
			r = editor_->WndProc(b,message,wParam,lParam);

		if ( message == WM_RBUTTONDOWN )
		{
			
			if ( editor_ != APP.GetCapture() )
			{
				editor_->DestroyControl();

				editor_ = nullptr;
			}



		}
		
	}

	if (message == WM_D2D_SET_COLOR )
	{
		ColorF clr = *(ColorF*)lParam;
		int idx = (int)wParam;

		if ( idx == COLOR_IDX_FORE )
			txt_clr_ = clr; //fore_ = clr;
				
		r = 1;
	}

	return r;
}

void D2DStatic::SetText( std::wstring str )
{
	str_ = str;
	text_ = nullptr;

	auto& cxt = parent_window_->GetContext();

	auto sz = rc_.Size();
	cxt.CreateTextLayout(str.c_str(), sz, &text_);

}


std::vector<_bstr_t> Split( LPCWSTR str, LPCWSTR split )
{
    std::vector<_bstr_t> ar;
    int splen = lstrlen(split);
    int len = lstrlen(str);
    _ASSERT( 0 < splen && splen <= 2  );

    int si = 0;
    for( int i = 0; i <= len; i++ )
    {
        if ( str[i] == split[0] || (i == len && 0 < len) )
        {
            if (splen == 1 || (splen == 2 && (i == len || str[i+1] == split[1] )) )
            {
                std::wstring s( &str[si], i-si );
                ar.push_back( _bstr_t(s.c_str()));
                si = i + splen;
            }
        }       
    }
    return ar;
}


std::map<_bstr_t, _bstr_t> Split(LPCWSTR str, WCHAR ch )
{
	std::map<_bstr_t, _bstr_t> m;

	auto ar = Split(str, L"&");

	for(auto& it : ar )
	{
		auto ar2 = Split( (LPCWSTR)it, L"=" );
		m[ar2[0]] = ar2[1];
	}
	return m;
}


bool find( std::map<_bstr_t, _bstr_t>& m, LPCWSTR key, _bstr_t* val)
{
	auto it = m.find(key);

	if ( it != m.end())
	{
		*val = (*it).second;
		return true;
	}
	return false;
}
 std::vector<float> Parse(LPCWSTR str)
{
	auto ch = str[0];
	std::vector<float> ar;
	
	if ( ch == '[' )
	{
		int spos,epos,i;
		i = 0;
		while( ch )
		{
			if ( ch == '[' ) spos = i+1;
			else if ( ch == ']' ) epos = i;
			ch = str[++i];
		}

		WCHAR cb[256]={};
		memcpy(cb, &str[spos], (epos-spos)*sizeof(WCHAR));
		
		auto xar = Split(cb, L",");

		for(auto & it: xar)
		{
			ar.push_back( (float)_wtof(it));
		}



	}
	else if ( '0' <= ch && ch <= '9' )
	{
		ar.push_back( (float)_wtof(str));	
	}

	return ar;
}

void D2DStatic::Extend( LPCWSTR prm )
{	
	auto m = Split( prm, '&' );

	SetText( (LPCWSTR)m[L"text"] );



	_bstr_t val;
	auto& cxt = parent_window_->GetContext();
	
	std::wstring fontname = L"–¾’©";
	float height = 26;
	auto sz = rc_.Size();

	if ( find(m, L"text", &val ))
	{
		str_ = val;
		text_ = nullptr;

	}
	if ( find(m, L"fontname", &val ))
	{
		fontname = val;

	}
	if ( find(m, L"fontheight", &val ))
	{
		auto sz = rc_.Size();
		height = (float)Parse(val)[0];
	}

	{

		ComPTR<IDWriteTextFormat> textformat;

		if ( S_OK == cxt.tsf_wfactory_->CreateTextFormat(fontname.c_str(), NULL, DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, height, LOCALE, & textformat))
		{
			cxt.tsf_wfactory_->CreateTextLayout(str_.c_str(), (UINT32)str_.length(), textformat, sz.width, sz.height, &text_ );
		}

		if ( find(m, L"text", &val ) && text_ == nullptr )
		{
			str_ = val;
			text_ = nullptr;

			cxt.CreateTextLayout(str_.c_str(), sz, &text_);
		}
	}

	_ASSERT(text_);

	if ( find(m, L"align", &val ))
	{
		DWRITE_TEXT_METRICS tm;

		if ( !wcscmp(val,L"right") )
		{
			extend_ = true;

			if ( text_ )
			{
				text_->GetMetrics(&tm);

				offpt_.x = (rc_.Width() - tm.width );
			}

		}
		else if ( !wcscmp(val,L"center") )
		{
			extend_ = true;
			if ( text_ )
			{
				text_->GetMetrics(&tm);

				offpt_.x = (rc_.Width() - tm.width ) / 2.0f;
			}

		}

	}
	

	if ( find(m, L"color", &val ))
	{
		auto ar = Parse(val);

		if ( 3 == ar.size() )
		{
			auto clr = D2RGB(ar[0],ar[1],ar[2]);

			txt_clr_ = clr;

			extend_ = true;
		}
	}
}