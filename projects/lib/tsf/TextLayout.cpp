#include "pch.h"
#include "TextLayout.h"

using namespace TSF;

CTextLayout::CTextLayout()
{
	nLineCnt_ = 0;
	str_ = NULL;
	row_width_ = 0;
	bPassword_ = false;
	bRecalc_ = true;
	DWTextLayout_ = nullptr;
	selected_halftone_color_ = D2RGBA(0,140,255,100);
}
CTextLayout::~CTextLayout()
{
	Clear();
}

//----------------------------------------------------------------
//
//
// Calc layout 行数の把握と文字単位にPOS,LEN,RECTを取得
//----------------------------------------------------------------

BOOL CTextLayout::Layout(D2DContext& cxt, const WCHAR *psz, int nCnt,const SIZE& sz, bool bSingleLine, int zCaret, int& StarCharPos,IDWriteTextFormat* fmt)
{
//TRACE( L"CTextLayout::Layout\n" );

	Clear();
	bSingleLine_ = bSingleLine;

	str_ = psz;
	nLineCnt_ = 0;
	StarCharPos_ = 0;

    // 行数を計算
 	{
		BOOL bNewLine = TRUE;
		for (int i = 0; i < nCnt; i++)
		{
			switch (psz[i])
			{
				case 0x0d:
				case 0x0a:                
					if (bNewLine)
						nLineCnt_++;
					bNewLine = TRUE;
					break;
				default:
					if (bNewLine)
						nLineCnt_++;
					bNewLine = FALSE;
					break;
			}
		}
	}
			
	
	
    // Count character of each line.　文字単位にPOS,LEN,RECTを取得
   
	int nCurrentLine = 0;

	CHPOS c;
	UINT crlf=0;
	int col = 0;
	{
		
		int nNewLine = 0;

		for (int i = 0; i < nCnt; i++)
		{
			switch (psz[i])
			{
				case 0x0d:
				case 0x0a:
				{
					c.pos = i;
					c.col = col;
					c.row = nCurrentLine;
					c.lf = true;
					CharPosMap_.push_back(c);

					nNewLine=0;
					nCurrentLine++;
					col=-1;					
								
					_ASSERT( crlf == 0 || crlf == psz[i] ); // CRLF is NG.  CRCR.. or LFLF.. is OK
					crlf = psz[i];
				}
				break;
				default:					
					/*if (nNewLine)
					{
						nCurrentLine++;
						col=0;
					}*/
					
					c.pos = i;
					c.col = col;
					c.row = nCurrentLine;
					c.lf = false;

					CharPosMap_.push_back(c);

					nNewLine = 0;
					crlf = 0;
					break;
			}
			col++;
		}
	}



	if ( DWTextLayout_ )
		DWTextLayout_->Release();


	{		
		CreateTextLayout ct(cxt.tsf_wfactory_, psz, nCnt, fmt, sz, bSingleLine_);

		StarCharPos = ct.Create( StarCharPos, zCaret, &DWTextLayout_ ); //★TextLayout_

		_ASSERT(DWTextLayout_);

		StarCharPos_ = StarCharPos;
			
		char_rectf_.Set(DWTextLayout_, false);

		int slen = nCnt;
		int len=0;

		//=============================================
		// 文字文のRECT作成、取得
		//
		const FRectF* prcs = char_rectf_.Create( psz, sz, slen, &len );

		nLineHeight_ = char_rectf_.LineHeight();

		for( int rcidx = 0; rcidx < len ; rcidx++ )
		{
			FRectF rc = prcs[rcidx];
			CharPosMap_[rcidx].rc = rc;
		}


		DWTextLayout_->GetMetrics(&tm_);
		return TRUE;
	}


    return FALSE;
}
float CTextLayout::GetLineHeight() const
{
	return nLineHeight_;
}


const FRectF* CTextLayout::GetCharRects2(int *len)
{
	*len = (int)CharPosMap_.size();	
	return char_rectf_.GetTextRect();
}

BOOL CTextLayout::GetCharInfo( int pos, CHPOS* info )
{
	if ( pos < (int)CharPosMap_.size() && 0 <= pos )
	{
		*info = CharPosMap_[pos];
		return true;
	}
	
	return false;

}

BOOL CTextLayout::Render(D2DContext& cxt, const FRectF& rcText,LPCWSTR psz,  int nCnt, int nSelStart, int nSelEnd,bool bTrail,
	const COMPOSITIONRENDERINFO *pCompositionRenderInfo, UINT nCompositionRenderInfo)
{
	{
		_ASSERT( DWTextLayout_ );

		(*cxt)->DrawTextLayout( FPointF(0,0), DWTextLayout_, cxt.black_ ); // D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_CLIP);
	}

	if ( nSelStart < nSelEnd && -1 < nSelStart )
	{
		ComPTR<ID2D1SolidColorBrush> br;
		(*cxt)->CreateSolidColorBrush(selected_halftone_color_, &br);

		for (int j = nSelStart; j < nSelEnd; j++)
		{
			(*cxt)->FillRectangle( CharPosMap_[j].rc, br ); 
		}
	}
    return TRUE;
}

//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------

BOOL CTextLayout::RectFromCharPos(UINT nPos, FRectF *prc)
{
	return RectFromCharPosEx( (int)nPos, prc, nullptr );
 }

BOOL CTextLayout::RectFromCharPosEx(int nPos, FRectF *prc, bool* blf)
{
	if ( nPos < 0 )
	{		
		prc->left = prc->top = 0;
		prc->bottom = nLineHeight_; 
		prc->right=TSF_FIRST_POS;

		return TRUE;
	}

	
	if ( this->nLineCnt_ == 0 ) return FALSE;

	if ( nPos < (int)CharPosMap_.size()) //find(nPos) != CharPosMap_.end())
	{
		auto c = CharPosMap_[nPos];
		FRectF rc = c.rc;
		if ( c.lf )
		{			
			rc.left = 0;
			rc.right =TSF_FIRST_POS;
			rc.Offset(0, rc.Height());		
			
			if (blf)
				*blf = true;
		}
		else
		{
			if (blf)
				*blf = false;
		}

		*prc = rc;
		return TRUE;
	}

	if ( nPos >= (int)CharPosMap_.size() )
	{
		// 文末
		if (blf) 
			*blf = true;

		auto c = CharPosMap_[CharPosMap_.size()-1];
		auto rc = c.rc ;

		if (c.lf )
		{
			rc.left = 0;
			rc.right =TSF_FIRST_POS;
			rc.Offset(0, rc.Height());		
		}
		else
		{
			rc.left = rc.right;
		}
		*prc = rc;
		return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------

int CTextLayout::CharPosFromPoint(const FPointF& pt)
{
	xassert( StarCharPos_ == 0 || (StarCharPos_ && this->bSingleLine_) );
		
	int j = 0;
	int lastone = -1;
	for( auto& it : CharPosMap_ )
	{
		auto rc = it.rc;

		if ( it.lf )
		{
			rc.Offset(0,rc.Height());
			rc.left = 0;
			rc.right = TSF_FIRST_POS;
		}

		if ( rc.top <= pt.y && pt.y <= rc.bottom )
		{
			lastone = j;

			if ( rc.PtInRect(pt) )
			{
				float w = rc.Width();
				if ( rc.left + (w*3/4) < pt.x )
					return j+1;

				return j;
			}
		}
		else if ( lastone != -1 )
			break;


		j++;
	}
	
	if (lastone == -1 )
		lastone = (int)CharPosMap_.size();

	return lastone+1; //-1;
}


//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------
CHPOS TSF::Real(const CHPOS& s) 
{
	CHPOS c(s);

	if ( c.lf )
	{
		c.col=0;
		c.lf= true;
		c.row++;
		c.rc.Offset(0,c.rc.Height());
		c.rc.left = 0;
		c.rc.right = TSF_FIRST_POS;
	}

	return c;
}
UINT CTextLayout::FineFirstEndCharPosInLine(UINT uCurPos, BOOL bFirst)
{
	_ASSERT(CharPosMap_.size() != 0 );

	auto c =  CharPosMap_[ min( uCurPos, (UINT)CharPosMap_.size()-1) ];

    if (bFirst)
	{
		// 先頭列へ
		UINT ret = min( uCurPos, (UINT)CharPosMap_.size()-1);
		
		while( 0 != ret )
		{
			auto c2 = CharPosMap_[ret];
			if ( c.row != c2.row )
			{
				ret++;
				break;
			}
			ret--;
		}
		return ret;

	}
	else
	{	
		// 行の最後尾
		UINT i = uCurPos;
		UINT ret=0;
		while( i < CharPosMap_.size() )
		{
			auto c2 = CharPosMap_[i];

			if ( c.row != c2.row || c2.lf )
				break;

			ret = i;
			i++;
		}
		return ret+1;

	}
    return (UINT)(-1);
}

void CTextLayout::GetTextLayout( IDWriteTextLayout** ppt )
{ 
	*ppt = DWTextLayout_; 
	DWTextLayout_->AddRef();
}

//----------------------------------------------------------------
//
// memory clear
//
//----------------------------------------------------------------

void CTextLayout::Clear()
{
    nLineCnt_ = 0;

	if ( DWTextLayout_ )
	{
		DWTextLayout_->Release();
		DWTextLayout_ = nullptr;
	}


	CharPosMap_.clear();
}

