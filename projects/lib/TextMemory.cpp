#include "pch.h"
#include "textmemory.h"


TextMemory::TextMemory():buf_(0),last_(0)
{ 

}
TextMemory::~TextMemory()
{
	Destroy();
}

void TextMemory::Alloc( DWORD sz )
{ 
	buf_len_ = sz;
	buf_ = ::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY|HEAP_NO_SERIALIZE, sz);
}

void TextMemory::ReAlloc( DWORD sz)
{ 	
	buf_ = ::HeapReAlloc(::GetProcessHeap(), HEAP_NO_SERIALIZE, buf_,  sz);
	buf_len_ = sz;
}


TextSentence TextMemory::AddNewString( LPCWSTR str )
{ 
	UINT len = (UINT)wcslen(str) + 1;

	if ( len > 50 )
	{
		int a = 0;

	}


	for(auto it = reuse_.begin(); it != reuse_.end();  )
	{
		if ( (len-1) <  it->cblen_ ) //&& (it->cblen_)/2 < (len-1) }}  )
		{
			TextSentence t = (*it);
			t.count_ = len-1;
			t.alive_ = true;

			memcpy( (byte*)buf_ + t.off_head_, str, sizeof(WCHAR)*len);
			reuse_.erase(it);

			TRACE( L"TextMemory::AddNewString reuse str=%s  nokori=%d\n" ,  str, reuse_.size()-1  );

			return t;
		}

		++it;
	}

	TextSentence h(this);
	while(1)
	{
		h.count_ = len-1;
		h.cblen_ = max(16, h.count_+1  );
		h.cblen_ = (h.cblen_+7)/8*8;

		if (last_ +sizeof(WCHAR) * h.cblen_ <  buf_len_ )
		{
			h.off_head_ = last_;			
			h.alive_ = true;

			memset( (byte*)buf_ + last_, 0, sizeof(WCHAR) * h.cblen_);
			memcpy( (byte*)buf_ + last_, str, sizeof(WCHAR)*len);
			last_ +=sizeof(WCHAR) * h.cblen_;

			TRACE( L"TextMemory::AddNewString last=%d, str=%s\n" , last_ , str);

			break;
		}
		else
		{
			DWORD newsz = static_cast<DWORD>(buf_len_ * 1.3f);

			newsz = (newsz+255)/256*256;
			TRACE( L"TextMemory::AddNewString Expand=%d, str=%s\n" , newsz, str);

			ReAlloc(newsz);
		}
	}
	return h;
}

void TextMemory::Destroy()
{
	reuse_.clear();
	HeapFree(GetProcessHeap(), 0, buf_);
	buf_ = nullptr;
	last_ = 0;
}

void TextMemory::Free( TextSentence str )
{ 
	if ( str.alive_ && str.parent_)
	{
		BYTE* p = (BYTE*)str.get();
		memset(p, 0, str.cblen_*sizeof(WCHAR));

		str.alive_ = false;
		reuse_.push_back(str);

		static int a = 0;
		if (reuse_.size() > 4 && a > 10)
		{
			std::sort(reuse_.begin(), reuse_.end(), [](const TextSentence& a, const TextSentence& b)->bool{
				return (a.cblen_ < b.cblen_ );
			});

			a = 0;
		}
		a++;


		if (  reuse_.size() > 2 )
			TRACE( L"TextMemory::Free cnt=%d\n" , reuse_.size() );
	}
		
}
/////////////////////////////////////////////////////////////

TextSentence::TextSentence(TextMemory* mem)
{	
	parent_ = mem;
	off_head_ = 0;
	count_ = 0;
	cblen_ = 0;
	alive_ = false;
	temp_ = nullptr;
}

TextSentence::TextSentence()
{
	parent_ = nullptr;
	off_head_ = 0;
	count_ = 0;
	cblen_ = 0;
	alive_ = false;
}
LPCWSTR TextSentence::get()
{ 
	if ( alive_ == false )
	{
		static WCHAR ch = 0;
		return &ch;
	}
	if (temp_)
		return temp_->c_str();

	return (LPCWSTR)(parent_->Head() + off_head_);
}
std::wstring_view TextSentence::str()
{
	return std::wstring_view( get() );

}

