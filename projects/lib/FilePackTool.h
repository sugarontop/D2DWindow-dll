#pragma once


namespace FILEPACK
{
	struct Item
	{
		std::wstring fnm;
		UINT32 size;
		UINT32 pos;
	};



	void some_file_to_files(std::wstring dst, std::wstring fnms, std::vector<Item>& ar);
	bool CreateSerializeBin( std::vector<Item>& ar,  IStream** psm );

	bool Convert2File(LPCWSTR fnm,DWORD magic_number, IStream* src);

	bool Convert2Stream(LPCWSTR fnm,DWORD magic_number, IStream** dst);
	bool ListContents(IStream* src, std::vector<Item>& ar);

	bool DeSerialize( IStream* sm, Item s, IStream** psm );


	bool Compress(IStream* src, IStream** psm );
	bool DeCompress(IStream* src, IStream** psm );


	struct FileHead
	{
		DWORD magic_number;
		WORD version;
		WORD HeadCount;
	};

	struct Head
	{
		bool enable;
		WCHAR fnm[260];
		DWORD pos;
		DWORD len;
	};

	#define MAGIC_NUMBER	1001


}
