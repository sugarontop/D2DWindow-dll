#include "pch.h"
#include "D2DTextbox.h"
#include "TextMemory.h"
#include "D2DColor.h"
using namespace V6;

#define  APP (D2DApp::GetInstance())


std::wstring MoneyString( const std::wstring& text );


D2DTextbox::D2DTextbox():back_(D2RGB(255,255,255)),fore_(D2RGB(0,0,0)),border_(D2RGB(0,0,0))
{

}
void D2DTextbox::CreateControl(D2DWindow* parent, D2DControls* pacontrol, TYP typ, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id )
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rctext_ =  rc;
	typ_ = typ;
	tm_ = {0};	
	ct_.bSingleLine_ = true;
	isImeOn_ = false;
	
	if (IsMultiline())
	{
		vscrollbar_.Create(this, FRectF(rctext_.right, rctext_.top, rctext_.right + BARW, rctext_.bottom));
		ct_.bSingleLine_ = false;
	}
}
D2DTextbox::~D2DTextbox()
{
	Clear();
}

void D2DTextbox::SetTypPassword()
{
	typ_ = IBridgeTSFInterface::TYP::PASSWORD;
}

bool D2DTextbox::IsMultiline() const
{
	return (typ_ == IBridgeTSFInterface::MULTILINE );
}

TSF::CTextEditorCtrl* D2DTextbox::ctrl() const
{
	return (TSF::CTextEditorCtrl*)parent_window_->tsf_.ctrl;
}
void D2DTextbox::SetReadonly(bool bReadOnly)
{
	stat_ = (bReadOnly ? (stat_ &~ STAT_ENABLE) : (stat_ | STAT_ENABLE));
}
void D2DTextbox::Draw(D2DContext& cxt)
{ 
	if ( BITFLG(STAT_VISIBLE) )
	{
		ComPTR<ID2D1SolidColorBrush> border;
		ComPTR<ID2D1SolidColorBrush> back;
		ComPTR<ID2D1SolidColorBrush> fore;
		
		(*cxt)->CreateSolidColorBrush(fore_, &fore);	
		(*cxt)->CreateSolidColorBrush(back_, &back);
		(*cxt)->CreateSolidColorBrush(border_, &border);
		

		D2DMatrix mat(*cxt);
		mat_ = mat.PushTransform();

		(*cxt)->DrawRectangle(rctext_, border);
		(*cxt)->FillRectangle(rctext_, back);


		auto rc = rctext_;
		auto bMultiline = IsMultiline();
		rc.InflateRect(1,1);

		D2DRectFilter fil(cxt, rc, vscrollbar_.Width());

		if (APP.IsCaptureEx(this)==1)
		{
			mat.Offset(rctext_);
			mat.Offset(-ct_.offpt_.x, -vscrollbar_.Scroll());
		
			mat_sc_ = mat.Copy();
		
			if (ctrl()->ct_)
			{
				ctrl()->Render(cxt, &tm_, fore, fmt_);

				
			
	#ifdef DRAW_CHAR_RECT
				// char RECTの表示
				int pos = 0;
				while(1)
				{
					FRectF rc;
					bool b;
					if ( !ctrl()->GetLayout()->RectFromCharPos_test(pos, &rc,&b))
						break;
					cxt.DrawBlackLine(rc);
					pos++;
				}
	#endif
				// draw caret
				{
					int xpos = ctrl()->CurrentCaretPos();
					FRectF rc;
					bool blf;
					if(  ctrl()->GetLayout()->RectFromCharPosEx(xpos-1, &rc, &blf) )
					{
						if ((::GetTickCount64() / 500)%2 == 0)							
							(*cxt)->FillRectangle( FRectF(rc.right, rc.top, FSizeF(CARET_W, rc.Height())), fore );
							//(*cxt)->FillRectangle( FRectF(rc.right, rc.bottom-CARET_W, FSizeF(rc.Width(), CARET_W)), fore );
					
						if (bMultiline)
							(*cxt)->FillRectangle( FRectF(0, rc.bottom-0.5f, FSizeF( rctext_.Width(), 0.5f)), fore );
					}
					cxt.Redraw();
				}	
			}
		}
		else if ( text_layout_ )
		{			
			mat.Offset(rctext_);
			mat.Offset(0, -vscrollbar_.Scroll());
			mat_sc_ = mat.Copy();

			(*cxt)->DrawTextLayout(FPointF(), text_layout_, fore );			
		}
		mat.PopTransform();

		if (bMultiline)
		{
			float h = ctrl()->GetLineHeight();
			int cnt = tm_.lineCount;

			if ( h*cnt > rctext_.Height())
				vscrollbar_.OnDraw(cxt);
		}
	}
}


std::wstring D2DTextbox::ConvertInputText(LPCWSTR text, int typ)
{
	if ( typ == 1 )		
		return MoneyString(text); 
	return text;
}



void D2DTextbox::ActiveSw(bool bActive)
{ 
	D2DContext& cxt = parent_window_->GetContext();

	_ASSERT(ctrl());

	ctrl()->SetContainer( &ct_, this ); 
	_ASSERT(ctrl()->ct_);

	ctrl()->CalcRender( cxt, fmt_ );
	
	text_layout_.Release();
	ctrl()->GetLayout()->GetTextLayout( &text_layout_ );
}

void D2DTextbox::StatActive(bool bActive)
{ 
	ActiveSw(bActive);

	if (bActive)
	{			
		ctrl()->SetFocus(&mat_sc_);
		//TRACE( L"D2DTextbox::StatActive(TRUE)   %x\n", this );

	}
	else
	{
		// from OnChangeFocus
		if ( ctrl()->GetContainer() == &ct_ )
		{						
			ctrl()->SetContainer( NULL, NULL );
		}
		
		_ASSERT(ctrl()->ct_==nullptr);
		//TRACE(L"D2DTextbox::StatActive(FALSE)   %x\n", this);
	}
}

bool D2DTextbox::OnChangeFocus(bool bActive, D2DCaptureObject*)
{

	StatActive(bActive);
	return true;
}

LRESULT D2DTextbox::WndProc(AppBase& b, UINT msg, WPARAM wp, LPARAM lp)
{
	if ( !BITFLG(STAT_ENABLE) )
		return 0;


	V6::D2DContextEx& cxt = this->parent_window_->GetContext();

	LRESULT ret = 0;
	TSF::TSFApp app(b.hWnd,  cxt);
	bool bl = false;
	static int mouse_mode = 0;

	switch( msg )
	{
		case WM_LBUTTONDOWN:
		{
			MouseParam* mp = (MouseParam*)lp;

			mp->mat = mat_sc_;

			auto pt = mat_.DPtoLP(mp->pt);

			if (GetVsrollbarRect().PtInRect(pt))
			{
				APP.SetCapture(this);
				ret = 1;
				bl = false;
				mouse_mode = 1;
			}
			else if ( rctext_.PtInRect(pt) )
			{
				APP.SetCapture(this);
				mouse_mode = 0;
				ret = 1;
				bl = true;
			}
			else if (APP.IsCapture(this) )
			{
				ImeActive(false);
				APP.ReleaseCapture();
				
				ret = 1;
				bl = true;
			}
				
		}
		break;
		case WM_LBUTTONUP:			
		{
			if (APP.IsCapture(this) )
			{
				MouseParam* mp = (MouseParam*)lp;
				mp->mat = mat_sc_;

				if (mouse_mode == 0 )
				{
					auto pt = mat_.DPtoLP(mp->pt);
					if ( !rctext_.PtInRect(pt) )
					{
						ImeActive(false);
						//APP.ReleaseCapture();	ここでリリースしない				
					}
					ret = 1;
					bl = true;
				}						
			}

		}
		break;		


		case WM_CHAR:		
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_MOUSEMOVE:			
		{
			if (APP.IsCapture(this))
			{
				if (mouse_mode == 0)
				{
					bl = true;
					ret = 1;
					AutoScroll();
				}
				else if ( mouse_mode == 1)
				{
					bl = false;
					ret = 1;
				}

				if ( msg == WM_MOUSEMOVE)
				{
					MouseParam* mp = (MouseParam*)lp;
					mp->mat = mat_sc_;
					b.bRedraw = true;
				}
					
				if ( msg == WM_KEYDOWN )
				{
					if ( wp == VK_RETURN && !IsMultiline() )
					{						
						parent_window_->SendMessage(WM_D2D_ON_TEXTBOX_RETURN, (WPARAM)this,0);
						ret = 1;
					}
					ret = 1;
				}
			}
		}
		break;
		case WM_MOUSEWHEEL:
		{
			MouseParam* mp = (MouseParam*)lp;

			auto pt = mat_.DPtoLP(mp->pt);
			if (IsMultiline() && rctext_.PtInRect(pt) && APP.IsCapture(this) && ctrl()->ct_)
			{
				_ASSERT(ctrl());

				ret = 1;
				bl = false;
				ctrl()->MoveSelectionUpDown((mp->zDelta > 0), false);
				AutoScroll();
			}
		}
		break;
		case WM_D2D_SET_COLOR:
		{
			ColorF clr = *(ColorF*)lp;

			if ( wp == 0 )
				back_ = clr;
			else if ( wp == 1 )
				fore_ = clr;
			else if ( wp == 2 )
				border_ = clr;

			bl = false;
			ret = 1;

		}
		break;
		case WM_D2D_COMMAND_SET:
		{
			if ( (UINT_PTR)this == (UINT_PTR)wp)
			{
				LPCWSTR cmd = (LPCWSTR)lp;

				auto ar = SplitW(cmd,L"&");

				for(auto& it : ar)
				{
					auto ar2 = SplitW(it.c_str(), L"=");

					if ( ar2.size() == 2)
					{
						if (ar2[0] == L"text")
						{
							Clear();
							SetText(ar2[1].c_str(), (int)ar2[1].length());
						}	
						else if (ar2[0] == L"color")
						{
							D2DColor clr(ar2[1].c_str());
							fore_ = clr;
						}
						else if (ar2[0] == L"bkcolor")
						{
							D2DColor clr(ar2[1].c_str());
							back_ = clr;
						}
					}
				}
				ret = 1;
			}
		}
		break;
		
		case WM_D2D_IME_ONOFF:
		{
			int d = (wp==0 ? 0 : 1);
			HIMC himc = ImmGetContext(b.hWnd);			 
			ImmSetOpenStatus(himc, d);
			ImmReleaseContext(b.hWnd, himc);
			bl=false;
			ret = 1;
		}
		break;
		case WM_D2D_ONIME_ONOFF:
		{
			if (APP.IsCapture(this))
			{
				bool on_bl = (WPARAM)(wp==1);


				isImeOn_ = on_bl;

				bl = false; ret=1; 
			}
		}
		break;
		case WM_D2D_SET_TEXT:
		{
			LPCWSTR str = (LPCWSTR)lp;

			this->SetText(str,wcslen(str),0);

			bl = false; ret=1; 
		}
		break;
		case WM_SETFOCUS:
		{
			if (APP.IsCapture(this))
			{
				OnChangeFocus(true,nullptr);

				bl = false; ret=1;
			}


			TRACE( L"WM_SETFOCUS\n");
		}
		break;
		case WM_KILLFOCUS:
		{
			TRACE( L"WM_KILLFOCUS\n");
		}
		break;
		case WM_LBUTTONDBLCLK:
			bl = true;
		break;

	}
		 

	if ( bl )
		ret = ctrl()->WndProc( &app, msg, wp, lp ); // WM_CHAR,WM_KEYDOWNの処理など


		
		
	if (mouse_mode == 1)
	{			
		ret = vscrollbar_.WndProc(b, msg, wp, lp);

		if (msg == WM_LBUTTONUP )
			mouse_mode = 0;
	}

	if (msg == WM_KEYDOWN && ret == 1)
	{

		AutoScroll();
	}
		
	return ret;
}
void D2DTextbox::ImeActive(bool bActive)
{
	parent_control_->SendMesage(WM_D2D_IME_ONOFF, (bActive? 1: 0),0);
}

void D2DTextbox::AutoScroll()
{
	if (IsMultiline())
	{		
		if (tm_.lineCount > 0)
		{
			int pos = ctrl()->CurrentCaretPos();

			if (vscrollbar_.pos_ != pos )
			{
				int len;
				const FRectF* rcs = ctrl()->GetLayout()->GetCharRects2(&len);
				
				if ( pos < len )
				{
					auto drc = mat_sc_.LPtoDP(rcs[pos]);
					auto drctext2 = mat_.LPtoDP(rctext_);

					float rowheight = tm_.height / tm_.lineCount;

					FSizeF s1(0,rowheight);
					s1 = mat_sc_.LPtoDP(s1);
					float dheight = s1.height;
			
					if (drc.bottom + dheight > drctext2.bottom)	
						vscrollbar_.SetScroll(rowheight);
					else if (drc.top - dheight < drctext2.top)
						vscrollbar_.SetScroll(-rowheight);
				}
			}
			vscrollbar_.pos_ = pos;
		}
	}
	else
	{
		// SINGLE LINE
		int pos = ctrl()->CurrentCaretPos();
		if (vscrollbar_.pos_ != pos) // 面倒なのでvscrollbar_.pos_を使用
		{
			FRectF rc;
			bool blf;
			if (ctrl()->GetLayout()->RectFromCharPosEx(pos-1, &rc, &blf))
				ct_.offpt_.x = min(9000.0f, max(0.0f,  rc.right - 0.93f*rctext_.Width()));

		}
		vscrollbar_.pos_ = pos;


	}
}

int D2DTextbox::CurrentPos() const
{
	return ct_.SelStart();
}
bool D2DTextbox::SetFont(LPCWSTR fontnm, float fontheight)
{
	ComPTR<IDWriteTextFormat> textformat;
	if ( S_OK == parent_window_->GetContext().tsf_wfactory_->CreateTextFormat(fontnm, NULL, 
		DWRITE_FONT_WEIGHT_REGULAR,DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		fontheight, LOCALE, & textformat))
	{
	
		fmt_ = nullptr;
		fmt_ = textformat;
		return true;
	}	
	
	return false; ;
}

void D2DTextbox::SetText(LPCWSTR str, int str_len, int insert_pos)
{ 
	if ( IsMultiline())
		SetMultilineText(str,str_len,insert_pos );
	else
		SetSinglelineText(str,str_len,insert_pos );
}

void D2DTextbox::SetSinglelineText(LPCWSTR str, int str_len, int insert_pos )
{
	std::wstringstream sm;
	for(int i = 0; i < str_len; i++ )
	{
		auto ch = str[i];
		if ( ch == 0x0A || ch == 0x0D )
		{
			sm << L'\0';
			break;
		}
		sm << ch;
	}
	
	auto s = sm.str();

	insert_pos = max(0, insert_pos);

	
	
	UINT n=0;
	ct_.InsertText(insert_pos, s.c_str(), (UINT32)s.length(), n);

	if ( this != APP.GetCapture() )
		ActiveSw(false);
	
	int pos = CurrentPos() + (int)s.length();
	if ( ctrl() )
	{
		ctrl()->MoveSelection( pos, pos, false );
		ctrl()->GetLayout()->bRecalc_ = true;
	}
}

void D2DTextbox::SetMultilineText(LPCWSTR str, int str_len, int insert_pos )
{

	std::wstringstream sm;
	for(int i = 0; i < str_len; i++ )
	{
		auto ch = str[i];
		if ( ch == 0x0A )
			sm << ch;
		else if ( ch != 0x0D )
			sm << ch;
	}
	auto s = sm.str();

	insert_pos = max(0, insert_pos);
	
	UINT n=0;
	ct_.InsertText(insert_pos, s.c_str(), (UINT32)s.length(), n);

	if ( this != APP.GetCapture() )
		ActiveSw(false);
	
	int pos = CurrentPos() + (int)s.length();
	if ( ctrl() )
	{
		ctrl()->MoveSelection( pos, pos, false );
		ctrl()->GetLayout()->bRecalc_ = true;

	}
}

void D2DTextbox::GetText( std::wstringstream* out,  bool crlf )
{
	auto p = ct_.GetTextBuffer();	
	auto len = ct_.GetTextLength();

	if ( crlf ) {
		for( UINT i = 0; i < len; i++ )
		{
			if ( *p == '\n' )
				*out << L"\r\n";
			else
				*out << *p;
			p++;
		}
	}
	else
		out->write(p,len);
}
bool D2DTextbox::GetSelectText(std::wstringstream* out, bool crlf )
{
	auto p = ct_.GetTextBuffer() + ct_.SelStart();
	int len = ct_.SelEnd() - ct_.SelStart();

	if ( len > 0 )
	{
		if (crlf) {
			for (int i = 0; i < len; i++)
			{
				if (*p == '\n')
					*out << L"\r\n";
				else
					*out << *p;
				p++;
			}
		}
		else
			out->write(p, len);
		return true;
	}
	return false;
}

void D2DTextbox::Clear()
{
	ct_.Reset();
	text_layout_.Release();
	vscrollbar_.SetTotalHeight(0);
	tm_ = {};

	auto ctrl = (TSF::CTextEditorCtrl*)this->parent_window_->tsf_.ctrl;

	if (ctrl)
		ctrl->Clear();

}
FRectF D2DTextbox::GetVsrollbarRect() const
{
	FRectF rc(rctext_);
	rc.right += vscrollbar_.Width();
	rc.left = rc.right - vscrollbar_.Width();
	return rc;
}


//static
void* D2DTextbox::CreateInputControl(D2DWindow* parent)
{ 
	if ( parent->tsf_.ctrl == nullptr)
	{
		auto ctrl = new TSF::CTextEditorCtrl();
		
		auto tmgr = parent->tsf_.pThreadMgr;
		auto id = parent->tsf_.TfClientId;
		auto hWnd = parent->tsf_.hWnd;

		ctrl->Create( hWnd, tmgr, id );	

		parent->tsf_.ctrl = ctrl;
	}
	return parent->tsf_.ctrl;
}

// IBridgeTSFInterface
FRectF D2DTextbox::GetClientRect() const
{ 
	DWRITE_TEXT_METRICS m;

	if (ctrl()->GetLayout() )
	{ 
		m = ctrl()->GetLayout()->GetTextMetrics();

		FRectF rc = rctext_.ZeroRect();
		rc.bottom = rc.top + m.height;

		int pos = ctrl()->CurrentCaretPos();

		int len;
		auto prc1 = ctrl()->GetLayout()->GetCharRects2(&len);

		if ( prc1 && 0 <pos && pos <= len )
		{
			FRectF rc2 = prc1[pos-1];

			rc.Offset(rc2.right,0);

			rc.top += rc2.top;
			rc.bottom = rc.top + rc2.Height();
		}

		rc.Offset( 0, m.height);

		return rc;   // 候補の表示位置, candidate
	}
	return rctext_;
}



float D2DTextbox::sc_barTotalHeight(bool bHbar)
{
	if ( bHbar )
		return rctext_.Height();

	return 0;

}

float D2DTextbox::sc_dataHeight(bool bHbar)
{
	if ( bHbar )
	{
		float h = ctrl()->GetLineHeight();
		return tm_.height + 50.0f;
	}
	return 0;

}
std::wstring D2DTextbox::GetTreeTyp(USHORT* typ)
{ 
	*typ=2; 
	return L"D2DTextbox";
}

std::wstring D2DTextbox::Ascii2W( LPCSTR s )
{
	int len = (int)strlen(s);
	int cblen = ::MultiByteToWideChar(CP_ACP, 0, s, len, nullptr, 0);
	std::wstring x(cblen, L'\0');
	::MultiByteToWideChar(CP_ACP, 0, s, len, (LPWSTR)(LPCWSTR)x.c_str(), cblen);
	return x;
}
std::string D2DTextbox::W2Ascii( LPCWSTR s )
{
	int len = (int)wcslen(s);
	int cblen = ::WideCharToMultiByte(CP_ACP, 0, s, len, nullptr, 0, nullptr, nullptr);
	std::string cb(cblen, '\0');
	::WideCharToMultiByte(CP_ACP, 0, s, len, (LPSTR)(LPCSTR)cb.c_str(), cblen, nullptr, nullptr);
	return cb;
}


std::wstring MoneyString( const std::wstring& text )
{
	/*
		入力例
	auto s0 = MoneyString("");		_ASSERT( s0 == L"" );
	auto s1 = MoneyString("1000");_ASSERT( s1 == L"1,000" );
	auto s2 = MoneyString("-1000");_ASSERT( s2 == L"-1,000" );
	auto s3 = MoneyString("-0001000");_ASSERT( s3 == L"-1,000" );

	auto s4 = MoneyString("-0001000.001");_ASSERT( s4 == L"-1,000.001" );
	auto s5 = MoneyString("+0001000.00100");_ASSERT( s5 == L"+1,000.00100" );
	auto s6 = MoneyString("1,000.000");_ASSERT( s6 == L"1,000.000" );
	auto s7 = MoneyString("1,00,000.000");_ASSERT( s7 == L"100,000.000" );

	*/
	_ASSERT( text.length() < 32 );

	std::wstring ret;

	WCHAR cba[32];
	WCHAR* p = cba;

	bool canma = false;
	for( UINT i = 0; i < text.length(); i++ )
	{
		WCHAR ch = text[i];
		if ( '0' <= ch && ch <= '9' )
			*p++ = ch;
		else if ( i == 0 && (ch == L'-' || ch =='+') )
			*p++ = ch;	
		else if ( ch == '.'  && canma == false)
		{	
			*p++ = ch;
			canma = true;	
		}
	}
	*p = 0;


	p = cba;

	WCHAR cbx[32];

	enum TYP { NONE,PLUS,MINUS };

	TYP ty = NONE;

	int start = 0;

	if ( *p == '-' )
	{
		ty = MINUS;
		start++;
	}
	else if ( *p == '+' )
	{
		ty = PLUS;
		start++;
	}
	else if ( *p == 0 )
		return text;
		
	p += start;

	while( *p && *p == '0' ) p++;


	WCHAR* ps = p;
	while( *p && *p != '.' ) p++;
	
	{
		std::wstring s1( ps, p );

		WCHAR* cb = cbx;

		if ( ty == MINUS )
		{
			cbx[0] = '-';
			cb = cbx+1;
		}
		else if ( ty == PLUS )
		{
			cbx[0] = '+';
			cb = cbx+1;
		}

		//xassert( s.length() < 32 );
	 
		int j =0, k = 0, slen = s1.length();

		for( int i = slen - 1; i >= 0; i-- ) 
		{
			if ( j && k % 3 == 0 )
				cb[j++] = ',';

			cb[j++] = s1[i];			
			k++;
		}
		cb[j] = 0;
		k = j-1;
	
		for( int i = 0; i < j; i++ )
		{		
			if ( i < k )
				std::swap( cb[i], cb[k--] );
			else
				break;
		}
		
		ret = cbx;

		if ( p )
		{
			// 小数点以下を追加
			ret += p;
		}
		
	}
	
	return ret;

}
