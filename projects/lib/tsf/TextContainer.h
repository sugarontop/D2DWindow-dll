#pragma once

namespace TSF {
class CTextContainer
{
public:
	CTextContainer(); 
	virtual ~CTextContainer();

	BOOL InsertText(int nPos, const WCHAR *psz, UINT nCnt, UINT& nResultCnt);
	BOOL RemoveText(int nPos, UINT nCnt);
	UINT GetText(int nPos, WCHAR *psz, UINT nBuffSize);
	UINT GetTextLength() const {return nTextSize_;}
	const WCHAR *GetTextBuffer() const {return psz_;}
	void Clear();
	void Reset();

	int SelStart() const { return nSelStart_; }
	int SelEnd() const { return nSelEnd_; }

	void SetSelStart(int a){ nSelStart_ = a; }
	void SetSelEnd(int a){ nSelEnd_ = a; }

private :
	int nSelStart_, nSelEnd_;
public :
	bool bSelTrail_;
	V6::FRectF rc_;
	SIZE view_size_;
	bool bSingleLine_;
	
	V6::FPointF offpt_; // Textbox2ì‡ÇÃï∂éöÇÃà⁄ìÆÅASinglelineÇ≈èdóv
	int nStartCharPos_;
private:
	BOOL EnsureBuffer(UINT nNewTextSize);
	const UINT LimitCharCnt_ = 65000;

	WCHAR* psz_;

	UINT nBufferCharCount_;
	UINT nTextSize_;


};
};
