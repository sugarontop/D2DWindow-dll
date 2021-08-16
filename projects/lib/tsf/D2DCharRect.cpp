
#include "pch.h"
#include "common.h"
#include "D2DMisc.h"
#include "D2DCharRect.h"
#define TAB_WIDTH_4CHAR 4
#define TSF_FIRST_POS 1
//-------------------------------------------------------------------------------------------------------------------
//
//
//
//-------------------------------------------------------------------------------------------------------------------
CharsRectF::CharsRectF()
{
	layout_ = nullptr;
	cnt_ = 0;
	rects_ = NULL;
	line_height_ = 0;
	bSingleLine_ = true;

}
void CharsRectF::Set(IDWriteTextLayout* tx, bool bSingle)
{
	layout_ = tx;
	cnt_ = 0;
	rects_ = nullptr;
	line_height_ = 0;
	bSingleLine_ = bSingle;
}

CharsRectF::CharsRectF(IDWriteTextLayout* tx, bool bSingle  )
{
	layout_ = tx;
	cnt_ = 0;
	rects_ = nullptr;
	line_height_ = 0;
	bSingleLine_ = bSingle;
}
void CharsRectF::Clear()
{
	//delete [] rects_;
	cnt_ = 0;
	line_height_ = 0;
}



void Set(IDWriteTextLayout* tx, bool bSingle);



const FRectF* CharsRectF::Create(LPCWSTR str, const FSizeF&  sz, int slen, int* plen)
{
	int mkcnt_ = ( ( str == nullptr || slen == 0 ) ? 1 : slen );
	
	float x, y;

	if ( layout_ )
	{				
		DWRITE_HIT_TEST_METRICS tm;
		layout_->HitTestTextPosition(0, false, &x, &y, &tm);
		line_height_ = tm.height;		
	}
	
	cnt_ = slen;
	rects_ = std::shared_ptr<FRectF[]>( new FRectF[ mkcnt_ ] );
	ZeroMemory( rects_.get(), sizeof(FRectF)*mkcnt_ );

	int r = 0;
	for (int i = 0; i < slen; i++)
	{
		float x1,y1;
		DWRITE_HIT_TEST_METRICS tm;

		layout_->HitTestTextPosition(i, false, &x1, &y1, &tm);


		if ( bSingleLine_ && y1 != y )
			break;

		FRectF rc;
		rc.SetRect(tm.left, tm.top, FSizeF(tm.width, tm.height));

		if ( str[i] == '\n' || str[i] == '\r' )
		{
			rc.SetWidth(TSF_FIRST_POS);
	
		}
			

		rects_[i] = rc;
		r++;
	}

	*plen = r;
	
	return rects_.get();

}

////////////////////////////////////////////////////////////////////////////////////////////
int CreateTextLayout::Create( int zStartPos, int zCaretPos, IDWriteTextLayout** layout)
{
	if ( IsSibleLine_ )
		return CreateSingleTextLayout(zStartPos,zCaretPos,layout);
	else
		return CreateMultiTextLayout(zStartPos,zCaretPos,layout);
}
int CreateTextLayout::CreateMultiTextLayout( int zStartPos, int zCaretPos, IDWriteTextLayout** layout)
{
	if ( fc_&& S_OK == fc_->CreateTextLayout( psz_+zStartPos, len_-zStartPos, fmt_, sz_.width, sz_.height, layout ))
	{
		
		LPCWSTR x = L"TABWIDTH";
		ComPTR<IDWriteTextLayout> tl;
		if ( S_OK == fc_->CreateTextLayout( x, TAB_WIDTH_4CHAR, fmt_, 1000, sz_.height,  &tl ))
		{
			DWRITE_TEXT_METRICS t;
			tl->GetMetrics(&t);
			
			(*layout)->SetIncrementalTabStop( t.width ); //タブの距離設定 tab is 4 chars
		}
	}

	return 0;
}

int CreateTextLayout::CreateSingleTextLayout( int zStartPos, int zCaretPos, IDWriteTextLayout** playout)
{
	int viewlen = 0;
	float width9999 = 9999;
	int off = 0;

	// 表示可能文字数を算出する。
	if ( len_ && (int)len_ - zStartPos >= 0)
	{
		ComPTR<IDWriteTextLayout> temp;
		auto hr = fc_->CreateTextLayout( psz_+zStartPos, len_-zStartPos, fmt_, width9999, sz_.height, &temp );

		float wd = 0;
		const float triming = 21.0f;	// ???
		for (UINT i = 0; i < len_; i++)
		{
			float x1=0,y1=0;
			DWRITE_HIT_TEST_METRICS tm;
			hr = temp->HitTestTextPosition(i, false, &x1, &y1, &tm);
		
			if ( (wd+triming) > sz_.width )
				break;

			wd += tm.width;

			viewlen = i+1;
		}

		// キャレットから先頭文字の位置を算出
	}

	if ( len_ > 0 && viewlen == 0 )
		viewlen = 1;

	
	{
		off = 0;
		if ( len_ && zStartPos<=(int)len_)
		{
			SingleLineCaret sc( zStartPos,len_, viewlen, zCaretPos-zStartPos );
				
			sc.CaretOff(-1);
			sc.CaretOff(1);

			off = sc.zS();
		}
		int len = max(0, (int)len_-off );


		WCHAR karamoji[2] = {0};

		if ( psz_ == nullptr )
			psz_ = karamoji;

		auto hr = fc_->CreateTextLayout( psz_+off, len, fmt_, width9999, sz_.height, playout );
		
		_ASSERT(hr == S_OK);
	}
	
	return off;

}