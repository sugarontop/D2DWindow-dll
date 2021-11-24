#pragma once


#include "common.h"
#include "D2DCharRect.h"

#include "..\D2DContext.h"

#define TSF_FIRST_POS 1

namespace TSF {

struct COMPOSITIONRENDERINFO 
{
    int nStart;
    int nEnd;
    TF_DISPLAYATTRIBUTE da;
};
//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------


struct CHPOS
{
	CHPOS():pos(0),row(0),col(0),lf(false){};

	int pos;
	int row;
	int col;
	bool lf;
	FRectF rc;

};

CHPOS Real(const CHPOS& s) ;


//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------

class CTextLayout
{
	public:
		CTextLayout();
		virtual ~CTextLayout();

		BOOL Layout(D2DContext& hdc, const WCHAR *psz,  int nCnt, const SIZE& sz, bool bSingleLine, int nSelEnd,int& StarCharPos, IDWriteTextFormat* fmt);		
		BOOL Render(D2DContext& hdc, const FRectF& rc, const WCHAR *psz,  int nCnt, int nSelStart, int nSelEnd,bool bSelTrail,const COMPOSITIONRENDERINFO *pCompositionRenderInfo, UINT nCompositionRenderInfo);

	public :
		int CharPosFromPoint(const FPointF& pt);
		BOOL RectFromCharPos(UINT nPos, FRectF *prc);
		BOOL RectFromCharPosEx(int nPos, FRectF *prc, bool* blf);

		UINT FineFirstEndCharPosInLine(UINT uCurPos, BOOL bFirst);

		float GetLineHeight() const;
		float GetLineWidth() const { return row_width_; }

		void Password( bool bl ){ bPassword_ = bl; }

		DWRITE_TEXT_METRICS GetTextMetrics() const { return tm_; }

		BOOL GetCharInfo( int pos, CHPOS* info );
		std::vector<CHPOS> CharPosMap_;
		
		void GetTextLayout( IDWriteTextLayout** ppt );
		const FRectF* GetCharRects2(int* len);

		bool bRecalc_;
		D2D1_COLOR_F selected_halftone_color_;
	private:
		void Clear();
		
		UINT nLineCnt_;
		float nLineHeight_;
		LPCWSTR str_;
		FLOAT row_width_;
		bool bSingleLine_;
		bool bPassword_;
		int StarCharPos_;
		IDWriteTextLayout* DWTextLayout_;

		DWRITE_TEXT_METRICS tm_;	
		CharsRectF char_rectf_; //cr;
		
};

};

