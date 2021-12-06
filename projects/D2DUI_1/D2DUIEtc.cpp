#include "pch.h"
#include "D2D1UI_1.h"


typedef std::vector<LPVOID> list;

DLLEXPORT HANDLE DACreateList()
{
	return new list();
}
DLLEXPORT bool  DAAddList(HANDLE h, void* p)
{
	_ASSERT(h);
	
	list& ls = *(list*)h;

	ls.push_back(p);

	return true;
}
DLLEXPORT UINT  DAListCount(HANDLE h)
{
	_ASSERT(h);
	list& ls = *(list*)h;
	return ls.size();
}
DLLEXPORT void* DAGet(HANDLE h,UINT idx)
{
	_ASSERT(h);
	list& ls = *(list*)h;

	_ASSERT(idx < ls.size());
	return *(ls.begin()+idx); 
}
DLLEXPORT void DADestroyList(HANDLE h)
{
	_ASSERT(h);
	list* pls = (list*)h;
	pls->clear();
	delete pls;
}
