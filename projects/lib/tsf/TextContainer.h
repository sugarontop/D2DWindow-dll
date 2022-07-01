#pragma once

namespace TSF {

class UndoTextEditor
{
	public :
		UndoTextEditor(){ };

		struct BInfo
		{
			BInfo():len(0),caretpos(0),enable(true){}

			std::shared_ptr<WCHAR[]> p;
			UINT len;
			UINT caretpos;
			bool enable;
		};

	private :
		std::stack<BInfo> undo_;		

	public :
		BInfo Undo();
		void AddChar(UINT pos, UINT len);
		void Delete(LPCWSTR str, UINT pos0, UINT pos1);
		void Clear();
};


class CTextContainer
{
public:
	CTextContainer(); 
	virtual ~CTextContainer();

	BOOL InsertText(int nPos, const WCHAR *psz, UINT nCnt, UINT& nResultCnt, bool undo_process=true);
	BOOL RemoveText(int nPos, UINT nCnt, bool undo_process=true);
	UINT GetText(int nPos, WCHAR *psz, UINT nBuffSize);
	UINT GetTextLength() const {return nTextSize_;}
	const WCHAR *GetTextBuffer() const {return psz_;}
	void Clear();
	void Reset();

	int SelStart() const { return nSelStart_; }
	int SelEnd() const { return nSelEnd_; }

	void SetSelStart(int a){ nSelStart_ = a; }
	void SetSelEnd(int a){ nSelEnd_ = a; }


	LONG AddTab(int row, bool bAdd );
	int  LineNo(LONG nPos) const;

	UndoTextEditor::BInfo Undo();

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
	std::shared_ptr<UndoTextEditor> undo_;

};
};
