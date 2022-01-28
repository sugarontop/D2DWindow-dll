
#include "pch.h"
#include "inet.h"
#include "D2DMisc.h"
#include <fstream>
#include "D2D1UI_1.h"
#include "D2DControls_with_Scrollbar.h"
#include "D2DSquarePaper.h"
#include "D2DAccordionbar.h"
#include "D2DFileManage.h"
#include "xml.h"
#include "D2DWhiteWindowControl.h"


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



//void Stream2Bitmap( IStream* sm, ID2D1RenderTarget* target, ID2D1Bitmap** bmp)
//{
//	ComPTR<IWICImagingFactory> d2dWICFactory;
//	ComPTR<IWICBitmapDecoder> d2dDecoder;
//	ComPTR<IWICFormatConverter> d2dConverter;
//	ComPTR<IWICBitmapFrameDecode> d2dBmpSrc;
//
//	auto hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, 
//		__uuidof(IWICImagingFactory), (void**)(&d2dWICFactory));
//	
//	_ASSERT(hr == S_OK);
//	hr = d2dWICFactory->CreateDecoderFromStream(sm, 0, WICDecodeMetadataCacheOnLoad, &d2dDecoder);
//	_ASSERT(hr == S_OK);
//	hr = d2dWICFactory->CreateFormatConverter(&d2dConverter);
//	_ASSERT(hr == S_OK);
//	hr = d2dDecoder->GetFrame(0, &d2dBmpSrc);
//	_ASSERT(hr == S_OK);
//	hr = d2dConverter->Initialize(d2dBmpSrc, GUID_WICPixelFormat32bppPBGRA,
//		WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
//	_ASSERT(hr == S_OK);
//	hr = target->CreateBitmapFromWicBitmap(d2dConverter, NULL, bmp);
//
//	_ASSERT(hr == S_OK);
//}



extern UIHandleWin hwin;


#include "D2DLogin.h"
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

	auto hctrls = D2DCreateEmptyControls( root, FRectF(), STAT_DEFAULT, NONAME,-1);

	auto hctrlsA = D2DCreateSquarePaper( hctrls,  FRectF(0,0,9000,9000), STAT_VISIBLE, L"sqare",-1);

	D2DSetColor(hctrlsA, ColorF::Black, ColorF::White,ColorF::Black);

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

			D2DSetStat(h, STAT_DEFAULT);


			return 0;

		}
		p1[2] = ::SysAllocString( L"login fail.");


		return 1;
	};

	APP.SetCapture(login.get());


}

void PartsDraw(LPVOID captureobj, D2DContext& cxt);
LRESULT PartsFunc(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);

struct Base
{
	USHORT select_id;

};
struct Memo
{
	DocFuncItem item;
	FRectF rc;
	USHORT id;
	Base* base;
	D2DMat mat;
};



void CreateDocumentControl(UIHandle h)
{
	
	LoadDocument(L"doc.xml", ar);


	auto hdoc1 = D2DCreateXXXControls( h, FRectF(), 0, L"document",-1); // 0:loginÇ∑ÇÈÇ‹Ç≈ï\é¶ÇµÇ»Ç¢ÅB
	auto hdoc = D2DCreateWhiteControls((LPVOID)hdoc1.p, nullptr, CenterFunc, hdoc1, FRectF(0,0,10000,10000), STAT_DEFAULT, L"document2",-1);

	//auto d1 = std::make_shared<D2DWhiteWindowControls>();
	//auto hlsb = D2DCreateDropdownListbox( hdoc, FRectF(100,100, FSizeF(400,20)), STAT_DEFAULT, L"ls",-1);

	
	int i = 0;
	//for(auto& it : ar)
	//	D2DAddItem(hlsb,i++, it.title);
	

	auto hparams = D2DCreateTextbox(hdoc,FRectF(100,150,FSizeF(400,20)), false,STAT_DEFAULT, L"params" );

	auto htxt1 = D2DCreateTextbox(hdoc,FRectF(100,200,FSizeF(400,400)), true,STAT_DEFAULT, L"note1" );

	auto b1 = D2DCreateButton(hdoc,FRectF(100,650,FSizeF(100,26)), STAT_DEFAULT, L"save", -1);
	D2DEnable(b1, false);

	auto b2 = D2DCreateButton(hdoc,FRectF(300,650,FSizeF(100,26)), STAT_DEFAULT, L"minimum", -1);

	
	/*auto sel_change = [](void* sender, LPCWSTR eventnm, void* prm)->DWORD{

		if ( !wcscmp(L"SELECT_CHANGE", eventnm))
		{
			int idx = *(int*)prm;

			auto doc = ar[idx];


			auto h = D2DCast(sender);
			auto h1 = D2DGetControlFromName(hwin, L"params");
			D2DSetText(h1, doc.param);

			h1 = D2DGetControlFromName(hwin, L"note1");
			D2DSetText(h1, doc.note);
		}

		return 0;

	};

	D2DEventHandler(hlsb, sel_change);*/



	auto save = [](void* sender, LPCWSTR eventnm, void* prm)->DWORD{
		
		if ( !wcscmp(L"CLICK", eventnm))
		{
			auto h = D2DCast(sender);
			auto h1 = D2DGetControlFromName(hwin, L"ls");


			int idx=0;
			D2DSendMessage(h1, WM_D2D_CB_GETSELECT, 0, (LPARAM)&idx );
			auto doc = ar[idx];

			h1 = D2DGetControlFromName(hwin, L"params");
			BS b1 = D2DGetText(h1, true);

			h1 = D2DGetControlFromName(hwin, L"note1");
			BS b2 = D2DGetText(h1, true);

			doc.param = b1;
			doc.note = b2;

			ar[idx] = doc;

			SaveDocument(idx, doc);
		}

		return 0;
	};

	D2DEventHandler(b1, save);


	auto mini = [](void* sender, LPCWSTR eventnm, void* prm)->DWORD{

		if ( !wcscmp(L"CLICK", eventnm))
		{
			auto h = D2DCast(sender);
			auto h1 = D2DGetControlFromName(hwin, L"document");
			
			D2DSendMessage(h1, WM_D2D_MINIMUM_SIZE, 0, (LPARAM)0 );

		
		}
		return 0;
	};

	D2DEventHandler(b2, mini);


	
	/// ///////////////////////////////////
	auto c1 = D2DCreateButton(hdoc,FRectF(100,700,FSizeF(100,26)), STAT_DEFAULT, L"dll.xml", -1);


	auto load = [](void* sender, LPCWSTR eventnm, void* prm)->DWORD{

		if ( !wcscmp(L"CLICK", eventnm))
		{
			auto h = D2DCast(sender);

			D2DEnable(h, false);

			auto h1 = D2DGetControlFromName(hwin, L"document");

			std::vector<DocFuncItem> ar;
			Load_DLL_Document(L"dll.xml", ar);

			FRectF rc(600,50,FSizeF(300,26));
			USHORT k=0;
			Base* b = new Base();
			b->select_id = -1;


			for(auto& it : ar )
			{
				Memo* m = new Memo();
				m->item = it;
				m->rc = rc;
				m->id = k++;
				m->base = b;

				auto x1 = D2DCreateWhiteControls(m, PartsDraw, PartsFunc, h1, rc, STAT_DEFAULT, NONAME,-1);

				rc.Offset(0,30);
			}
		}
		return 0;
	};

	D2DEventHandler(c1, load);


}


void PartsDraw(LPVOID captureobj, D2DContext& cxt)
{
	Memo* m = (Memo*)captureobj;
	

	D2DMatrix mat(*cxt);
	m->mat = mat.PushTransform();
	mat.Offset(m->rc);

	if ( m->base->select_id == m->id )
	{
		cxt.DFillRect(m->rc.ZeroRect(), ColorF::DarkViolet);
		cxt.DText(FPointF(), (LPCWSTR)m->item.func, ColorF::White);
	}
	else
	{
		cxt.DFillRect(m->rc.ZeroRect(), ColorF::FloralWhite);
		cxt.DText(FPointF(), (LPCWSTR)m->item.func);
	}

	



	mat.PopTransform();
}

#define WM_D2D_PARTS_SELECT_CHANGED		WM_D2D_USER_FIRST

LRESULT PartsFunc(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	Memo* m = (Memo*)captureobj;
	auto me = (D2DControl*)b.card;


	LRESULT r = 0;

	switch(message)
	{
		case WM_D2D_DESTROY:
			if ( m->id == 0)
				delete m->base;

			delete m;
		break;
		case WM_LBUTTONDOWN:
		{
			MouseParam& mouse = *(MouseParam*)lParam;

			if ( m->id == 0)
				m->base->select_id = -1; // reset

			auto pt = m->mat.DPtoLP(mouse.pt);
			if ( m->rc.PtInRect(pt))
			{
				m->base->select_id = m->id;

				me->GetParent()->SendMessage(WM_D2D_PARTS_SELECT_CHANGED, 0, (LPARAM)m);


				APP.SetCapture(me);

				


				r = 1;
			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			if (APP.IsCapture(me))
			{
				MouseParam& mouse = *(MouseParam*)lParam;
				
				auto pt = m->mat.DPtoLP(mouse.pt);
				auto pt1 = m->mat.DPtoLP(mouse.ptprv);				
				auto sz = pt - pt1;

				m->rc.Offset(sz);

				me->SetRect(m->rc);

				b.bRedraw = true;
				r = 1;
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			if (APP.IsCapture(me))
			{
				APP.ReleaseCapture();


				




				r = 1;
			}
		}
		break;
	}


	return r;
}

LRESULT CenterFunc(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;

	auto me = (D2DControl*)b.card;

	switch( message )
	{
		case WM_D2D_PARTS_SELECT_CHANGED:
		{
			Memo* m = (Memo*)lParam;

			auto h1 = D2DGetControlFromName(hwin, L"params");
			D2DSetText(h1, m->item.func);


			r = 1;

		}
		break;





	}

	return r;

}
