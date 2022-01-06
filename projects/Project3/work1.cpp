
#include "pch.h"
#include "inet.h"
#include "D2DMisc.h"
#include <fstream>
#include "D2D1UI_1.h"
#include "D2DControls_with_Scrollbar.h"
#include "D2DSquarePaper.h"
#include "D2DAccordionbar.h"
#include "D2DFileManage.h"



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



void Stream2Bitmap( IStream* sm, ID2D1RenderTarget* target, ID2D1Bitmap** bmp)
{
	ComPTR<IWICImagingFactory> d2dWICFactory;
	ComPTR<IWICBitmapDecoder> d2dDecoder;
	ComPTR<IWICFormatConverter> d2dConverter;
	ComPTR<IWICBitmapFrameDecode> d2dBmpSrc;

	auto hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, 
		__uuidof(IWICImagingFactory), (void**)(&d2dWICFactory));
	
	_ASSERT(hr == S_OK);
	hr = d2dWICFactory->CreateDecoderFromStream(sm, 0, WICDecodeMetadataCacheOnLoad, &d2dDecoder);
	_ASSERT(hr == S_OK);
	hr = d2dWICFactory->CreateFormatConverter(&d2dConverter);
	_ASSERT(hr == S_OK);
	hr = d2dDecoder->GetFrame(0, &d2dBmpSrc);
	_ASSERT(hr == S_OK);
	hr = d2dConverter->Initialize(d2dBmpSrc, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
	_ASSERT(hr == S_OK);
	hr = target->CreateBitmapFromWicBitmap(d2dConverter, NULL, bmp);

	_ASSERT(hr == S_OK);
}



extern UIHandleWin hwin;

//#define TEST1

#include "D2DLogin.h"
#define  APP (D2DApp::GetInstance())
void CreateDocumentControl(UIHandle h);
void CreateLoginControl(UIHandle h);

void CreateControl(HWND hWnd)
{
	hwin = D2DCreateMainHWnd(hWnd, 14,0);    
    auto root = D2DGetRootControls(hwin);

	auto hctrls = D2DCreateEmptyControls(hwin, root, FRectF(), STAT_DEFAULT, NONAME,-1);

	auto hctrlsA = D2DCreateSquarePaper(hwin, hctrls,  FRectF(0,0,9000,9000), STAT_VISIBLE, L"sqare",-1);

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

#include "xml.h"

std::vector<DocFunc> ar;

void CreateDocumentControl(UIHandle h)
{
	
	LoadDocument(L"doc.xml", ar);


	auto hdoc = D2DCreateEmptyControls(hwin, h, FRectF(), 0, L"document",-1); // login‚·‚é‚Ü‚Å‘€ì‚³‚¹‚È‚¢A•\Ž¦‚µ‚È‚¢B

	auto hlsb = D2DCreateDropdownListbox(hwin, hdoc, FRectF(100,100, FSizeF(400,20)), STAT_DEFAULT, L"ls",-1);

	
	int i = 0;
	for(auto& it : ar)
		D2DAddItem(hlsb,i++, it.title);
	

	auto hparams = D2DCreateTextbox(hwin,hdoc,FRectF(100,150,FSizeF(400,20)), false,STAT_DEFAULT, L"params" );

	auto htxt1 = D2DCreateTextbox(hwin,hdoc,FRectF(100,200,FSizeF(400,400)), true,STAT_DEFAULT, L"note1" );

	auto b1 = D2DCreateButton(hwin,hdoc,FRectF(100,650,FSizeF(100,26)), STAT_DEFAULT, L"save", -1);

	
	auto sel_change = [](void* sender, LPCWSTR eventnm, void* prm)->DWORD{

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

	D2DEventHandler(hlsb, sel_change);



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

			SaveDocument(idx, doc);
		}

		return 0;
	};

	D2DEventHandler(b1, save);


}
