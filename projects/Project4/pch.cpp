#include "pch.h"

#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"dwrite.lib") 
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib,"d3d11.lib")



void Trace(LPCWSTR fm, ...)
{
	if (!fm) return;
	va_list ap;
	va_start(ap, fm);
	size_t len = _vscwprintf(fm, ap) + 1;
	WCHAR* buffer = new WCHAR[len];
	int nWritten = _vswprintf_l(buffer, len, fm, 0, ap);
	if (nWritten > 0)
		buffer[len - 1] = 0;
	va_end(ap);
	::OutputDebugString(buffer);
	delete [] buffer;
}