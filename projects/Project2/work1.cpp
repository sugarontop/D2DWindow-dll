
#include "pch.h"
#include "inet.h"
#include "D2DMisc.h"

using namespace V6;
void work(IStream** ppJpg)
{
	InternetInfo* info =  CreateInternetInfo();

	info->bGet = true;
	info->url =  SysAllocString(L"https://192.168.10.65/zaimu/temp/test1.jpg");

	DWORD dw;
	auto h = CreateThread(0,0,InetAsync, info, 0, &dw);

	::WaitForSingleObject(h, INFINITE);

	if ( info->result == 200 )
	{		
		*ppJpg = info->pstream;

		(*ppJpg)->AddRef();

	}


	DeleteInternetInfo(info);
}

void Stream2Bitmap( IStream* sm, ID2D1RenderTarget* target, ID2D1Bitmap** bmp)
{
	ComPTR<ID2D1Factory> d2dFactory;
	ComPTR<IWICImagingFactory> d2dWICFactory;
	ComPTR<IWICBitmapDecoder> d2dDecoder;
	ComPTR<IWICFormatConverter> d2dConverter;
	ComPTR<IWICBitmapFrameDecode> d2dBmpSrc;

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory), NULL, (void**)&d2dFactory);    


	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, 
		__uuidof(IWICImagingFactory), (void**)(&d2dWICFactory));

	hr = d2dWICFactory->CreateDecoderFromStream(sm, 0, WICDecodeMetadataCacheOnLoad, &d2dDecoder);

	hr = d2dWICFactory->CreateFormatConverter(&d2dConverter);

	hr = d2dDecoder->GetFrame(0, &d2dBmpSrc);

	hr = d2dConverter->Initialize(d2dBmpSrc, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);

	hr = target->CreateBitmapFromWicBitmap(d2dConverter, NULL, bmp);


}