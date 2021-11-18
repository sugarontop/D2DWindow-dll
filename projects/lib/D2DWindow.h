#pragma once
#include "AppBase.h"
#include "D2DContext.h"

namespace V6 {

class D2DControl;
class D2DControls;



struct TSFIsland
{
	HWND hWnd;
	TfClientId TfClientId;
	ComPTR<ITfThreadMgr2> pThreadMgr; 
	ComPTR<ITfKeystrokeMgr> pKeystrokeMgr;
	ComPTR<ITfDisplayAttributeMgr> DisplayAttributeMgr;	
	void* ctrl;
};



class D2DWindow
{
	friend class D2DControl;
	public :
		D2DWindow();
		D2DWindow(HWND hwnd);
		~D2DWindow();

		LRESULT SendMessage(UINT msg, WPARAM wp, LPARAM lp );
		
		LRESULT PostMessage(UINT msg, WPARAM wp, LPARAM lp)
		{
			EnterCriticalSection( &message_lock_ );
			{
				MSG m(msg,wp,lp);
				post_message_queue_.push_back( m );
			}
			LeaveCriticalSection(&message_lock_);

			return 0;
		}

		bool CreateResource( FSizeF* size );

		HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
		void ForceWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);

		int MessageBox(const FRectF& rc, LPCWSTR text, LPCWSTR title);
		int FloatingMenu(LPVOID sender,const FRectF& rc, std::vector<MenuItem>& items);


		FRectF GetClientRect() const
		{
			RECT rc;
			::GetClientRect(hWnd_, &rc);
			return FRectF( (float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom );
		}

		HWND GetHwnd() const { return hWnd_; }

		V6::D2DContextEx cxt;
		TSFIsland tsf;
		
		std::shared_ptr<D2DControls> top_control_;
		std::map<std::wstring, D2DControl*> name_map_;
	protected :
		HRESULT InnerWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
		bool TSFInit(HWND hWnd);
		void TSFExit();

	protected :
		HWND hWnd_;
		
		std::stack<D2DControl*> capture_obj_;
		std::vector< std::shared_ptr<D2DControl>> death_objects_;
		

		struct MSG
		{	
			MSG(UINT m, WPARAM wp1, LPARAM lp1):msg(m),wp(wp1),lp(lp1){}
			UINT msg;
			WPARAM wp;
			LPARAM lp;
		};
		std::vector<MSG> post_message_queue_;
		CRITICAL_SECTION message_lock_;
};











};