#pragma once


struct DocFunc
{
	BS title;
	BS param;
	BS note;

};


bool LoadDocument(LPCWSTR fnm, std::vector<DocFunc>& ar);
bool SaveDocument(int idx, DocFunc doc);