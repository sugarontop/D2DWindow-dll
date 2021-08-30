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
	//TSF::CTextEditorCtrl* D2DTextbox::ctrl();
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
		LRESULT PostMessage(UINT msg, WPARAM wp, LPARAM lp);

		bool CreateResource( FSizeF* size );

		HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
		void ForceWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);

		int MessageBox(LPCWSTR text);
		HWND GetHwnd() const { return hWnd_; }

		V6::D2DContextEx cxt;
		TSFIsland tsf;
		
		std::shared_ptr<D2DControls> top_control_;
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