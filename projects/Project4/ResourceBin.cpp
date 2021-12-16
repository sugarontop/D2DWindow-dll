#include "pch.h"
#include "FilePackTool.h"

#include "ResourceBin.h"

using namespace FILEPACK;



bool LoadResource(LPCWSTR fnm, ResourceBin* rb)
{
	ComPTR<IStream> sm;
	ComPTR<IStream>& sm2 = rb->sm; 

	if ( !Convert2Stream(fnm,MAGIC_NUMBER, &sm) )
		return false;

	if ( !DeCompress(sm, &sm2))
		return false;

	std::vector<Item> ar;
	if (!ListContents(sm2, ar))
		return false;

	
	for(auto& it : ar )
	{
		ResourceItem item;
		item.fnm = it.fnm;
		item.len = it.size;
		item.pos = it.pos;
		
		rb->items[it.fnm] = item;
	}

	return true;

}


static void Stream2Bitmap( IStream* sm, ID2D1RenderTarget* target, ID2D1Bitmap** bmp)
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

bool GetResourcePng(ResourceBin& src, LPCWSTR nm, ID2D1RenderTarget* target, ID2D1Bitmap** pbmp)
{
	if ( src.items.find(nm) == src.items.end())
		return false;

	auto it = src.items[nm];

	FILEPACK::Item s;
	s.fnm = it.fnm;
	s.pos = it.pos;
	s.size = it.len;

	ComPTR<IStream> sm;

	if (DeSerialize( src.sm, s, &sm ))
		Stream2Bitmap(sm,target, pbmp);

	return true;
}
