
#include "pch.h"
#include "inet.h"
#include "D2DMisc.h"
#include <fstream>
#include "D2D1UI_1.h"
#include "D2DSquarePaper.h"
#include "D2DAccordionbar.h"
#include "D2DFileManage.h"
#include "xml.h"
#include "D2DWhiteWindowControl.h"
#include "D2DColor.h"
#include "chart_top.h"
#include "javasc.h"



using namespace V6;

bool LoadTextFile(LPCWSTR fnm, bool bUtf8, std::wstringstream* out);

D2DControls* CreateObjectList(D2DControls* ctrl,  FSizeF size, LPCWSTR k );

extern UIHandleWin hwin;


#define  APP (D2DApp::GetInstance())
void CreateDocumentControl(UIHandle h);
void CreateLoginControl(UIHandle h);

void CenterDraw(LPVOID captureobj, D2DContext& cxt);
LRESULT CenterFunc(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
D2DControls* CreateSquare(UIHandle parent,  FSizeF size, LPCWSTR k );

extern std::map<int, JsValueRef> objBank;

void CreateControl(HWND hWnd)
{

	hwin = D2DCreateMainHWnd(hWnd, 14,0);    
    auto root = D2DGetRootControls(hwin);

	auto hctrlsA = D2DCreateEmptyControls( root, FRectF(), STAT_DEFAULT, NONAME,-1);

	CreateLoginControl(hctrlsA);



	CreateDocumentControl(hctrlsA);

}
void CreateLoginControl(UIHandle h)
{
	auto hlogin = D2DCreateLogin( h,FRectF(400,100,FSizeF(350,350)), STAT_DEFAULT|STAT_MODAL, L"login" );
	
	D2DColor clr(0x9e9e8a),clr2(D2RGBA(0,0,0,0));
	
	D2DSendMessage(hlogin, WM_D2D_SET_COLOR,0,(LPARAM)&clr);
	D2DSendMessage(hlogin, WM_D2D_SET_COLOR,2,(LPARAM)&clr2);

	auto on_try_login = [](void* sender, void* p)->DWORD
	{
		BSTR* p1 = (BSTR*)p;

		std::wstring cd = p1[0];
		std::wstring pwd = p1[1];

		if ( cd == pwd )
		{
			
			((D2DControls*)sender)->GetParentControls()->SetStat(STAT_DEFAULT);


			UIHandle h = {};
			h.p = ((D2DControls*)sender)->GetParentControls();

			D2DSetColor(h, ColorF::Green, ColorF::White,ColorF::Black);		

			h = D2DGetControlFromName(hwin, L"@document");

			if ( h.p )
				D2DSetStat(h, STAT_DEFAULT);


			return 0;

		}
		p1[2] = ::SysAllocString( L"login fail.");


		return 1;
	};

	typedef DWORD (*login)(void*,void*);

	login func = on_try_login;
	

	D2DSendMessage(hlogin, WM_D2D_SET_EVENTHANDLER, (WPARAM)L"login",(LPARAM)func);

	APP.SetCapture( (D2DControl*)hlogin.p);
}


struct BobInstance
{
	BobInstance()
	{
		clr[0] = ColorF::AliceBlue;
		clr[1] = ColorF::AliceBlue;
		clr[2] = ColorF::AliceBlue;
		
		hme={};
		hprv={};
		hactive={};
		prc = nullptr;
		child = nullptr;
	};

	FRectF* prc;
	FRectF orgrc;
	D2DColor clr[3];
	D2DMat mat;
	UIHandle hme;
	UIHandle hprv;

	UIHandle hactive;
	D2DControls* child;
	FSizeF voff;

};


void CreateCardControls(UIHandle h, int id);

bool df1(LPVOID captureobj, D2DContext& cxt)
{
	BobInstance* m = (BobInstance*)captureobj;

	
	D2DMatrix mat(*cxt);

	m->mat = mat.PushTransform();


	FRectF& rc = *(m->prc);

	D2DRectFilter fil(cxt, rc);
	
	mat.Offset(rc);
	
	cxt.DFillRect(rc.ZeroRect(), m->clr[0]);


	FRectF rc1 = rc.ZeroRect();
	rc1.InflateRect(-3,-3);
	cxt.DDrawRect(rc1, m->clr[1], m->clr[0]);

	D2DControls* c = (D2DControls*)m->hme.p;



	mat.PushTransform();
	mat.Offset(m->voff.width, m->voff.height);
	c->InnerDraw(cxt);
	mat.PopTransform();


	mat.PopTransform();

	return false;


}

void xD2DJsCallFunction( int id, LPCWSTR funcnm, JsValueRef* arg,  int argcnt)
{
	JsValueRef b1 = objBank[id];
	JsValueType ty1,ty;
	auto ty01= JsGetValueType(b1, &ty);
	_ASSERT(ty == JsObject);

	JsPropertyIdRef funcid;
	auto er = JsGetPropertyIdFromName(funcnm, &funcid); 
					
	JsValueRef func;
					
	er = JsGetProperty(b1, funcid, &func);					
	er = JsGetValueType(func, &ty1);
	if (ty1 == JsFunction)
	{
		_ASSERT(argcnt+2 < 10);
		JsValueRef _arg[10];

		_arg[0] = b1;
		_arg[1] = b1;

		for(int i=0; i < argcnt;i++)
			_arg[2+i] = arg[i];

		JsValueRef result1;
		er = JsCallFunction(func,_arg, argcnt+2, &result1);
	}				

}


LPCWSTR NameGen( BSTR nm, LPCWSTR xx)
{
	static WCHAR cb[512];
	StringCbPrintf(cb,512,L"%s@%s", nm, xx );
	return cb;
}

#define BTN_RUN_ID	2022
#define BTN_CLEAR_ID	2023

LRESULT df2(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	BobInstance* m = (BobInstance*)captureobj;
	auto me = (D2DControl*)b.card;


	LRESULT r = 0, r2 = 0;

	switch(message)
	{
		case WM_D2D_CREATE:
		{
			m->hme = *(UIHandle*)lParam;			
			
			m->orgrc = D2DGetRect(m->hme);
			m->prc = (FRectF*)D2DGetRect2(m->hme);

			
			
			auto id = D2DGetId(m->hme);

			if ( 11 == id )
			{
				CreateCardControls(m->hme, 10);
			}
			else if ( 10 == id )
			{
				auto tx = D2DCreateTextbox(m->hme, FRectF(10,40,FSizeF(600,550)), true, STAT_DEFAULT, L"TX1",-1,-1);
				JsRegistUIHandle(L"TX1", tx );
				D2DSetFont(tx, L"?l?r ????", 14);
				
				std::wstringstream sm;

				if ( LoadTextFile(L"init.js",false, &sm ) )
					D2DSetText( tx, sm.str().c_str()); 

				D2DSetColor(tx, ColorF::Gray, ColorF::White, ColorF::Gray);

				auto b1 = D2DCreateButton(m->hme, FRectF(10,10,FSizeF(100,25)), STAT_DEFAULT, L"JSRunButton", BTN_RUN_ID);
				D2DSetText(b1, L"Run");
				auto b2 = D2DCreateButton(m->hme, FRectF(120,10,FSizeF(100,25)), STAT_DEFAULT, L"JSClearButton", BTN_CLEAR_ID);
				D2DSetText(b2, L"Clear");
			}
			else if ( 13 == id )
			{
				LPCWSTR prm = L"text= name:s4&color=[128,217,204]&align=center&fontheight=12";

				D2DCreateStatic(m->hme, FRectF(10,10,FSizeF(200,260)), STAT_DEFAULT, prm, NONAME, -2 );


			}
			else if ( 12 == id )
			{
				//m->voff = FSizeF(5,5);	

				auto tab = D2DCreateTabControls(m->hme, FRectF(0,0,FSizeF(700,800)),  STAT_DEFAULT, L"TABC" );

				D2DSetColor(tab,ColorF::LightGray,ColorF::White,ColorF::White);

				auto tab1 = D2DGetTab(tab, 0); 
				D2DSendMessage(tab,WM_D2D_SET_TEXT,0,(LPARAM)L"JPN");
				
				auto tx = D2DCreateTextbox(tab1, FRectF(10,40,FSizeF(600,350)), true, STAT_DEFAULT, L"TX2",-1,-1);

				JsRegistUIHandle(L"TX2", tx );

				auto tab2 = D2DAddNewTab(tab, L"ROC");

				auto tab2btn = D2DCreateButton(tab2, FRectF(10,10,FSizeF(100,20)), STAT_DEFAULT, L"ChildWindow", 10);

				D2DEventHandlerDelegate func =[](void* sender,LPCWSTR eventName, void* param)->DWORD {					
					UIHandle h = {};
					h.p = sender;
					
					auto hc = D2DGetParent(h);
					auto hcw = D2DCreateChildWindow(hc, FRectF(10,50,FSizeF(600,800)),	STAT_DEFAULT, L"child", 11);


					CreateCardControls(hcw, 10);

					return 0;
				};

				D2DEventHandler(tab2btn, func);

			}


			r = 1;
		}
		break;
		case WM_NOTIFY:
		{
			if ( wParam == BTN_RUN_ID )
			{				
				//auto tx = D2DGetControlFromName(hwin,L"TX1");
				auto tx = JsRegistGet(L"TX1");
				auto bs = D2DGetText( tx, true );

				JsRun(bs);

				r = 1;
			}		
			else if ( wParam == BTN_CLEAR_ID )
			{
				//auto tx = D2DGetControlFromName(hwin,L"TX1");
				auto tx = JsRegistGet(L"TX1");
				D2DClear( tx );

				r = 1;
			}
			else if ( wParam >= 10000 )
			{
				D2DNMHDR& nmh = *(D2DNMHDR*)lParam;

				int id = D2DGetId(nmh.sender);

				if ( nmh.code == EVENTID_SELECTCHANGED)
				{					
					auto *text = L"test  ";
					int select_idx = (int)nmh.prm1;

					WCHAR cb[64];
					StringCbPrintf (cb,64,L"%s %d", text, select_idx);

					JsValueRef val;
					JsPointerToString(cb, lstrlen(cb), &val);

					JsValueRef arg[] = {val};

					xD2DJsCallFunction( id, L"OnChanged", arg, 1);
				}
				else if ( nmh.code == EVENTID_CLICK)
				{
					xD2DJsCallFunction( id, L"OnClick", nullptr, 0); // equal to "b1->OnClick(b1);"
				}

				r = 1;
			}
			
		}
		break;


		case WM_D2D_DESTROY:
		{
			delete m;
			r = 1;
		}
		break;
		case WM_D2D_SET_COLOR:
		{
			ColorF clr = *(ColorF*)lParam;

			if ( wParam == 0 )
				m->clr[0] = clr;
			else if ( wParam == 1 )
				m->clr[1] = clr;
			else if ( wParam == 2 )
				m->clr[2] = clr;

			r = 1;
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
			r = D2DDefControlProc(m->hme,b,message,wParam,lParam);
			
			if ( r == 0)
			{
			
				MouseParam& pm = *(MouseParam*)lParam;

				auto pt = m->mat.DPtoLP(pm.pt);

				FRectF rc = *(m->prc);

				if ( rc.PtInRect(pt) )
				{
					r = 1;

					if ( m->hactive.p != m->hme.p )
					{
						m->hactive = m->hme;

						FRectF rcDst(0,0,rc.Width()*2,  rc.Height()*2);

						m->hprv = D2DSetTopControl(m->hme);

						D2DSmoothRect(1,99,  m->hme, m->prc, rcDst);

					

					}
					else
					{
						auto w = D2DGetWindow(m->hme);
						BSTR nm = D2DGetName( D2DGetParent(m->hme));
						D2DSetStat(D2DGetControlFromName( w, NameGen(nm,L"s1")), STAT_DEFAULT);
						D2DSetStat(D2DGetControlFromName( w, NameGen(nm,L"s2")), STAT_DEFAULT);
						D2DSetStat(D2DGetControlFromName( w, NameGen(nm,L"s3")), STAT_DEFAULT);
						D2DSetStat(D2DGetControlFromName( w, NameGen(nm,L"s4")), STAT_DEFAULT);


						FRectF rcDst = m->orgrc;

						D2DSmoothRect(1,98,  m->hme, m->prc, rcDst);
						m->hactive.p = nullptr;


					
					}

				}
			}
			return r;
		}
		break;
		case WM_D2D_SMOOTH_COMPLETE:
		{
			if ( lParam == 99 )
			{
				if ( m->hactive.p == m->hme.p )
				{
					auto w = D2DGetWindow(m->hme);
					BSTR nm = D2DGetName( D2DGetParent(m->hme));

					D2DSetStat(D2DGetControlFromName( w, NameGen(nm,L"s1")), 0);
					D2DSetStat(D2DGetControlFromName( w, NameGen(nm,L"s2")), 0);
					D2DSetStat(D2DGetControlFromName( w, NameGen(nm,L"s3")), 0);
					D2DSetStat(D2DGetControlFromName( w, NameGen(nm,L"s4")), 0);


					D2DSetStat(m->hme, STAT_DEFAULT);
					r = 1;
				}

			}
			else if ( lParam == 98 )
			{
				D2DSetTopControl(m->hprv);
				m->hprv.p = nullptr;
				r = 1;
			}
		}
		break;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = m->mat.DPtoLP(pm.pt);
			FRectF rc = *(m->prc);
			if ( rc.PtInRect(pt) )
			{
				r2=1;
			}
			
		}
		break;
	}

	if ( r == 0 )
		r = D2DDefControlProc(m->hme,b,message,wParam,lParam);




	return r+r2;
}
void CreateDocumentControl(UIHandle h)
{
	auto cw = GetSystemMetrics(SM_CXSCREEN);
	auto ch = GetSystemMetrics(SM_CYSCREEN);	

	FSizeF sz(cw/2.0f,ch/2.0f);


	auto hdoc1 = D2DCreateXXXControls( h, FRectF(), 0, L"document",-1); // 0:login?????????\?????????B
	
	UIHandle ha[4]={};
	ha[0] = D2DCreateWhiteControls( (LPVOID)new BobInstance(), df1,df2,hdoc1, FRectF(0,0, sz), STAT_DEFAULT, L"s1",10);
	ha[1] = D2DCreateWhiteControls( (LPVOID)new BobInstance(), df1,df2,hdoc1, FRectF(sz.width,sz.height,sz), STAT_DEFAULT, L"s2",11);
	ha[2] = D2DCreateWhiteControls( (LPVOID)new BobInstance(), df1,df2,hdoc1, FRectF(0,sz.height,sz), STAT_DEFAULT, L"s3",12);
	ha[3] = D2DCreateWhiteControls( (LPVOID)new BobInstance(), df1,df2,hdoc1, FRectF(sz.width,0,sz), STAT_DEFAULT, L"s4",13);


	JsRegistUIHandle(L"s1", ha[0] );
	JsRegistUIHandle(L"s2", ha[1] );
	JsRegistUIHandle(L"s3", ha[2] );
	JsRegistUIHandle(L"s4", ha[3] );

	// https://coolors.co/9e9e8a

	D2DColor c1(0x998a9e);
	D2DColor c2(0x8a8d9e);
	D2DColor c3(0x8a9e94);
	D2DColor c4(0x9e9e8a);

	/*D2DColor c1(0xDDDDD0);
	D2DColor c2(0xD1DED5);
	D2DColor c3(0xD1D3DE);
	D2DColor c4(0xDED1DE);*/

	/*D2DColor c1(0xDACF67);
	D2DColor c2(0x66D975);
	D2DColor c3(0x6694D9);
	D2DColor c4(0xD96696);*/

	D2DSetColor(ha[0], c1, ColorF::White,ColorF::Black);		
	D2DSetColor(ha[1], c2, D2RGB(0x8D,0xD9,0xCC),ColorF::White);		
	D2DSetColor(ha[2], c3, ColorF::White,ColorF::Black);		
	D2DSetColor(ha[3], c4, ColorF::White,ColorF::Black);		

	auto nm = D2DGetName(hdoc1);

}


LRESULT kf2(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	BobInstance* m = (BobInstance*)captureobj;
	auto me = (D2DControl*)b.card;


	LRESULT r = 0, r2 = 0;

	switch(message)
	{
		case WM_D2D_CREATE:
		{
			m->hme = *(UIHandle*)lParam;			
			m->orgrc = D2DGetRect(m->hme);
			m->prc = (FRectF*)D2DGetRect2(m->hme);
			
			auto id = D2DGetId(m->hme);

			if ( 12 == id )
			{
				m->child = CreateStockChart( (D2DControls*)m->hme.p,  FSizeF(1600,650), L"yahoo" );

			}
			else if ( 11 == id )
			{
				m->child = CreateSquare( m->hme,  FSizeF(5000,5000), L"C" );

			}
			else if ( 10 == id )
			{
				m->child = CreateObjectList((D2DControls*)m->hme.p,  FSizeF(1000,600), L"list" );


			}

			r = 1;
		}
		break;
		case WM_D2D_DESTROY:
		{
			delete m;
			r = 1;
		}
		break;
		case WM_D2D_SET_COLOR:
		{
			ColorF clr = *(ColorF*)lParam;

			if ( wParam == 0 )
				m->clr[0] = clr;
			else if ( wParam == 1 )
				m->clr[1] = clr;
			else if ( wParam == 2 )
				m->clr[2] = clr;

			r = 1;
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
			r = D2DDefControlProc(m->hme,b,message,wParam,lParam);
			
			if ( r == 0)
			{
			
				MouseParam& pm = *(MouseParam*)lParam;

				auto pt = m->mat.DPtoLP(pm.pt);

				FRectF rc = *(m->prc);

				if ( rc.PtInRect(pt) )
				{
					r = 1;

					if ( m->hactive.p != m->hme.p )
					{
						m->hactive = m->hme;

						auto hpa = D2DGetParent(m->hme);
						rc = D2DGetRect(hpa);

						auto w = D2DGetWindow(m->hme);
						FRectF rcDst(0,0,rc.Width(), rc.Height());

						BSTR nm = D2DGetName( D2DGetParent(m->hme));
						D2DSetStat(D2DGetControlFromName( w, NameGen(nm,L"k1")), 0);
						D2DSetStat(D2DGetControlFromName( w, NameGen(nm,L"k2")), 0);
						D2DSetStat(D2DGetControlFromName( w, NameGen(nm,L"k3")), 0);

						D2DSetStat(m->hme,STAT_DEFAULT);


						m->hprv = D2DSetLastControl(m->hme );

						D2DSmoothRect(1,199,  m->hme, m->prc, rcDst);

						//
						if ( m->child )
							m->child->SetStat(STAT_DEFAULT);

					}
					else
					{
						auto w = D2DGetWindow(m->hme);
						//BSTR nm = D2DGetName( D2DGetParent(m->hme));

						FRectF rcDst = m->orgrc;

						D2DSmoothRect(1,198,  m->hme, m->prc, rcDst);

						

						m->hactive.p = nullptr;


					
					}

				}
			}
			return r;
		}
		break;
		case WM_D2D_SMOOTH_COMPLETE:
		{
			if ( lParam == 199 )
			{
				if ( (INT_PTR)m->hme.p == (INT_PTR)wParam)
				{
					//auto w = D2DGetWindow(m->hme);
					//BSTR nm = D2DGetName( D2DGetParent(m->hme));

					D2DSetStat(m->hme, STAT_DEFAULT);
					r = 1;
				}

			}
			else if ( lParam == 198 )
			{
				if ( (INT_PTR)m->hme.p == (INT_PTR)wParam)
				{
					D2DSetLastControl(m->hprv);

					auto w = D2DGetWindow(m->hme);
					BSTR nm = D2DGetName( D2DGetParent(m->hme));
					D2DSetStat(D2DGetControlFromName( w, NameGen(nm,L"k1")), STAT_DEFAULT);
					D2DSetStat(D2DGetControlFromName( w, NameGen(nm,L"k2")), STAT_DEFAULT);
					D2DSetStat(D2DGetControlFromName( w, NameGen(nm,L"k3")), STAT_DEFAULT);

					if ( m->child )
						m->child->SetStat(0);

					m->hprv.p = nullptr;
					1;
				}
	
			}

		}
		break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = m->mat.DPtoLP(pm.pt);
			FRectF rc = *(m->prc);
			if ( rc.PtInRect(pt) )
			{
				r2=1;
			}
			
		}
		break;
	}

	if ( r == 0 )
		r = D2DDefControlProc(m->hme,b,message,wParam,lParam);


	return r+r2;
}


void CreateCardControls(UIHandle h, int id)
{
	UIHandle ha[3]={};

	FRectF rc = D2DGetRect(h);
	
	ha[0] = D2DCreateWhiteControls( (LPVOID)new BobInstance(), df1,kf2,h, FRectF(0,0, FSizeF(20,rc.Height())), STAT_DEFAULT, L"k1", id);
	ha[1] = D2DCreateWhiteControls( (LPVOID)new BobInstance(), df1,kf2,h, FRectF(20,0, FSizeF(20,rc.Height())), STAT_DEFAULT, L"k2",id+1);
	ha[2] = D2DCreateWhiteControls( (LPVOID)new BobInstance(), df1,kf2,h, FRectF(40,0, FSizeF(20,rc.Height())), STAT_DEFAULT, L"k3",id+2);

	D2DColor c1(0x998a9e);
	D2DColor c2(0x8a8d9e);
	D2DColor c3(0x8a9e94);
	D2DColor c4(0x9e9e8a);

	D2DSetColor(ha[0], c1, ColorF::White,ColorF::Black);		
	D2DSetColor(ha[1], c2, ColorF::White,ColorF::Black);		
	D2DSetColor(ha[2], c3, ColorF::White,ColorF::Black);		

}


D2DControls* CreateSquare(UIHandle parent,  FSizeF size, LPCWSTR k )
{
	auto ret = D2DCreateSquarePaper(parent, FRectF(20,0,size), STAT_DEFAULT, NONAME, -1 );

	return (D2DControls*)ret.p;

}