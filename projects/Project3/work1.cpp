
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