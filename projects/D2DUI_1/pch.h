// pch.h: プリコンパイル済みヘッダー ファイルです。
// 次のファイルは、その後のビルドのビルド パフォーマンスを向上させるため 1 回だけコンパイルされます。
// コード補完や多くのコード参照機能などの IntelliSense パフォーマンスにも影響します。
// ただし、ここに一覧表示されているファイルは、ビルド間でいずれかが更新されると、すべてが再コンパイルされます。
// 頻繁に更新するファイルをここに追加しないでください。追加すると、パフォーマンス上の利点がなくなります。

#ifndef PCH_H
#define PCH_H

// プリコンパイルするヘッダーをここに追加します
#include "framework.h"


// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <unordered_map>

#include <string>
#include <stack>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <fcntl.h>
#include <io.h>
#include <sstream>
#include <codecvt>
#include <dispex.h>
#include <limits.h>
#include <functional>
#include <memory>
#include <type_traits>
#include <algorithm>
#include <strsafe.h>

#include <msxml6.h>
#import <msxml6.dll> 

#include <comutil.h>
#include <mmsystem.h>
#include <wincodec.h>
#include <algorithm>
#include <string_view>

#include <imm.h>
#include <msctf.h>


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

#include "D2DMisc.h"
#include "comptr.h"

void Trace(LPCWSTR fm, ...);

#define TRACE	 Trace
typedef _bstr_t BS;
#define HEART_BEET_ID 1000

#pragma warning( disable : 26812 )
#pragma warning( disable : 26495 )

#define LOCALE	L"ja-jp"

//
//inline void ThrowIfFailed( LRESULT hr, LPCWSTR msg, UINT line, LPCSTR fnm )
//{
//	if (FAILED(hr))
//	{		
//		//General access denied error 0x80070005 
//
//
//		int a = 0;
//	}
//}
//#define THR(hr) ThrowIfFailed(hr,nullptr, __LINE__, __FILE__) 



#endif //PCH_H
