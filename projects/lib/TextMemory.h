#pragma once

class TextMemory;
class TextSentence
{
	friend class TextMemory;
public :
	TextSentence();
	TextSentence(TextMemory* mem);
	//TextSentence( const std::wstring& str);

	LPCWSTR get();
	std::wstring_view str();
	TextMemory* getbuffer() { return parent_; }
protected :
	TextMemory* parent_;
	UINT_PTR off_head_;
	USHORT count_;
	USHORT cblen_;

	std::wstring* temp_;

	bool alive_;
};
class TextMemory
{
	friend class TextSentence;
public :
	TextMemory();
	~TextMemory();

	void Alloc( DWORD sz );	
	TextSentence AddNewString( LPCWSTR str );
	void Free( TextSentence str );
	void Destroy();
	

protected :
	BYTE* Head(){ return (BYTE*)buf_; }
	void ReAlloc( DWORD sz);
protected :
	LPVOID buf_;
	UINT_PTR last_;
	UINT_PTR buf_len_;

	std::vector<TextSentence> reuse_;

};

