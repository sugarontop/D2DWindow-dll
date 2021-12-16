#pragma once



struct ResourceItem
{
	std::wstring fnm;
	DWORD pos;
	DWORD len;
};

struct ResourceBin
{
	ComPTR<IStream> sm;	
	std::map<std::wstring, ResourceItem> items;

};

bool LoadResource(LPCWSTR fnm, ResourceBin* rb);
bool GetResourcePng(ResourceBin& src, LPCWSTR nm, ID2D1RenderTarget* target, ID2D1Bitmap** pbmp);
