#include "pch.h"
#include "D2D1UI_1.h"
#include "D2DTextbox.h"
#include "D2DButton.h"
#include "D2DContext.h"
#include "D2DDropdownListbox.h"
#include "D2DWhiteControl.h"
#include "D2DClientControl.h"
#include "D2DStatic.h"
#include "D2DSquarePaper.h"
#include "D2DWhiteWindow.h"
#include "D2DMDISplitControls.h"
#include "D2DControls_with_Scrollbar.h"
#include "D2DAccordionbar.h"
using namespace V6;
#define  APP (D2DApp::GetInstance())
//UIHandle Renewal_UIHandle(  UIHandle h );
UIHandle ConvertUIHandle(D2DControl* p);

DLLEXPORT void D2DInitail(INT_PTR p )
{
	D2DApp::SetD2DAppForDLL((D2DApp*)p);


}

DLLEXPORT UIHandleWin D2DCreateMainHWnd( HWND hWnd,  float fontheight, int typ )
{
	auto win = new D2DWindow(hWnd);

	V6::D2DContextEx& cxt = win->cxt;
	FSizeF size;
	win->CreateResource( &size );

	cxt.CreateDeviceIndependentResources(fontheight);

	_ASSERT( typ == 0 );

	{
		auto topcontrol = std::shared_ptr<D2DControls>( new D2DControls());
		topcontrol->CreateControl( win, nullptr, FRectF(0.0f,0.0f, size), STAT_VISIBLE, L"_root" );	
		win->top_control_ = topcontrol;
	}
	/*else if ( typ == 1 )
	{
		auto topcontrol = std::make_shared<D2DControls_with_Scrollbar>();
		topcontrol->CreateControl( win, nullptr, FRectF(0.0f,0.0f, size), STAT_VISIBLE, L"_root" );	
		win->top_control_ = topcontrol;
	}*/



	UIHandleWin r;
	r.p = win;
	r.typ = TYP_MAIN_WINDOW;
	return r;

}
DLLEXPORT void D2DDestroyWindow(UIHandleWin main  )
{
	auto win = (D2DWindow*)main.p;
	delete win;
}

DLLEXPORT ID2D1DeviceContext* D2DGetDeviceContext(UIHandleWin main  )
{
	if ( main.typ == TYP_MAIN_WINDOW )
	{
		auto win = (D2DWindow*)main.p;
		return *(win->cxt);
	}
	return nullptr;
}
DLLEXPORT UIHandle D2DCreateWhiteWindow(UIHandleWin hwin, UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id )
{
	_ASSERT(hwin.p);
	_ASSERT(hctrls.p);

	auto pgtx = new D2DWhiteWindow(); 

	auto win = (D2DWindow*)hwin.p;
	auto ctrls = (D2DControls*)hctrls.p;

	pgtx->CreateControl(win,ctrls, rc, stat, name, id );
	ctrls->Add( std::shared_ptr<D2DWhiteWindow>(pgtx));	


	UIHandle r;
	r.p = pgtx;
	r.typ = TYP_CONTROLS;
	return r;



}

DLLEXPORT UIHandle D2DCreateEmptyControls(UIHandleWin hwin, UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id )
{
	_ASSERT(hwin.p);
	_ASSERT(hctrls.p);

	auto pgtx = new D2DControls_with_Scrollbar(); 

	auto win = (D2DWindow*)hwin.p;
	auto ctrls = (D2DControls*)hctrls.p;

	pgtx->CreateControl(win,ctrls, rc, stat, name, id );
	ctrls->Add( std::shared_ptr<D2DControls_with_Scrollbar>(pgtx));	

	UIHandle r;
	r.p = pgtx;
	r.typ = TYP_CONTROLS;
	return r;
}


DLLEXPORT UIHandle D2DCreateSquarePaper(UIHandleWin hwin, UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id )
{
	_ASSERT(hwin.p);
	_ASSERT(hctrls.p);

	auto pgtx = new D2DSquarePaper(); 

	auto win = (D2DWindow*)hwin.p;
	auto ctrls = (D2DControls*)hctrls.p;

	pgtx->CreateControl(win,ctrls, rc, stat, name, id );
	ctrls->Add( std::shared_ptr<D2DSquarePaper>(pgtx));	

	UIHandle r;
	r.p = pgtx;
	r.typ = TYP_CONTROLS;
	return r;



}
DLLEXPORT UIHandle D2DCreateStatic(UIHandleWin hwin, UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR text, LPCWSTR name, int id )
{
	_ASSERT(hwin.p);
	_ASSERT(hctrls.p);

	auto pgtx = new D2DStatic(); 

	auto win = (D2DWindow*)hwin.p;
	auto ctrls = (D2DControls*)hctrls.p;

	pgtx->CreateControl(win,ctrls, rc, stat, name, id );
	ctrls->Add( std::shared_ptr<D2DStatic>(pgtx));	
	pgtx->SetText(text);

	UIHandle r;
	r.p = pgtx;
	r.typ = TYP_STATIC;
	return r;
}

DLLEXPORT UIHandle D2DCreateTextbox(UIHandleWin hwin, UIHandle hctrls, const FRectF& rc, bool multiline, DWORD stat, LPCWSTR name, int id, int ext )
{
	_ASSERT(hwin.p);
	_ASSERT(hctrls.p);
	
	auto pgtx = new D2DTextbox(); 
	auto typ = ( multiline ?  IBridgeTSFInterface::MULTILINE :  IBridgeTSFInterface::SINGLELINE );

	auto win = (D2DWindow*)hwin.p;
	auto ctrls = (D2DControls*)hctrls.p;

	pgtx->CreateControl(win,ctrls, typ, rc, stat, name, id );
	ctrls->Add( std::shared_ptr<D2DTextbox>(pgtx));	


	if ( ext == 1 )
	{
		pgtx->SetBackColor(D2RGBA(0,0,0,0));
		pgtx->SetBorderColor(D2RGBA(0,0,0,0));


	}


	UIHandle r;
	r.p = pgtx;
	r.typ = TYP_TEXTBOX;
	return r;
}

D2DControl* D2DCastControl(UIHandle h )
{
	UIHandle r = h;
	D2DControl* p2 = (D2DControl*)h.p;
	if ( h.typ == TYP_TEXTBOX )
	{
		D2DTextbox* p = (D2DTextbox*)h.p;
		p2 = dynamic_cast<D2DControl*>(p);	// ‘½dŒp³‚ß‚ñ‚Ç‚¤
		_ASSERT( h.typ == p2->GetTypeid());

		auto p3 = dynamic_cast<D2DTextbox*>(p2);	

		_ASSERT( h.typ == p3->GetTypeid());

		int a = 0;
	}
	else if ( h.typ == TYP_BUTTON )
	{
		p2 = static_cast<D2DControl*>(p2);		
	}
	else if ( h.typ == TYP_DROPDOWNLISTBOX)
	{
		p2 = static_cast<D2DControl*>(p2);		
	}

	return p2;
}

DLLEXPORT UIHandle D2DCreateDropdownListbox(UIHandleWin hwin, UIHandle hctrls , const FRectF& rc, DWORD stat, LPCWSTR name, int id)
{
	auto pgtx = new D2DDropdownListbox();

	auto win = (D2DWindow*)hwin.p;
	auto ctrls = (D2DControls*)hctrls.p;

	pgtx->CreateControl(win,ctrls, rc, stat, name, id );
	ctrls->Add( std::shared_ptr<D2DDropdownListbox>(pgtx));	

	UIHandle r;
	r.p = pgtx;
	r.typ = TYP_DROPDOWNLISTBOX;
	return r;
}

DLLEXPORT int D2DAddItem(UIHandle h, int idx, LPCWSTR str)
{
	if ( h.typ == TYP_DROPDOWNLISTBOX )
	{
		auto cb =  (D2DDropdownListbox*)h.p;

		cb->AddItem(idx, str );

	}

	return 0;
}

DLLEXPORT UIHandle D2DGetParent(UIHandle h)
{
	D2DControl* pc = (D2DControl*)h.p;
	return ConvertUIHandle(pc->GetParentControls());
}

DLLEXPORT UIHandle D2DMessageBox(UIHandleWin hwin, const D2D1_RECT_F& rc, LPCWSTR title, LPCWSTR message)
{	
	auto win = (D2DWindow*)hwin.p;
	win->MessageBox(FRectF(rc), message,title);

	UIHandle r;
	r.p = 0;
	r.typ = TYP_NULL;
	return r;
}
DLLEXPORT UIHandle D2DFloatingMenu(UIHandleWin hwin, const D2D1_RECT_F& rc, LPVOID sender, MenuItem* items, int items_cnt)
{	
	std::vector<MenuItem> ar;

	for(int i = 0; i < items_cnt; i++ )
		ar.push_back(items[i]);

	auto win = (D2DWindow*)hwin.p;
	win->FloatingMenu(sender, FRectF(rc), ar );

	UIHandle r;
	r.p = 0;
	r.typ = TYP_NULL;
	return r;
}
DLLEXPORT UIHandle D2DCreateControls(UIHandleWin hwin, UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id)
{
	auto cs1 = new D2DControls();
	auto win = (D2DWindow*)hwin.p;
	auto ctrls = (D2DControls*)hctrls.p;

	cs1->CreateControl(win, ctrls, rc,stat, name,id);
	ctrls->Add(std::shared_ptr<D2DControls>(cs1));


	UIHandle r;
	r.p = cs1;
	r.typ = TYP_CONTROLS;
	return r;
}
DLLEXPORT UIHandle D2DCreateWhiteControls(LPVOID captureobj, DelegateDrawFunc func1, DelegateProcFunc func2, UIHandleWin hwin, UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id)
{
	auto cs1 = new D2DWhiteControl(captureobj, func1, func2);
	auto win = (D2DWindow*)hwin.p;
	auto ctrls = (D2DControls*)hctrls.p;
	cs1->CreateControl(win, ctrls, rc, stat, name, id);
	ctrls->Add(std::shared_ptr<D2DWhiteControl>(cs1));

	AppBase ab = {};
	ab.hWnd = win->GetHwnd();
	
	


	UIHandle r;
	r.p = cs1;
	r.typ = TYP_CONTROLS;

	cs1->WndProc(ab, WM_D2D_CREATE,  (WPARAM)&hwin ,(LPARAM)&r);

	return r;
}
DLLEXPORT UIHandle D2DCreateClientControls(LPVOID captureobj, DelegateDrawFunc func1, DelegateProcFunc func2, UIHandleWin hwin, UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id)
{
	auto cs1 = new D2DClientControls(captureobj, func1, func2);
	auto win = (D2DWindow*)hwin.p;
	auto ctrls = (D2DControls*)hctrls.p;
	cs1->CreateControl(win, ctrls, rc, stat, name, id);
	ctrls->Add(std::shared_ptr<D2DClientControls>(cs1));

	AppBase ab = {};
	ab.hWnd = win->GetHwnd();

	UIHandle r;
	r.p = cs1;
	r.typ = TYP_CONTROLS;

	cs1->WndProc(ab, WM_D2D_CREATE,  (WPARAM)&hwin ,(LPARAM)&r);

	return r;
}

DLLEXPORT void D2DSetCapture(UIHandle h)
{
	D2DControl* p = (D2DControl*)h.p;
	
	_ASSERT(p);
	APP.SetCapture(p);

}
DLLEXPORT void D2DReleaseCapture()
{
	APP.ReleaseCapture();
}
DLLEXPORT bool D2DIsCapture(UIHandle h)
{
	auto p = APP.GetCapture();

	return ( p == dynamic_cast<D2DCaptureObject*>( (D2DControl*)h.p));

}
DLLEXPORT UIHandle D2DGetCapture()
{
	UIHandle r = {0};
	auto p = APP.GetCapture();

	if ( dynamic_cast<D2DTextbox*>(p))
	{
		auto tx = dynamic_cast<D2DTextbox*>(p);
		r.p = tx;
		r.typ = TYP_TEXTBOX;
	}
	else if  (dynamic_cast<D2DWhiteControl*>(p))
	{
		auto tx = dynamic_cast<D2DWhiteControl*>(p);
		r.p = tx;
		r.typ = TYP_WHITE_CONTROL;

	}
	else if  (dynamic_cast<D2DClientControls*>(p))
	{
		auto tx = dynamic_cast<D2DClientControls*>(p);
		r.p = tx;
		r.typ = TYP_CLIENT_CONTROL;

	}
	else if  (dynamic_cast<InnerMessageBox*>(p))
	{
		auto tx = dynamic_cast<InnerMessageBox*>(p);
		r.p = tx;
		r.typ = TYP_MESSAGEBOX;

	}
	else if  (dynamic_cast<InnerFloatingMenu*>(p))
	{
		auto tx = dynamic_cast<InnerFloatingMenu*>(p);
		r.p = tx;
		r.typ = TYP_MESSAGEBOX;

	}
	else if  (dynamic_cast<D2DDropdownListbox*>(p))
	{
		auto tx = dynamic_cast<D2DDropdownListbox*>(p);
		r.p = tx;
		r.typ = TYP_DROPDOWNLISTBOX;

	}
	else if  (dynamic_cast<D2DAccordionbar*>(p))
	{
		auto tx = dynamic_cast<D2DAccordionbar*>(p);
		r.p = tx;
		r.typ = TYP_ACCORDIONBAR;

	}


	else if ( p != nullptr )
	{
		_ASSERT( 1==0); // –¢‘Î‰ž
	}



	return r;
}
DLLEXPORT void D2DSetText( UIHandle h, LPCWSTR str )
{
	if ( h.typ == TYP_TEXTBOX )
	{
		auto tx = dynamic_cast<D2DTextbox*>( D2DCastControl(h));
		//auto tx =  (D2DTextbox*)h.p;
		tx->Clear();
		tx->SetText(str, wcslen(str) );
	}
	else if ( h.typ == TYP_BUTTON )
	{
		auto btn = (D2DButton*)h.p;
		btn->SetText(str);
	}
}

DLLEXPORT void D2DInsertText(UIHandle h, LPCWSTR str, int len, int start_pos)
{
	if (h.typ == TYP_TEXTBOX)
	{
		auto tx = dynamic_cast<D2DTextbox*>( D2DCastControl(h));

		if (start_pos < 0)
			start_pos = tx->CurrentPos();

		tx->SetText(str, len, start_pos);
	}
}
DLLEXPORT BSTR D2DGetText(UIHandle h, bool bAll)
{
	if (h.typ == TYP_TEXTBOX)
	{
		std::wstringstream sm;
		auto tx = dynamic_cast<D2DTextbox*>( D2DCastControl(h));

		if ( bAll )
			tx->GetText( &sm, false );
		else
			tx->GetSelectText(&sm);

		auto s = sm.str();
		int len = (int)s.length();
		if ( len > 0 )
		{
			BSTR ret = ::SysAllocString( sm.str().c_str());
			return ret;
		}
	}
	return nullptr;
}

DLLEXPORT int D2DSetStat(UIHandle h, int stat)
{
	int old = 0;
	if (h.typ != TYP_MAIN_WINDOW)
	{
		auto p = (D2DControl*)h.p;
		old = p->GetStat();
		p->SetStat(stat);
	}
	return old;
}

DLLEXPORT void D2DReadOnly(UIHandle h, bool readonly)
{
	if (h.typ == TYP_TEXTBOX)
	{
		auto tx = dynamic_cast<D2DTextbox*>( D2DCastControl(h));

		if ( tx )		
			tx->SetReadonly(readonly);
	}
}




DLLEXPORT UIHandle D2DGetRootControls(UIHandleWin hMainWnd )
{
	UIHandle r;
	r.p = ((D2DWindow*)hMainWnd.p)->top_control_.get();
	r.typ = TYP_CONTROLS;
	return r;
}


UIHandle ConvertUIHandle(D2DControl* p)
{
	UIHandle r;
	auto ctrl =  p;
	r.p = ctrl;
	r.typ = p->GetTypeid();

	if ( r.typ == TYP_TEXTBOX )
	{
		r.p = dynamic_cast<D2DTextbox*>(ctrl);

	}


	return r;
}



DLLEXPORT UIHandle D2DGetControlFromID(UIHandleWin hMainWnd, UINT id)
{
	UIHandle r;
	D2DControls* x = ((D2DWindow*)hMainWnd.p)->top_control_.get();

	auto ctrl =  x->GetControlFromID(id);
	r.p = ctrl;
	r.typ = x->GetTypeid();

	if ( r.typ == TYP_TEXTBOX )
	{
		r.p = dynamic_cast<D2DTextbox*>(ctrl);

	}


	return r;
}
DLLEXPORT UIHandle D2DGetControlFromName(UIHandleWin hMainWnd, LPCWSTR nm)
{
	UIHandle r;
	D2DControls* x = ((D2DWindow*)hMainWnd.p)->top_control_.get();

	auto ctrl = x->GetControl( nm );
	r.p = ctrl;
	r.typ = ctrl->GetTypeid();

	if ( r.typ == TYP_TEXTBOX )
	{
		r.p = dynamic_cast<D2DTextbox*>(ctrl);

	}

	return r;

}



DLLEXPORT UIHandle D2DCreateButton(UIHandleWin hwin, UIHandle hctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id )
{
	auto pgtx = new D2DButton();
	auto win = (D2DWindow*)hwin.p;
	auto ctrls = (D2DControls*)hctrls.p;

	pgtx->CreateControl(win,ctrls, rc, stat, name, id );
	ctrls->Add( std::shared_ptr<D2DButton>(pgtx));	


	UIHandle r;
	r.p = pgtx;
	r.typ = TYP_BUTTON;
	return r;
}

//DLLEXPORT UIHandle D2DCreateDropdownListbox(D2DWindow* win, D2DControls* ctrls, const FRectF& rc, DWORD stat, LPCWSTR name, int id)
//{
//	auto pgtx = new D2DDropdownListbox();
//	pgtx->CreateControl(win, ctrls, rc, stat, name, id);
//	ctrls->Add(std::shared_ptr<D2DDropdownListbox>(pgtx));
//
//
//	UIHandle r;
//	r.p = pgtx;
//	r.typ = TYP_DROPDOWNLISTBOX;
//	return r;
//
//}

DLLEXPORT XDropdownListBox* D2DConvert(UIHandle r)
{	
	D2DDropdownListbox* pp = (D2DDropdownListbox* )r.p;


	return dynamic_cast<XDropdownListBox*>(pp);
}



DLLEXPORT void D2DSwapChain(UIHandleWin main, HWND hWnd  )
{
	_ASSERT( main.typ == TYP_MAIN_WINDOW );
	auto& cxt = ((D2DWindow*)main.p)->cxt;
	
	cxt.SwapChain();
	cxt.DoRedraw( hWnd);
}
DLLEXPORT void D2DDraw(UIHandleWin main, void* hWnd  )
{	
	_ASSERT( main.typ == TYP_MAIN_WINDOW );

	auto& cxt = ((D2DWindow*)main.p)->cxt;
	auto root = ((D2DWindow*)main.p)->top_control_;
	root->Draw(cxt);

	if ( cxt.bRedraw_ )
		cxt.DoRedraw((HWND)hWnd);
}

DLLEXPORT void D2DSetColor(UIHandle h, ColorF back, ColorF fore, ColorF border )
{
	if ( h.typ == TYP_TEXTBOX )
	{
		auto tx = static_cast<D2DTextbox*>(h.p);

		tx->SetBackColor(back);
		tx->SetForeColor(fore);
		tx->SetBorderColor(border);

	}
}



DLLEXPORT D2D1_RECT_F D2DGetRect(UIHandle h )
{
	D2DControl* p2 = D2DCastControl(h);
	return p2->GetRect();
}
DLLEXPORT void D2DSetRect( UIHandle h, D2D1_RECT_F rc )
{
	D2DControl* p2 = D2DCastControl(h);
	p2->SetRect(rc);
	
}

DLLEXPORT HRESULT D2DDefWndProc(UIHandleWin main ,AppBase& app, UINT message, WPARAM wParam, LPARAM lParam)
{
	_ASSERT( main.typ == TYP_MAIN_WINDOW );

	auto win = (D2DWindow*)main.p;
	return win->WndProc(app, message, wParam,lParam); // STAT_ENABLE‚Í—LŒø
}

DLLEXPORT void D2DForceWndProc(UIHandleWin main, AppBase& app, UINT message, WPARAM wParam, LPARAM lParam)
{	
	_ASSERT(main.typ == TYP_MAIN_WINDOW);
	auto win = (D2DWindow*)main.p;
	win->ForceWndProc(app, message, wParam, lParam); // STAT_ENABLE‚Í–³Ž‹‚·‚é
}
DLLEXPORT  void D2DDestroyControl(UIHandle h)
{
	D2DControl* h2 = D2DCastControl(h);


	h2->DestroyControl();

}


DLLEXPORT void D2DMDISetTopControl(UIHandle h)
{
	D2DControl* h2 = D2DCastControl(h);

	D2DControls* x = h2->GetParentControls();

	x->SetFirstControl(h2);

}
DLLEXPORT int D2DSendMessage(UIHandle h, UINT msg, WPARAM wp, LPARAM lp)
{
	auto ctrl = D2DCastControl(h);

	AppBase b={};

	return (int)ctrl->WndProc(b,msg,wp,lp);
}

DLLEXPORT D2D1_RECT_F* RectAnimation(const D2D1_RECT_F& rcStart, const D2D1_RECT_F& rcEnd, D2D1_RECT_F* p, int p_size, int style)
{
	_ASSERT( 0 < p_size && p );
	float xstep_left = (rcEnd.left - rcStart.left) / p_size;
	float ystep_top = (rcEnd.top - rcStart.top) / p_size;
	float xstep_right = (rcEnd.right - rcStart.right) / p_size;
	float ystep_bottom = (rcEnd.bottom - rcStart.bottom) / p_size;

	p[0] = rcStart;

	if ( style == 0 )
	{
		for(int i = 1; i < p_size-1; i++ )
		{
			FRectF rc(rcStart);

			rc.left = xstep_left * i + rc.left;
			rc.right = xstep_right * i + rc.right;
			rc.top = ystep_top * i + rc.top;
			rc.bottom = ystep_bottom * i + rc.bottom;

			p[i] = rc;
		}
	}


	p[p_size-1] = rcEnd;
	return p;

}