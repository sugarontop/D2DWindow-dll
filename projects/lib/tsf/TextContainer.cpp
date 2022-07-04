#include "pch.h"
#include "TextContainer.h"

using namespace TSF;
using namespace V6;

#define MINI_BUFFER_SIZE	16

CTextContainer::CTextContainer()
{
	nSelStart_ = nSelEnd_ = 0;
	bSelTrail_ = false;
	bSingleLine_ = false;
	
	nStartCharPos_ = 0;
	psz_ = 0;
	nTextSize_ = 0;
	view_size_ = {0};
	nBufferCharCount_ = 0;
	undo_ = std::make_shared<UndoTextEditor>();
	ime_stat_ = 0;

	EnsureBuffer(MINI_BUFFER_SIZE);
}
CTextContainer::~CTextContainer()
{
	Clear();
}


UndoTextEditor::BInfo CTextContainer::Undo()
{
	return undo_->Undo();
}

BOOL CTextContainer::InsertText(int nPos, const WCHAR *psz, UINT nCnt, UINT& nResultCnt, bool undo_process)
{ 	
	if ( LimitCharCnt_ < GetTextLength() + nCnt )
	{
		nCnt = LimitCharCnt_ - GetTextLength();
	}
	else if ( nBufferCharCount_ == 0 && nCnt == 0 )
	{		
		nBufferCharCount_ = MINI_BUFFER_SIZE;
		psz_ = new WCHAR[nBufferCharCount_];
		memset(psz_, 0, nBufferCharCount_*sizeof(WCHAR) );

		return TRUE;
	}

	if (!EnsureBuffer(nTextSize_ + nCnt))
	{
		return FALSE;
	}

	_ASSERT(nTextSize_ + nCnt < nBufferCharCount_);

	// move target area text to last.
	memmove(psz_ + nPos + nCnt, psz_ + nPos, (nTextSize_ - nPos) * sizeof(WCHAR));
	
	// add new text
	memcpy(psz_ + nPos, psz, nCnt * sizeof(WCHAR));

	nTextSize_ += nCnt;

	psz_[nTextSize_] = 0;

	if ( undo_process )
		undo_->AddChar( nPos, nCnt, ime_stat_ ); // ime_stat_: [0 ansi input, 1-3 ime imput]

	nResultCnt = nCnt;
	return TRUE;
}

BOOL CTextContainer::RemoveText(int nPos, UINT nCnt, bool undo_process)
{ 
	if (!nCnt)
		return TRUE;

	if (nPos + nCnt - 1 > nTextSize_)
		nCnt = nTextSize_ - nPos;


	auto start =  psz_ + nPos;
	auto end =  psz_ + nPos + nCnt;

	if (undo_process)
		undo_->Delete(psz_, nPos, nPos+nCnt, ime_stat_);


	int cnt = (int)nTextSize_-(nPos+nCnt);

	if (cnt > 0 )
	{
		memmove( start, end,  sizeof(WCHAR)*cnt );
	}

	nTextSize_ -= nCnt;

	return TRUE;
}

UINT CTextContainer::GetText(int nPos, WCHAR *psz, UINT nCnt)
{ 
	if (!nCnt)
		return 0;

	auto start =  psz_ + nPos;

	if (nPos + nCnt - 1 > nTextSize_)
		nCnt = nTextSize_ - nPos;

	memcpy(psz, start, sizeof(WCHAR)*nCnt);

	return nCnt;
}

void CTextContainer::Clear()
{ 
	delete [] psz_;
	psz_ = nullptr;
	nBufferCharCount_ = 0;
	nTextSize_ = 0;

	nSelStart_= nSelEnd_=0;
	bSelTrail_ = false;
	offpt_={};
	nStartCharPos_ = 0;
	undo_->Clear();
}
void CTextContainer::UndoAdjust()
{
	undo_->UndoAdjust();

}
void CTextContainer::Reset()
{
	Clear();
	EnsureBuffer(MINI_BUFFER_SIZE);
}

BOOL CTextContainer::EnsureBuffer(UINT nNewTextSize)
{ 
	if ( nNewTextSize != MINI_BUFFER_SIZE )
		nNewTextSize = (max(256, nNewTextSize) + 255)/256*256;
	
	if ( nBufferCharCount_ < nNewTextSize )
	{
		auto psz2 = new WCHAR[nNewTextSize+1];

		if ( psz2 == nullptr )
			return FALSE;

		memcpy(psz2,psz_,nBufferCharCount_*sizeof(WCHAR));

		delete [] psz_;
		psz_ = psz2;
		nBufferCharCount_ = nNewTextSize+1;

		return TRUE;
	}
	return TRUE;
}

LONG CTextContainer::AddTab(int row, bool bAdd )
{
	UINT nResultCnt;
	int irow = 0, pos = 0;

	auto ch = psz_[pos];
	while(ch!=0)
	{
		if (row == irow)
			break;
		
		ch = psz_[pos];
		if (ch == L'\n')
			irow++;
		pos++;
	}

	if ( bAdd )
		InsertText(pos, L"\t",1, nResultCnt);
	else
		RemoveText(pos, 1);

	while( psz_[pos] != L'\n' && psz_[pos] != L'\0' )
	{
		pos++;
	}

	return pos;
}
int CTextContainer::LineNo(LONG nPos) const
{
	int ret = 0;
	for(int i=0; i < nPos; i++)
	{
		if (psz_[i] == L'\n')
			ret++;
	}
	return ret;
}


/////////////////////////////////////////////////////////////

void UndoTextEditor::AddChar(UINT pos,UINT len, byte stat)
{	
	BInfo b;
	b.caretpos = pos;
	b.len = len;
	b.p = nullptr;
	b.stat = stat * 10; // 入力系は1->10, 2->20, 3->30へ変更

	undo_.push(b);

}

void UndoTextEditor::UndoAdjust()
{
	// IME ONして入力開始のstat=10までさかのぼってundo情報を消す。
	auto& undo2 = undo_;

	std::vector<BInfo> ar;

	while( !undo2.empty() )
	{
		auto b = undo2.top();
		
		ar.push_back(b);

		_ASSERT( ar[0].stat == 20 );

		if ( b.stat == 10 )
		{
			undo2.pop();

			undo2.push( ar[0] );
			break;
		}
		undo2.pop();
		
	}

}
UndoTextEditor::BInfo UndoTextEditor::Undo()
{
	BInfo b;	

	while( !undo_.empty() )
	{
		b = undo_.top();

		if ( b.stat != 2 )
		{
			undo_.pop();
			break;
		}

		undo_.pop();
	}

	return b;
}
void UndoTextEditor::Delete(LPCWSTR str, UINT pos0, UINT pos1, byte stat)
{
	_ASSERT( pos0 <= pos1 );
	
	if ( pos0 == pos1) return;
	
	BInfo b;

	WCHAR* cb = new WCHAR[pos1-pos0+1];
	memcpy(cb, str+pos0, (pos1-pos0)*sizeof(WCHAR));
	cb[pos1-pos0] = 0;

	b.p = std::shared_ptr<WCHAR[]>(cb);
	b.len = (int)pos1- (int)pos0;
	b.caretpos = pos0;
	b.stat = stat;

	undo_.push(b);
}

void UndoTextEditor::Clear()
{
	std::stack<BInfo> empty;

	undo_ = empty;

}