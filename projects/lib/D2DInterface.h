#pragma once

namespace V6
{
	class D2DContext;


	typedef  bool (*DelegateDrawFunc)(LPVOID captureobj, D2DContext& cxt);
	typedef LRESULT(*DelegateProcFunc)(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
	typedef DWORD(*DelegateClick)(void* sender,LPCWSTR funcnm, void* p );

	class XDropdownListBox
	{
		public :
			XDropdownListBox(){};
			virtual void xAddItem( int idx, std::wstring title )=0;
			virtual int xItemCount() = 0;
			virtual int xGetSelect() = 0;
			virtual bool xSetSelect(int idx)=0;
			virtual void xClear()=0;
	};




};