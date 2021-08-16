#include "pch.h"
#include "D2D1UI_1.h"
#include "D2DTextbox.h"
#include "D2DButton.h"
#include "D2DContext.h"
#include "D2DDropdownListbox.h"
#include "D2DWhiteControl.h"
using namespace V6;
#define  APP (D2DApp::GetInstance())

DLLEXPORT UIHandleWin D2DCreateMainHWnd( HWND hWnd,  float fontheight )
{
	auto win = new D2DWindow(hWnd);

	V6::D2DContextEx& cxt = win->cxt;
	FSizeF size;
	win->CreateResource( &size );

	//RECT rc;
	//::GetClientRect(hWnd, &rc);
	//cxt.CreateDeviceResources(hWnd, rc.right, rc.bottom);
	cxt.CreateDeviceIndependentResources(fontheight);


	auto topcontrol = std::shared_ptr<D2DControls>( new D2DControls());
	topcontrol->CreateControl( win, nullptr, FRectF(0.0f,0.0f, size), STAT_VISIBLE, L"_root" );

	
	win->top_control_ = topcontrol;



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

DLLEXPORT UIHandle D2DCreateTextbox(UIHandleWin hwin, UIHandle hctrls, const FRectF& rc, bool multiline, DWORD stat, LPCWSTR name )
{
	auto pgtx = new D2DTextbox(); 
	auto typ = ( multiline ?  IBridgeTSFInterface::MULTILINE :  IBridgeTSFInterface::SINGLELINE );

	auto win = (D2DWindow*)hwin.p;
	auto ctrls = (D2DControls*)hctrls.p;

	pgtx->CreateControl(win,ctrls, typ, rc, stat, name );
	ctrls->Add( std::shared_ptr<D2DTextbox>(pgtx));	


	UIHandle r;
	r.p = pgtx;
	r.typ = TYP_TEXTBOX;
	return r;
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


	UIHandle r;
	r.p = cs1;
	r.typ = TYP_CONTROLS;
	return r;
}

DLLEXPORT UIHandle D2DGetCapture()
{
	UIHandle r = {0};
	auto p = APP.GetCapture();
	auto tx = dynamic_cast<D2DTextbox*>(p);

	if (tx)
	{
		r.p = tx;
		r.typ = TYP_TEXTBOX;
	}
	return r;
}
DLLEXPORT void D2DSetText( UIHandle h, LPCWSTR str )
{
	if ( h.typ == TYP_TEXTBOX )
	{
		auto tx =  (D2DTextbox*)h.p;
		tx->SetText(str, wcslen(str) );
	}
}

DLLEXPORT void D2DInsertText(UIHandle h, LPCWSTR str, int len, int start_pos)
{
	if (h.typ == TYP_TEXTBOX)
	{
		auto tx = (D2DTextbox*)h.p;

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
		auto tx = (D2DTextbox*)h.p;

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
DLLEXPORT UIHandle D2DGetRootControls(UIHandleWin hMainWnd )
{
	UIHandle r;
	r.p = ((D2DWindow*)hMainWnd.p)->top_control_.get();
	r.typ = TYP_CONTROLS;
	return r;
}

DLLEXPORT UIHandle D2DGetControlFromID(UIHandleWin hMainWnd, UINT id)
{
	UIHandle r;
	D2DControls* x = ((D2DWindow*)hMainWnd.p)->top_control_.get();

	r.p =  x->GetControlFromID(id);
	r.typ = TYP_CONTROLS;

	return r;
}
DLLEXPORT UIHandle D2DGetControlFromName(UIHandleWin hMainWnd, LPCWSTR nm)
{
	UIHandle r;
	D2DControls* x = ((D2DWindow*)hMainWnd.p)->top_control_.get();

	r.p = x->GetControl( nm );
	r.typ = TYP_CONTROLS;

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


DLLEXPORT D2D1_RECT_F* RectAnimation(const D2D1_RECT_F& rcStart, const D2D1_RECT_F& rcEnd, D2D1_RECT_F* p, int p_size, int style)
{
	_ASSERT( 0 < p_size && p );
	float xstep = (rcEnd.left - rcStart.left) / p_size;
	float ystep = (rcEnd.top - rcStart.top) / p_size;

	p[0] = rcStart;

	if ( style == 0 )
	{
		for(int i = 1; i < p_size-1; i++ )
		{
			FRectF rc(rcStart);

			rc.left = xstep * i + rc.left;
			rc.right = xstep * i + rc.right;
			rc.top = ystep * i + rc.top;
			rc.bottom = ystep * i + rc.bottom;

			p[i] = rc;
		}
	}

	p[p_size-1] = rcEnd;
	return p;

}