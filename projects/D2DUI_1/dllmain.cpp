// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "pch.h"


void D2DInstanceExit();


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		break;
	
	
	
		case DLL_PROCESS_DETACH:
			D2DInstanceExit();
		break;
    }
    return TRUE;
}

