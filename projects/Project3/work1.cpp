
#include "pch.h"
#include "inet.h"
#include "D2DMisc.h"
#include <fstream>
#include "D2D1UI_1.h"
//#include "D2DControls_with_Scrollbar.h"
#include "D2DSquarePaper.h"
#include "D2DAccordionbar.h"
#include "D2DFileManage.h"
#include "xml.h"
#include "D2DWhiteWindowControl.h"
#include "D2DColor.h"

#include "D2DLogin.h"
//#include "yahoo.h"
#include "chart_top.h"

using namespace V6;

bool FileReadStream( LPCWSTR fnm, IStream** sm )
{
	LRESULT hr = ::CreateStreamOnHGlobal(NULL,TRUE, sm);

	CREATEFILE2_EXTENDED_PARAMETERS cfprm = {};
	cfprm.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
	
	HANDLE h = ::CreateFile2(fnm,GENERIC_READ,FILE_SHARE_READ,OPEN_EXISTING, &cfprm);

	if ( h != (HANDLE)-1)
	{
		byte cb[256];
		DWORD dcb;
		while( ReadFile(h,cb,256,&dcb,nullptr) && dcb != 0)
		{			
			ULONG dlen;
			(*sm)->Write(cb, (ULONG)dcb, &dlen);
		}

		::CloseHandle(h);

		ULARGE_INTEGER ui;
		LARGE_INTEGER uii={};
		(*sm)->Seek(uii,STREAM_SEEK_SET,&ui);
		return true;
	}

	return false;
}


extern UIHandleWin hwin;


#define  APP (D2DApp::GetInstance())
void CreateDocumentControl(UIHandle h);
void CreateLoginControl(UIHandle h);
std::vector<DocFunc> ar;

void CenterDraw(LPVOID captureobj, D2DContext& cxt);
LRESULT CenterFunc(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);



void CreateControl(HWND hWnd)
{

	hwin = D2DCreateMainHWnd(hWnd, 14,0);    
    auto root = D2DGetRootControls(hwin);

	auto hctrlsA = D2DCreateEmptyControls( root, FRectF(), STAT_DEFAULT, NONAME,-1);

	//auto hctrlsA = D2DCreateSquarePaper( hctrls,  FRectF(0,0,9000,9000), STAT_VISIBLE, L"sqare",-1);

	//D2DSetColor(hctrlsA, ColorF::Black, ColorF::White,ColorF::Black);

	CreateLoginControl(hctrlsA);

	CreateDocumentControl(hctrlsA);

}
void CreateLoginControl(UIHandle h)
{
	auto login = std::make_shared<D2DLogin>();

	auto kc = (D2DControls*)h.p;
	login->CreateControl((D2DWindow*)hwin.p, kc, FRectF(100,100,FSizeF(350,350)), STAT_DEFAULT|STAT_MODAL, L"login" );
	kc->Add( login );


	login->on_try_login_ = [](void* sender, void* p)->DWORD
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

			h = D2DGetControlFromName(hwin, L"document");

			if ( h.p )
				D2DSetStat(h, STAT_DEFAULT);


			return 0;

		}
		p1[2] = ::SysAllocString( L"login fail.");


		return 1;
	};

	APP.SetCapture(login.get());


}


struct BobInstance
{
	BobInstance()
	{
		clr[0] = ColorF::AliceBlue;
		clr[1] = ColorF::AliceBlue;
		clr[2] = ColorF::AliceBlue;
	
	};

	FRectF* prc;
	FRectF orgrc;
	D2DColor clr[3];
	D2DMat mat;
	UIHandle hme;

	UIHandle hactive;

};



bool df1(LPVOID captureobj, D2DContext& cxt)
{
	BobInstance* m = (BobInstance*)captureobj;

	
	D2DMatrix mat(*cxt);

	m->mat = mat.PushTransform();


	auto rc = *(m->prc);

	D2DRectFilter fil(cxt, rc);
	
	mat.Offset(rc);
	
	cxt.DFillRect(rc.ZeroRect(), m->clr[0]);


	FRectF rc1 = rc.ZeroRect();
	rc1.InflateRect(-3,-3);
	cxt.DDrawRect(rc1, m->clr[1], m->clr[0]);

	D2DControls* c = (D2DControls*)m->hme.p;
	c->InnerDraw(cxt);


	mat.PopTransform();

	return false;


}
LRESULT df2(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	BobInstance* m = (BobInstance*)captureobj;
	auto me = (D2DControl*)b.card;


	LRESULT r = 0;

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
				auto tx = D2DCreateTextbox(m->hme, FRectF(10,10,200,200), true, STAT_DEFAULT, L"TX1",-1,-1);

				D2DSetText( tx, L"4 split screens.\n ダブルクリックで全体画面へ。");




			}
			else if ( 11 == id )
			{
			
				LPCWSTR prm = L"text=Quote n.28&color=[128,217,204]&align=center&fontheight=30";

				D2DCreateStatic(m->hme, FRectF(100,10,FSizeF(200,260)), STAT_DEFAULT, prm, NONAME, -2 );

				std::wstring prm2 = L"color=[128,217,204]&align=left&fontname=Meiryo&fontheight=16&text=";

				prm2 += L"When I was a boy I was told that anybody could becaome Persident.Now I'm beginning to believe it.";

				D2DCreateStatic(m->hme, FRectF(10,50,FSizeF(1200,260)), STAT_DEFAULT, prm2.c_str(), NONAME, -2 );
				
			}
			else if ( 10 == id )
			{
				CreateStockChart( (D2DControls*)m->hme.p,  FSizeF(1600,650) );


				//for(int ij=0; ij < 1; ij++)
				//{
				//	/*auto yf = std::make_shared<yahoo_finance>();

				//	auto w = D2DGetWindow(m->hme);

				//	yf->CreateControl((D2DWindow*)w.p, (D2DControls*)m->hme.p, FRectF(50+ij*100,100+ij*100,FSizeF(1000,500)), STAT_DEFAULT, L"yahoo_finance" );
				//	((D2DControls*)m->hme.p)->Add(yf);*/
	
				//}
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

				auto rc = *(m->prc);

				if ( rc.PtInRect(pt) )
				{
					r = 1;

					if ( m->hactive.p != m->hme.p )
					{
						m->hactive = m->hme;

						FRectF rcDst(0,0,rc.Width()*2,  rc.Height()*2);

						D2DSetTopControl(m->hme);

						D2DSmoothRect(1,99,  hwin, m->prc, rcDst);

					

					}
					else
					{
						auto w = D2DGetWindow(m->hme);
						D2DSetStat(D2DGetControlFromName( w, L"s1"), STAT_DEFAULT);
						D2DSetStat(D2DGetControlFromName( w, L"s2"), STAT_DEFAULT);
						D2DSetStat(D2DGetControlFromName( w, L"s3"), STAT_DEFAULT);
						D2DSetStat(D2DGetControlFromName( w, L"s4"), STAT_DEFAULT);


						FRectF rcDst = m->orgrc;

						D2DSmoothRect(1,98,  hwin, m->prc, rcDst);
						m->hactive.p = nullptr;


					
					}

				}
			}
			return r;
		}
		break;
		case WM_D2D_SMOOTH_COMPLETE:
		{
			if ( wParam == 99 )
			{
				if ( m->hactive.p == m->hme.p )
				{
					auto w = D2DGetWindow(m->hme);

					D2DSetStat(D2DGetControlFromName( w, L"s1"), 0);
					D2DSetStat(D2DGetControlFromName( w, L"s2"), 0);
					D2DSetStat(D2DGetControlFromName( w, L"s3"), 0);
					D2DSetStat(D2DGetControlFromName( w, L"s4"), 0);


					D2DSetStat(m->hme, STAT_DEFAULT);
					r = 1;
				}

			}
		}
		break;
		case WM_LBUTTONDOWN:
		{	
			int a = 0;
		}
		break;
	}

	if ( r == 0 )
		r = D2DDefControlProc(m->hme,b,message,wParam,lParam);


	return r;
}
void CreateDocumentControl(UIHandle h)
{
	auto cw = GetSystemMetrics(SM_CXSCREEN);
	auto ch = GetSystemMetrics(SM_CYSCREEN);	

	FSizeF sz(cw/2.0f,ch/2.0f);


	auto hdoc1 = D2DCreateXXXControls( h, FRectF(), 0, L"document",-1); // 0:loginするまで表示しない。
	
	UIHandle ha[4];
	ha[0] = D2DCreateWhiteControls( (LPVOID)new BobInstance(), df1,df2,hdoc1, FRectF(0,0, sz), STAT_DEFAULT, L"s1",10);
	ha[1] = D2DCreateWhiteControls( (LPVOID)new BobInstance(), df1,df2,hdoc1, FRectF(sz.width,sz.height,sz), STAT_DEFAULT, L"s2",11);
	ha[2] = D2DCreateWhiteControls( (LPVOID)new BobInstance(), df1,df2,hdoc1, FRectF(0,sz.height,sz), STAT_DEFAULT, L"s3",12);
	ha[3] = D2DCreateWhiteControls( (LPVOID)new BobInstance(), df1,df2,hdoc1, FRectF(sz.width,0,sz), STAT_DEFAULT, L"s4",13);

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


}


