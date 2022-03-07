#include "pch.h"
#include "D2DColor.h"

using namespace V6;
bool D2DColor::ColorFromString( LPCWSTR s, DWORD* ret )
{
	DWORD clr = 0;
	const WCHAR* p = (WCHAR*)s;

	if ( *p++ == '#' )
	{
		auto my_wstoi = [](WCHAR ch)->DWORD
		{
			if ( L'0' <= ch && ch <= L'9' )
				return ( ch-L'0');
			else if ( L'a' <= ch && ch <= L'f' )
				return ( ch-L'a') + 10;
			else if ( L'A' <= ch && ch <= L'F' )
				return ( ch-L'A') + 10;
			return 0;
		};

		int len = lstrlen(p);
		int cnt = ( len == 8 ? 4 : 3);
		if ( len== 6 || len == 8 )
		{
			DWORD x[4];

			for(int i = 0; i < cnt;i++ )
			{
				DWORD a1 = my_wstoi(*p++);
				DWORD a2 = my_wstoi(*p++);
				x[i] = a1*16+a2;
			}

			if ( cnt == 3 )
				*ret = (x[0] << 16 | x[1] << 8 | x[2]);
			else if ( cnt == 4 )
				*ret = (x[0] << 24 | x[1] << 16 | x[2] << 8 | x[3]);

			return true;
		}
	}
	return false;
}
