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

	if (undo_process)
		undo_->AddChar( nPos, nCnt);

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
		undo_->Delete(psz_, nPos, nPos+nCnt);

	memmove( start, end,  sizeof(WCHAR)*(nTextSize_-(nPos+nCnt)));

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

void UndoTextEditor::AddChar(UINT pos,UINT len)
{	
	BInfo b;
	b.caretpos = pos;
	b.len = len;
	b.p = nullptr;

	undo_.push(b);

}

UndoTextEditor::BInfo UndoTextEditor::Undo()
{
	BInfo b;
	b.enable = false;

	if ( undo_.empty()) return b ;

	b = undo_.top();

	undo_.pop();

	return b;
}
void UndoTextEditor::Delete(LPCWSTR str, UINT pos0, UINT pos1)
{
	_ASSERT( pos0 <= pos1 );
	
	if ( pos0 == pos1) return;
	
	BInfo b;

	WCHAR* cb = new WCHAR[pos1-pos0+1];
	memcpy(cb, str+pos0, (pos1-pos0)*sizeof(WCHAR));
	cb[pos1-pos0] = 0;

	b.p = std::shared_ptr<WCHAR[]>(cb);
	b.len = pos1-pos0;
	b.caretpos = pos0;

	undo_.push(b);
}

void UndoTextEditor::Clear()
{
	std::stack<BInfo> empty;

	undo_ = empty;

}