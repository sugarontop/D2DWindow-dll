
#include "pch.h"
#include "inet.h"
#include "D2DMisc.h"

using namespace V6;
void work(IStream** ppJpg)
{
	InternetInfo* info =  CreateInternetInfo();

	info->bGet = true;
	info->url =  SysAllocString(L"https://192.168.10.65/zaimu/temp/youtube [#168].png");

	DWORD dw;
	auto h = CreateThread(0,0,InetAsync, info, 0, &dw);

	::WaitForSingleObject(h, INFINITE);


	
	if ( info->result == 200 )
	{		
		*ppJpg = info->pstream;

		long rc = (*ppJpg)->AddRef();

		_ASSERT(rc==2);

	}


	DeleteInternetInfo(info);
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


#include "D2D1UI_1.h"
#include "D2DControls_with_Scrollbar.h"
#include "D2DSquarePaper.h"
#include "D2DAccordionbar.h"

extern UIHandleWin hwin;

void CreateControl(HWND hWnd)
{
	hwin = D2DCreateMainHWnd(hWnd, 14,0);    
    auto root = D2DGetRootControls(hwin);

	auto hctrls = D2DCreateEmptyControls(hwin, root, FRectF(), STAT_DEFAULT, NONAME,-1);

	auto hd = D2DCreateSquarePaper(hwin, hctrls,  FRectF(0,0,9000,9000), STAT_DEFAULT, NONAME,-1);

	auto clr = D2RGBA(170,170,170,150);
	D2DSendMessage(hd, WM_D2D_SET_COLOR,1,(LPARAM)&clr);

	clr = D2RGBA(200,200,200,255);
	D2DSendMessage(hd, WM_D2D_SET_COLOR,0,(LPARAM)&clr);
	//auto p1 = center->GetMainControls();

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
	bar->CreateControl((D2DWindow*)hwin.p, (D2DControls*)hd.p, FRectF(550,100,FSizeF(20,100)), STAT_DEFAULT, NONAME);
	((D2DControls*)hd.p)->Add(bar);

	
	hctrls.p = bar.get();
	auto sq = D2DCreateSquarePaper(hwin,hctrls, FRectF(0,0,FSizeF(800,100)), STAT_DEFAULT, NONAME, 0);

	
	// //////////////////////////////////////////////////
	
	bar = std::make_shared<D2DAccordionbar>();
	bar->CreateControl((D2DWindow*)hwin.p, (D2DControls*)hd.p, FRectF(550,250,FSizeF(20,100)), STAT_DEFAULT, NONAME);
	((D2DControls*)hd.p)->Add(bar);

	hctrls.p = bar.get();
	sq = D2DCreateSquarePaper(hwin,hctrls, FRectF(0,0,FSizeF(800,100)), STAT_DEFAULT, NONAME, 0);

}