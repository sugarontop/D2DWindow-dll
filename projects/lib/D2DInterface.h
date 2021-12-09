#pragma once

namespace V6
{
	class D2DContext;


	typedef  void (*DelegateDrawFunc)(LPVOID captureobj, D2DContext& cxt);
	typedef LRESULT(*DelegateProcFunc)(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);


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