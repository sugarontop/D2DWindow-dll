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
	LimitCharCnt_ = 65500;	
	nStartCharPos_ = 0;
	psz_ = 0;
	nTextSize_ = 0;
	view_size_ = {0};
	nBufferCharCount_ = 0;

	EnsureBuffer(MINI_BUFFER_SIZE);
}
CTextContainer::~CTextContainer()
{
	Clear();
}

BOOL CTextContainer::InsertText(int nPos, const WCHAR *psz, UINT nCnt, UINT& nResultCnt)
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

	

	nResultCnt = nCnt;
	return TRUE;
}

BOOL CTextContainer::RemoveText(int nPos, UINT nCnt)
{ 
	if (!nCnt)
		return TRUE;

	if (nPos + nCnt - 1 > nTextSize_)
		nCnt = nTextSize_ - nPos;


	auto start =  psz_ + nPos;
	auto end =  psz_ + nPos + nCnt;

	memmove( start, end,  sizeof(WCHAR)*(nTextSize_-(nPos+nCnt)));

	nTextSize_ -= nCnt;
	return TRUE;
}

BOOL CTextContainer::GetText(int nPos, WCHAR *psz, UINT nCnt)
{ 
	if (!nCnt)
		return FALSE;

	auto start =  psz_ + nPos;

	if (nPos + nCnt - 1 > nTextSize_)
		nCnt = nTextSize_ - nPos;

	memcpy(psz, start, sizeof(WCHAR)*nCnt);

	return TRUE;
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
