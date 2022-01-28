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
#include "D2DImageControl.h"
#include "D2DXXXControls.h"
using namespace V6;
#define  APP (D2DApp::GetInstance())

UIHandle ConvertUIHandle(D2DControl* p);

DLLEXPORT void WINAPI D2DInitail(INT_PTR p )
{
	D2DApp::SetD2DAppForDLL((D2DApp*)p);


}

DLLEXPORT UIHandleWin WINAPI D2DCreateMainHWnd( HWND hWnd,  float fontheight, int typ )
{
	auto win = new D2DWindow(hWnd);

	V6::D2DContextEx& cxt = win->GetContext();
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
DLLEXPORT void WINAPI D2DDestroyWindow(UIHandleWin main  )
{
	auto win = (D2DWindow*)main.p;
	delete win;
}

DLLEXPORT ID2D1DeviceContext* WINAPI D2DGetDeviceContext(UIHandleWin main  )
{
	if ( main.typ == TYP_MAIN_WINDOW )
	{
		auto win = (D2DWindow*)main.p;
		return *(win->GetContext());
	}
	return nullptr;
}
DLLEXPORT UIHandle WINAPI D2DCreateWhiteWindow(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id )
{
	_ASSERT(hctrls.p);

	auto pgtx = new D2DWhiteWindow(); 

	auto ctrls = (D2DControls*)hctrls.p;
	auto win = ctrls->GetParent();

	pgtx->CreateControl(win,ctrls, rc, stat, name, id );
	ctrls->Add( std::shared_ptr<D2DWhiteWindow>(pgtx));	


	UIHandle r;
	r.p = pgtx;
	r.typ = TYP_CONTROLS;
	return r;
}

DLLEXPORT UIHandle WINAPI D2DCreateImage(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id )
{
	auto pgtx = new D2DImageControl();

	auto ctrls = (D2DControls*)hctrls.p;
	auto win = ctrls->GetParent();

	pgtx->CreateControl(win,ctrls, rc, stat, name, id );
	ctrls->Add( std::shared_ptr<D2DImageControl>(pgtx));	

	UIHandle r;
	r.p = pgtx;
	r.typ = TYP_IMAGE;
	return r;

}
DLLEXPORT UIHandle WINAPI D2DCreateControlsWithScrollbar(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id )
{
	_ASSERT(hctrls.p);

	auto pgtx = new D2DControls_with_Scrollbar(); 

	auto ctrls = (D2DControls*)hctrls.p;
	auto win = ctrls->GetParent();

	pgtx->CreateControl(win,ctrls, rc, stat, name, id );
	ctrls->Add( std::shared_ptr<D2DControls_with_Scrollbar>(pgtx));	

	UIHandle r;
	r.p = pgtx;
	r.typ = TYP_CONTROLS;
	return r;
}

DLLEXPORT UIHandle WINAPI D2DCreateEmptyControls(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id )
{
	_ASSERT(hctrls.p);

	auto pgtx = std::make_shared<D2DControls>(); 

	auto ctrls = (D2DControls*)hctrls.p;
	auto win = ctrls->GetParent();

	pgtx->CreateControl(win,ctrls, rc, stat, name, id );
	ctrls->Add( pgtx);	

	UIHandle r;
	r.p = pgtx.get();
	r.typ = TYP_CONTROLS;
	return r;
}

DLLEXPORT UIHandle WINAPI D2DCreateXXXControls(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id )
{
	_ASSERT(hctrls.p);

	auto pgtx = std::make_shared<D2DXXXControls>(); 

	auto ctrls = (D2DControls*)hctrls.p;
	auto win = ctrls->GetParent();

	pgtx->CreateControl(win,ctrls, rc, stat, name, id );
	ctrls->Add( pgtx);	

	UIHandle r;
	r.p = pgtx.get();
	r.typ = TYP_CONTROLS;
	return r;
}

DLLEXPORT UIHandle WINAPI D2DCreateListbox(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id )
{
	_ASSERT(hctrls.p);

	auto pgtx = new D2DSimpleListbox();

	auto ctrls = (D2DControls*)hctrls.p;
	auto win = ctrls->GetParent();

	pgtx->CreateControl(win,ctrls, rc, stat, name, id );
	ctrls->Add( std::shared_ptr<D2DSimpleListbox>(pgtx));	

	UIHandle r;
	r.p = pgtx;
	r.typ = TYP_CONTROLS;
	return r;
}



DLLEXPORT UIHandle WINAPI D2DCreateSquarePaper(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id )
{
	_ASSERT(hctrls.p);

	auto pgtx = new D2DSquarePaper(); 

	auto ctrls = (D2DControls*)hctrls.p;
	auto win = ctrls->GetParent();

	pgtx->CreateControl(win,ctrls, rc, stat, name, id );
	ctrls->Add( std::shared_ptr<D2DSquarePaper>(pgtx));	

	UIHandle r;
	r.p = pgtx;
	r.typ = TYP_CONTROLS;
	return r;
}
DLLEXPORT UIHandle WINAPI D2DCreateStatic(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR text, LPCWSTR name, int id )
{
	_ASSERT(hctrls.p);

	auto pgtx = new D2DStatic(); 

	auto ctrls = (D2DControls*)hctrls.p;
	auto win = ctrls->GetParent();

	pgtx->CreateControl(win,ctrls, rc, stat, name, id );
	ctrls->Add( std::shared_ptr<D2DStatic>(pgtx));	
	pgtx->SetText(text);

	UIHandle r;
	r.p = pgtx;
	r.typ = TYP_STATIC;
	return r;
}

DLLEXPORT UIHandle WINAPI D2DCreateTextbox(UIHandle hctrls, const D2D1_RECT_F& rc, bool multiline, DWORD stat, LPCWSTR name, int id, int ext )
{
	_ASSERT(hctrls.p);
	
	auto pgtx = new D2DTextbox(); 
	auto typ = ( multiline ?  IBridgeTSFInterface::MULTILINE :  IBridgeTSFInterface::SINGLELINE );

	auto ctrls = (D2DControls*)hctrls.p;
	auto win = ctrls->GetParent();

	pgtx->CreateControl(win,ctrls, typ, rc, stat, name, id );
	ctrls->Add( std::shared_ptr<D2DTextbox>(pgtx));	


	if ( ext == 1 )
		pgtx->SetTypPassword();
	else if ( ext == 2 )
		pgtx->SetReadonly(true);


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
		p2 = dynamic_cast<D2DControl*>(p);	// 多重継承めんどう
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



DLLEXPORT UIHandle WINAPI D2DCreateDropdownListbox(UIHandle hctrls , const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id)
{
	auto pgtx = new D2DDropdownListbox();

	auto ctrls = (D2DControls*)hctrls.p;
	auto win = ctrls->GetParent();

	pgtx->CreateControl(win,ctrls, rc, stat, name, id );
	ctrls->Add( std::shared_ptr<D2DDropdownListbox>(pgtx));	

	UIHandle r;
	r.p = pgtx;
	r.typ = TYP_DROPDOWNLISTBOX;
	return r;
}

DLLEXPORT int WINAPI D2DAddItem(UIHandle h, int idx, LPCWSTR str)
{
	if ( h.typ == TYP_DROPDOWNLISTBOX )
	{
		auto cb =  (D2DDropdownListbox*)h.p;

		cb->AddItem(idx, str );

	}
	else if ( h.typ == TYP_SIMPLE_LISTBOX )
	{
		auto ls = (D2DSimpleListbox*)h.p;

		ls->AddItem(idx, str);


	}

	return 0;
}

DLLEXPORT UIHandle WINAPI D2DGetParent(UIHandle h)
{
	D2DControl* pc = (D2DControl*)h.p;
	return ConvertUIHandle(pc->GetParentControls());
}

DLLEXPORT UIHandle WINAPI D2DMessageBox(UIHandleWin hwin, const D2D1_RECT_F& rc, LPCWSTR title, LPCWSTR message)
{	
	auto win = (D2DWindow*)hwin.p;
	win->MessageBox(rc, message,title);

	UIHandle r;
	r.p = 0;
	r.typ = TYP_NULL;
	return r;
}
DLLEXPORT UIHandle WINAPI D2DFloatingMenu(UIHandleWin hwin, const D2D1_RECT_F& rc, LPVOID sender, MenuItem* items, int items_cnt)
{	
	std::vector<MenuItem> ar;

	for(int i = 0; i < items_cnt; i++ )
		ar.push_back(items[i]);

	auto win = (D2DWindow*)hwin.p;
	win->FloatingMenu(sender, D2D1_RECT_F(rc), ar );

	UIHandle r;
	r.p = 0;
	r.typ = TYP_NULL;
	return r;
}
DLLEXPORT UIHandle WINAPI D2DCreateControls(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id)
{
	auto cs1 = new D2DControls();
	auto ctrls = (D2DControls*)hctrls.p;
	auto win = ctrls->GetParent();

	cs1->CreateControl(win, ctrls, rc,stat, name,id);
	ctrls->Add(std::shared_ptr<D2DControls>(cs1));


	UIHandle r;
	r.p = cs1;
	r.typ = TYP_CONTROLS;
	return r;
}
DLLEXPORT UIHandle WINAPI D2DCreateWhiteControls(LPVOID captureobj, DelegateDrawFunc func1, DelegateProcFunc func2, UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id)
{
	auto cs1 = new D2DWhiteControl(captureobj, func1, func2);
	auto ctrls = (D2DControls*)hctrls.p;
	auto win = ctrls->GetParent();
	cs1->CreateControl(win, ctrls, rc, stat, name, id);
	ctrls->Add(std::shared_ptr<D2DWhiteControl>(cs1));

	AppBase ab = {};
	ab.hWnd = win->GetHwnd();
	
	


	UIHandle r;
	r.p = cs1;
	r.typ = TYP_CONTROLS;

	cs1->WndProc(ab, WM_D2D_CREATE,  (WPARAM)win ,(LPARAM)&r);

	return r;
}
DLLEXPORT UIHandle WINAPI D2DCreateClientControls(LPVOID captureobj, DelegateDrawFunc func1, DelegateProcFunc func2, UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id)
{
	auto cs1 = new D2DClientControls(captureobj, func1, func2);
	
	auto ctrls = (D2DControls*)hctrls.p;
	auto win = ctrls->GetParent();

	cs1->CreateControl(win, ctrls, rc, stat, name, id);
	ctrls->Add(std::shared_ptr<D2DClientControls>(cs1));

	AppBase ab = {};
	ab.hWnd = win->GetHwnd();

	UIHandle r;
	r.p = cs1;
	r.typ = TYP_CONTROLS;

	cs1->WndProc(ab, WM_D2D_CREATE,  (WPARAM)win ,(LPARAM)&r);

	return r;
}

DLLEXPORT void WINAPI D2DSetFocus( UIHandle h )
{
	if ( h.typ == TYP_TEXTBOX )
	{
		auto tx = dynamic_cast<D2DTextbox*>( D2DCastControl(h));
		tx->StatActive(true);


		APP.ReleaseCapture();
		APP.SetCapture(tx);

	}

}

DLLEXPORT void WINAPI D2DSetCapture(UIHandle h)
{
	D2DControl* p = (D2DControl*)h.p;
	
	_ASSERT(p);
	APP.SetCapture(p);

}
DLLEXPORT void WINAPI D2DReleaseCapture()
{
	APP.ReleaseCapture();
}
DLLEXPORT bool WINAPI D2DIsCapture(UIHandle h)
{
	auto p = APP.GetCapture();

	return ( p == dynamic_cast<D2DCaptureObject*>( (D2DControl*)h.p));

}
DLLEXPORT bool WINAPI D2DSetProcfunction(UIHandle h, DelegateProcFunc func2)
{
	D2DControl* p = (D2DControl*)h.p;
	auto sq = dynamic_cast<D2DSquarePaper*>(p);
	
	if ( sq )
	{
		sq->import_function_ = func2;
		return true;
	}
	return false;
}
DLLEXPORT void WINAPI D2DEnable(UIHandle h, bool enable)
{
	D2DControl* p = (D2DControl*)h.p;
	p->Enable(enable);
}

DLLEXPORT UIHandle WINAPI D2DCast(void* target)
{
	D2DControl* p = (D2DControl*)target;

	UIHandle r = {0};

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
	else if  (dynamic_cast<D2DSimpleListbox*>(p))
	{
		auto tx = dynamic_cast<D2DSimpleListbox*>(p);
		r.p = tx;
		r.typ = TYP_SIMPLE_LISTBOX;

	}
	else if  (dynamic_cast<D2DWhiteWindow*>(p))
	{
		auto tx = dynamic_cast<D2DWhiteWindow*>(p);
		r.p = tx;
		r.typ = TYP_WHITE_WINDOW;

	}
	else if  (dynamic_cast<D2DControls*>(p))
	{
		auto tx = dynamic_cast<D2DControls*>(p);
		r.p = tx;
		r.typ = TYP_CONTROLS;

	}
	else if  (dynamic_cast<D2DScrollbar*>(p))
	{
		auto tx = dynamic_cast<D2DScrollbar*>(p);
		r.p = tx;
		r.typ = TYP_SCROLLBAR;

	}
	else if  (dynamic_cast<D2DButton*>(p))
	{
		auto tx = dynamic_cast<D2DButton*>(p);
		r.p = tx;
		r.typ = TYP_BUTTON;

	}
	else if ( p != nullptr )
	{
		_ASSERT( 1==0); // 未対応
	}

	return r;

}

DLLEXPORT UIHandle WINAPI D2DGetCapture()
{
	auto p = APP.GetCapture();

	return D2DCast(p);
}
DLLEXPORT void WINAPI D2DSetText( UIHandle h, LPCWSTR str )
{
	if ( h.typ == TYP_TEXTBOX )
	{
		auto tx = static_cast<D2DTextbox*>( D2DCastControl(h));
		tx->Clear();
		tx->SetText(str, wcslen(str) );
	}
	else if ( h.typ == TYP_BUTTON )
	{
		auto btn = (D2DButton*)h.p;
		btn->SetText(str);
	}
}
DLLEXPORT void WINAPI D2DSetFont(UIHandle h, LPCWSTR fontnm, float height)
{
	if ( h.typ == TYP_TEXTBOX )
	{
		auto tx = static_cast<D2DTextbox*>( D2DCastControl(h));
		tx->SetFont(fontnm, height );

	}
}

DLLEXPORT void WINAPI D2DInsertText(UIHandle h, LPCWSTR str, int len, int start_pos)
{
	if (h.typ == TYP_TEXTBOX)
	{
		auto tx = static_cast<D2DTextbox*>( D2DCastControl(h));

		if (start_pos < 0)
			start_pos = tx->CurrentPos();

		tx->SetText(str, len, start_pos);
	}
}
DLLEXPORT BSTR WINAPI D2DGetText(UIHandle h, bool bAll)
{
	if (h.typ == TYP_TEXTBOX)
	{
		std::wstringstream sm;
		auto tx = static_cast<D2DTextbox*>( D2DCastControl(h));

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

DLLEXPORT int WINAPI D2DSetStat(UIHandle h, int stat)
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

DLLEXPORT void WINAPI D2DReadOnly(UIHandle h, bool readonly)
{
	if (h.typ == TYP_TEXTBOX)
	{
		auto tx = dynamic_cast<D2DTextbox*>( D2DCastControl(h));

		if ( tx )		
			tx->SetReadonly(readonly);
	}
}




DLLEXPORT UIHandle WINAPI D2DGetRootControls(UIHandleWin hMainWnd )
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



DLLEXPORT UIHandle WINAPI D2DGetControlFromID(UIHandleWin hMainWnd, UINT id)
{
	UIHandle r;
	D2DControls* x = ((D2DWindow*)hMainWnd.p)->top_control_.get();

	auto ctrl =  x->GetControlFromID(id);
	r.p = ctrl;
	r.typ = x->GetTypeid();

	if ( r.typ == TYP_TEXTBOX )
	{
		r.p = static_cast<D2DTextbox*>(ctrl);

	}


	return r;
}
DLLEXPORT UIHandle WINAPI D2DGetControlFromName(UIHandleWin hMainWnd, LPCWSTR nm)
{
	UIHandle r;
	D2DControls* x = ((D2DWindow*)hMainWnd.p)->top_control_.get();

	auto ctrl = x->GetControl( nm );
	r.p = ctrl;
	r.typ = ctrl->GetTypeid();

	if ( r.typ == TYP_TEXTBOX )
	{
		r.p = static_cast<D2DTextbox*>(ctrl);

	}

	return r;

}



DLLEXPORT UIHandle WINAPI D2DCreateButton(UIHandle hctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id )
{
	auto pgtx = new D2DButton();
	
	auto ctrls = (D2DControls*)hctrls.p;
	auto win = ctrls->GetParent();

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

DLLEXPORT XDropdownListBox* WINAPI D2DConvert(UIHandle r)
{	
	D2DDropdownListbox* pp = (D2DDropdownListbox* )r.p;


	return dynamic_cast<XDropdownListBox*>(pp);
}



DLLEXPORT void WINAPI D2DSwapChain(UIHandleWin main, HWND hWnd  )
{
	_ASSERT( main.typ == TYP_MAIN_WINDOW );
	auto& cxt = ((D2DWindow*)main.p)->GetContext();
	
	cxt.SwapChain();
	cxt.DoRedraw( hWnd);
}
DLLEXPORT void WINAPI D2DDraw(UIHandleWin main, void* hWnd  )
{	
	_ASSERT( main.typ == TYP_MAIN_WINDOW );

	auto pwin = (D2DWindow*)main.p;

	auto& cxt = pwin->GetContext();
	auto root = pwin->top_control_;
	root->Draw(cxt);

	
	{
		static int no=0;

		if ( pwin->Smooth_ )
			no = pwin->Smooth_(pwin, no);
		else
			no = 0;
	}


	if ( cxt.bRedraw_ )
		cxt.DoRedraw((HWND)hWnd);
}

DLLEXPORT void WINAPI D2DSetColor(UIHandle h, ColorF back, ColorF fore, ColorF border )
{
	if ( h.typ == TYP_TEXTBOX )
	{
		auto tx = static_cast<D2DTextbox*>(h.p);

		tx->SetBackColor(back);
		tx->SetForeColor(fore);
		tx->SetBorderColor(border);
	}
	else
	{
		auto tx = static_cast<D2DControl*>(h.p);
		
		AppBase b={};

		tx->WndProc(b,WM_D2D_SET_COLOR, 0, (LPARAM)&back);
		tx->WndProc(b,WM_D2D_SET_COLOR, 1, (LPARAM)&fore);
		
	}
}



DLLEXPORT D2D1_RECT_F WINAPI D2DGetRect(UIHandle h )
{
	D2DControl* p2 = D2DCastControl(h);
	return p2->GetRect();
}
DLLEXPORT void WINAPI D2DSetRect( UIHandle h, D2D1_RECT_F rc )
{
	D2DControl* p2 = D2DCastControl(h);
	p2->SetRect(rc);
	
}

DLLEXPORT LRESULT WINAPI D2DDefWndProc(UIHandleWin main ,AppBase& app, UINT message, WPARAM wParam, LPARAM lParam)
{
	_ASSERT( main.typ == TYP_MAIN_WINDOW );

	auto win = (D2DWindow*)main.p;
	return win->WndProc(app, message, wParam,lParam); // STAT_ENABLEは有効
}

DLLEXPORT void WINAPI D2DForceWndProc(UIHandleWin main, AppBase& app, UINT message, WPARAM wParam, LPARAM lParam)
{	
	_ASSERT(main.typ == TYP_MAIN_WINDOW);
	auto win = (D2DWindow*)main.p;
	win->ForceWndProc(app, message, wParam, lParam); // STAT_ENABLEは無視する
}
DLLEXPORT  void WINAPI D2DDestroyControl(UIHandle h)
{
	D2DControl* h2 = D2DCastControl(h);


	h2->DestroyControl();

}
DLLEXPORT void WINAPI D2DEventHandler( UIHandle h, D2DEventHandlerDelegate handler)
{
	auto p = dynamic_cast<D2DButton*>(D2DCastControl( h ));	
	if ( p )
	{
		p->click_ = handler;
		return;
	}
	auto p1 = dynamic_cast<D2DDropdownListbox*>(D2DCastControl( h ));	
	if ( p1 )
	{
		p1->click_ = handler;
		return;
	}



}


DLLEXPORT void WINAPI D2DMDISetTopControl(UIHandle h)
{
	D2DControl* h2 = D2DCastControl(h);

	D2DControls* x = h2->GetParentControls();

	x->SetFirstControl(h2);

}
DLLEXPORT int WINAPI D2DSendMessage(UIHandle h, UINT msg, WPARAM wp, LPARAM lp)
{
	auto ctrl = D2DCastControl(h);

	AppBase b={};

	return (int)ctrl->WndProc(b,msg,wp,lp);
}

DLLEXPORT D2D1_RECT_F* WINAPI RectAnimation(const D2D1_RECT_F& rcStart, const D2D1_RECT_F& rcEnd, D2D1_RECT_F* p, int p_size, int style)
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


DLLEXPORT void WINAPI SmoothRect(int typ, UIHandleWin win, D2D1_RECT_F* target, D2D1_RECT_F dstRect)
{
	if ( typ == 0 )
		*target = dstRect;
	else if ( typ == 1 )
	{
		int cnt = 25;
		FRectF* prc = new FRectF[cnt];
		FRectF srect = target[0];
		int atyp = typ - 1;
	
		RectAnimation(srect, dstRect, prc, cnt, atyp);

		D2DWindow* pwin = (D2DWindow*)win.p;
		pwin->Smooth_ = [prc, cnt,target](D2DWindow* win, int no)->int
		{						
			if ( no < cnt )
			{
				*target = prc[no];
				win->GetContext().bRedraw_ = true;
			}
			else if ( no == cnt )
			{
				*target = prc[no-1];
				win->GetContext().bRedraw_ = true;
				delete [] prc;
				win->Smooth_ = nullptr;
			}

			return (no+1);
		};
	}
}


DLLEXPORT bool WINAPI D2DStream2Bitmap( IStream* bmpstream, ID2D1RenderTarget* target, ID2D1Bitmap** bmp)
{
	ComPTR<IWICImagingFactory> d2dWICFactory;
	ComPTR<IWICBitmapDecoder> d2dDecoder;
	ComPTR<IWICFormatConverter> d2dConverter;
	ComPTR<IWICBitmapFrameDecode> d2dBmpSrc;

	auto hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, 
		__uuidof(IWICImagingFactory), (void**)(&d2dWICFactory));
	
	if (hr != S_OK) return false;
	hr = d2dWICFactory->CreateDecoderFromStream(bmpstream, 0, WICDecodeMetadataCacheOnLoad, &d2dDecoder);
	if (hr != S_OK) return false;
	hr = d2dWICFactory->CreateFormatConverter(&d2dConverter);
	if (hr != S_OK) return false;
	hr = d2dDecoder->GetFrame(0, &d2dBmpSrc);
	if (hr != S_OK) return false;
	hr = d2dConverter->Initialize(d2dBmpSrc, GUID_WICPixelFormat32bppPBGRA,	WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
	if (hr != S_OK) return false;
	hr = target->CreateBitmapFromWicBitmap(d2dConverter, NULL, bmp);

	return (hr == S_OK);
}