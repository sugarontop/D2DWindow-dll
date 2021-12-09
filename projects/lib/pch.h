#pragma once

#include <windows.h>

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <unordered_map>

//#include <atlbase.h>


#include <string>
#include <stack>
#include <sstream>
#include <vector>
#include <map>
#include <set>

#include <fcntl.h>
#include <io.h>
//#include <WinSock2.h>
//#include <Ws2tcpip.h>
#include <sstream>
#include <codecvt>
#include <dispex.h>
#include <limits.h>
#include <functional>
#include <memory>
#include <type_traits>
//#include <fstream>

#include <imm.h>
#include <msctf.h>

#include <msxml6.h>
#define MSXML	L"MSXML2.DOMDocument.6.0"	// CLSID clsid; hr = CLSIDFromProgID( MSXML, &clsid );CComPtr<IXMLDOMDocument> xml;	hr = CoCreateInstance( clsid, NULL,CLSCTX_ALL,IID_IXMLDOMDocument,(void**)&xml );
#define MSXMLHTTP L"MSXML2.XMLHTTP.6.0"	// 自動キャッシュが効く　CComPtr<IXMLHTTPRequest> req; CLSIDFromProgID( MSXMLHTTP, &clsid ); hr = CoCreateInstance( clsid,NULL,CLSCTX_INPROC_SERVER,IID_IXMLHTTPRequest,(LPVOID*)&req );

#include <comutil.h>
#include <mmsystem.h>
#include <wincodec.h>
#include <algorithm>

#define USE_ID2D1DEVICECONTEXT

#ifdef USE_ID2D1DEVICECONTEXT

	#ifdef USE_WIN8_METRO_CONTEXT
		#include <d3d11_2.h>
		#include <d2d1_2.h>		
		#include <dwrite_2.h>
		#include <d2d1_2helper.h>
		#include <d2d1effects_1.h>
	#else
		#include <d3d11_1.h>
		#include <d2d1_1.h>
		#include <dwrite_1.h>
		#include <d2d1_1helper.h>
		
	#endif
#else
	#include <d2d1_1.h>
	#include <dwrite_1.h>
	#include <d2d1helper.h>
#endif

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
using namespace std::placeholders;
#include <string_view>
#include "D2DMisc.h"

void Trace(LPCWSTR fm, ...);

#define TRACE	 Trace

#include "comptr.h"
inline void ThrowIfFailed( LRESULT hr, LPCWSTR msg, UINT line, LPCSTR fnm )
{
	if (FAILED(hr))
	{		
		//General access denied error 0x80070005 


		int a = 0;
	}
}
#define THR(hr) ThrowIfFailed(hr,nullptr, __LINE__, __FILE__)





