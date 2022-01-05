
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


void CreateControl(HWND hWnd)
{
	hwin = D2DCreateMainHWnd(hWnd, 14,0);    
    auto root = D2DGetRootControls(hwin);

	auto hctrls = D2DCreateEmptyControls(hwin, root, FRectF(), STAT_DEFAULT, NONAME,-1);

	auto hctrlsA = D2DCreateSquarePaper(hwin, hctrls,  FRectF(0,0,9000,9000), STAT_VISIBLE, L"sqare",-1);

	D2DSetColor(hctrlsA, ColorF::Black, ColorF::White,ColorF::Black);


	auto login = std::make_shared<D2DLogin>();

	auto kc = (D2DControls*)hctrlsA.p;
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
			
			return 0;

		}
		p1[2] = ::SysAllocString( L"login fail.");


		return 1;
	};

	APP.SetCapture(login.get());



}



void CreateControl2(HWND hWnd)
{
	hwin = D2DCreateMainHWnd(hWnd, 14,0);    
    auto root = D2DGetRootControls(hwin);

	auto hctrls = D2DCreateEmptyControls(hwin, root, FRectF(), STAT_DEFAULT, NONAME,-1);

	auto hd = D2DCreateSquarePaper(hwin, hctrls,  FRectF(0,0,9000,9000), STAT_DEFAULT, L"sqare",-1);



	auto tlog = D2DCreateButton(hwin, hd, FRectF(700,100,FSizeF(200,200)), STAT_DEFAULT, NONAME,999);
	D2DSetText(tlog, L"LOGIN");

	
	auto func = [](void* sender, void*)->DWORD
	{
		int a = 0;

		auto login = std::make_shared<D2DLogin>();

		auto kc = ((D2DControl*)sender)->GetParentControls(); 
		login->CreateControl((D2DWindow*)hwin.p, kc, FRectF(100,100,FSizeF(350,350)), STAT_DEFAULT|STAT_MODAL, L"login" );
		kc->Add( login );


		login->on_try_login_ = [](void* sender, void* p)->DWORD
		{
			BSTR* p1 = (BSTR*)p;

			std::wstring cd = p1[0];
			std::wstring pwd = p1[1];

			if ( cd == pwd )
				return 0;


			p1[2] = ::SysAllocString( L"login fail.");


			return 1;
		};

		APP.SetCapture(login.get());

		return 0;
	};
	
	
	D2DEventHandlerOnClick(tlog, func );
	
	



	auto clr = D2RGBA(170,170,170,150);
	D2DSendMessage(hd, WM_D2D_SET_COLOR,1,(LPARAM)&clr);

	clr = D2RGBA(200,200,200,255);
	D2DSendMessage(hd, WM_D2D_SET_COLOR,0,(LPARAM)&clr);
	//auto p1 = center->GetMainControls();


	//auto sccontrols = std::make_shared<D2DControls_with_Scrollbar>();
	//sccontrols->CreateControl((D2DWindow*)hwin.p, (D2DControls*)hd.p, FRectF(700,100,FSizeF(500,600)), STAT_DEFAULT, L"filemng_sc");
	//((D2DControls*)hd.p)->Add(sccontrols);

	//auto fmg = std::make_shared<D2DFileManage>();
	//fmg->CreateControl((D2DWindow*)hwin.p, sccontrols.get(), FRectF(0,0,FSizeF(700,500)), STAT_DEFAULT, L"filemng");
	//sccontrols->Add(fmg);




	FRectF rc(200,100,FSizeF(300,20));

	for(int i=0; i < 10; i++ )
	{
		auto tx = D2DCreateTextbox(hwin, hd, rc, false, STAT_DEFAULT, NONAME );

		auto clr = D2RGBA(255,0,0,0);
		D2DSendMessage(tx, WM_D2D_SET_COLOR,0,(LPARAM)&clr); // back
		clr = D2RGBA(255,255,255,255);
		D2DSendMessage(tx, WM_D2D_SET_COLOR,1,(LPARAM)&clr); // fore
		clr = D2RGB(100,100,100);
		D2DSendMessage(tx, WM_D2D_SET_COLOR,2,(LPARAM)&clr); // border

		rc.Offset(0, 30);
	}

	
	auto bar = std::make_shared<D2DAccordionbar>();
	bar->CreateControl((D2DWindow*)hwin.p, (D2DControls*)hd.p, FRectF(550,100,FSizeF(20,100)), STAT_DEFAULT,  L"my accordion");
	((D2DControls*)hd.p)->Add(bar);

	
	hctrls.p = bar.get();
	auto sq = D2DCreateSquarePaper(hwin,hctrls, FRectF(0,0,FSizeF(800,100)), STAT_DEFAULT, L"square paper", 0);

	
	// //////////////////////////////////////////////////
	
	bar = std::make_shared<D2DAccordionbar>();
	bar->CreateControl((D2DWindow*)hwin.p, (D2DControls*)hd.p, FRectF(550,250,FSizeF(20,100)), STAT_DEFAULT, L"my accordion1");
	((D2DControls*)hd.p)->Add(bar);

	hctrls.p = bar.get();
	sq = D2DCreateSquarePaper(hwin,hctrls, FRectF(0,0,FSizeF(800,100)), STAT_DEFAULT, L"square paper1", 0);







	D2DWindow* pw = (D2DWindow*)hwin.p;
	
	//ComPTR<IStream> sm;
	//work(&sm);
	//ComPTR<ID2D1Bitmap> bmp;
	//Stream2Bitmap( sm, *(pw->cxt) , &bmp);

	
	auto ls = D2DCreateListbox(hwin, hd, FRectF(100,450,FSizeF(300,300)), STAT_DEFAULT, NONAME );

#ifdef TEST1

	LPCWSTR fnm[] = {L"flag_canada.png",L"flag_andorra.png",L"flag_argentina.png",L"flag_australia.png",
	L"flag_belgium.png",L"flag_botswana.png",L"flag_brasil.png",L"flag_bulgaria.png",L"flag_cameroon.png",
	L"flag_canada.png",L"flag_central_african_republic.png",L"flag_chile.png",L"flag_colombia.png"
	};
	

	for(int i = 0; i < _countof(fnm); i++)
	{
		std::wstring cfnm = L".\\res\\";
		cfnm += fnm[i];
		
		ComPTR<IStream> sm;
		ComPTR<ID2D1Bitmap> bmp;
		if (FileReadStream( cfnm.c_str(), &sm ))
			Stream2Bitmap( sm, *(pw->cxt) , &bmp);

		auto p = bmp.Detach();

		D2DSendMessage(ls, WM_D2D_LISTBOX_ADD_ITEM, 1,(LPARAM)p);
	}
#else

	UIHandle ctrls = D2DCreateControls(hwin, hd, FRectF(0,0,1,1), STAT_ENABLE, NONAME,-1);

	for(int i = 0; i < 8; i++ )
	{
		auto c1 = D2DCreateControls(hwin, ctrls, FRectF(0,0,200,50), STAT_DEFAULT, NONAME,-1);
		
		
		
		D2DCreateStatic(hwin,c1,FRectF(10,10,FSizeF(50,25)), STAT_DEFAULT, L"TEXT:", NONAME );
		//D2DCreateTextbox(hwin,c1,FRectF(60,10,FSizeF(200,25)), FALSE, STAT_DEFAULT, NONAME );
		D2DCreateButton(hwin,c1,FRectF(60,10,FSizeF(100,25)),STAT_DEFAULT, NONAME,-1);
	}


	D2DSendMessage(ls, WM_D2D_LISTBOX_ADD_ITEM, 2,(LPARAM)ctrls.p);

	
	
	
	
	
	
	
	



#endif
}