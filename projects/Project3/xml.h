#pragma once


struct DocFunc
{
	BS title;
	BS param;
	BS note;

};

bool LoadDocument(LPCWSTR fnm, std::vector<DocFunc>& ar);
bool SaveDocument(int idx, DocFunc doc);


struct DocFuncItem
{		
	BS func;
	BS title;
	BS ret;
	USHORT jnr;
	BS note;
	std::vector<BS> prm;
};

bool Load_DLL_Document(LPCWSTR fnm, std::vector<DocFuncItem>& ar);