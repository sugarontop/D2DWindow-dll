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

class D2DControl;

struct ControlMapItem
{
	D2DControl* p;
	int row;
	int col;
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

		LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
		void ForceWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT CheckPostMessage(AppBase* pb);

		int MessageBox(const FRectF& rc, LPCWSTR text, LPCWSTR title);
		int FloatingMenu(LPVOID sender,const FRectF& rc, std::vector<MenuItem>& items);


		void ListUp(std::vector<ControlMapItem>& ar);
		void ResourceUpdate(bool bCreate);

		FRectF GetClientRect() const;

		HWND GetHwnd() const { return hWnd_; }
		D2DContextEx& GetContext(){ return cxt_; }
		
		TSFIsland tsf_;
		
		std::shared_ptr<D2DControls> top_control_;
		std::map<std::wstring, D2DControl*> name_map_;
		std::function<int(D2DWindow*, int)> Smooth_;


	protected :
		LRESULT InnerWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
		bool TSFInit(HWND hWnd);
		void TSFExit();

	protected :
		HWND hWnd_;
		D2DContextEx cxt_;
		
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