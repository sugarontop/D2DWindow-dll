
#include "pch.h"
#include "D2DContext.h"
#include "imagetool.h"


using namespace V6;

bool RenderTargetToBitmp(ID2D1RenderTarget* rt, D2D1_RECT_U areaRect, ID2D1Bitmap** nbmp)
{
    try
	{
	
		ComPTR<ID2D1Bitmap> bmp;

		float dpix, dpiy;
		rt->GetDpi(&dpix, &dpiy);
        
		D2D1_BITMAP_PROPERTIES Properties = D2D1::BitmapProperties(rt->GetPixelFormat(), dpix, dpiy);

		FSizeU sz(areaRect.right-areaRect.left, areaRect.bottom-areaRect.top);

		THR(rt->CreateBitmap(sz, Properties, &bmp));

		FPointU pt(0,0);
    
		THR(bmp->CopyFromRenderTarget(&pt, rt, &areaRect));

		bmp.AddRef();
		*nbmp = bmp;

		return true;
	}
	catch(...)
	{

		return false;
	}
}

bool BitmapSaveToFile(ID2D1DeviceContext* cxt, LPCWSTR fnm, ID2D1Image* bmp2)
{
	try
	{
		ComPTR<IWICImagingFactory2> wic;
		THR(wic.CreateInstance(CLSID_WICImagingFactory2));

		ComPTR<IWICStream> stream;
		THR(wic->CreateStream(&stream));
		THR(stream->InitializeFromFilename(fnm, GENERIC_WRITE));

		ComPTR<IWICBitmapEncoder> encoder;
		THR(wic->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder));
		THR(encoder->Initialize(stream, WICBitmapEncoderNoCache));

		ComPTR<IWICBitmapFrameEncode> frameEncode;
		THR(encoder->CreateNewFrame(&frameEncode, nullptr));	
		THR(frameEncode->Initialize(nullptr));

		ComPTR<IWICImageEncoder> imageEncoder;
		ComPTR<ID2D1Device> device;
		cxt->GetDevice(&device);
		THR(wic->CreateImageEncoder(device,&imageEncoder));
            
		THR(imageEncoder->WriteFrame(bmp2, frameEncode, nullptr));
            
		THR(frameEncode->Commit());
		THR(encoder->Commit());
		THR(stream->Commit(STGC_DEFAULT));
	
		return true;
	}
	catch(...)
	{

		return false;
	}

}


bool CreateBitampFromStream(ID2D1RenderTarget* pRenderTarget, IStream* sm, ID2D1Bitmap** nbmp)
{
    
	try
	{
		//WICPixelFormatGUID format = GUID_WICPixelFormat32bppPBGRA;

		_ASSERT( sm && pRenderTarget );

		ComPTR<IWICImagingFactory> pIWICFactory;
		ComPTR<IWICBitmapDecoder> pbmpdecoder;
		ComPTR<IWICBitmapFrameDecode> pWICBitmapFrame;
		ComPTR<IWICFormatConverter> pFormatConverter;

    
		LARGE_INTEGER s1={0};
		sm->Seek(s1,0,0);
   
		
		THR(pIWICFactory.CreateInstance(CLSID_WICImagingFactory));
		THR(pIWICFactory->CreateDecoderFromStream(sm,NULL, WICDecodeMetadataCacheOnLoad, &pbmpdecoder ));
		THR(pbmpdecoder->GetFrame( 0, &pWICBitmapFrame ));
		THR(pIWICFactory->CreateFormatConverter( &pFormatConverter ));
		THR(pFormatConverter->Initialize(
								  pWICBitmapFrame                   // BitmapSource
								, GUID_WICPixelFormat32bppPRGBA     // ピクセルフォーマット
								, WICBitmapDitherTypeNone           // BitmapDitherType
								, NULL                              // バレット
								, 1.0f                              // 透過率
								, WICBitmapPaletteTypeMedianCut     // パレットタイプ
							));

		// ID2D1Bitmap作成、pRenderTargetが変わると使用できない
		THR(pRenderTarget->CreateBitmapFromWicBitmap( pFormatConverter, NULL, nbmp ));

		return true;
	}
	catch(...)
	{
		return false;
	}
}



bool CreateMemoryRenderTarget( UINT width, UINT height, ID2D1RenderTarget** out)
{
	try
	{
		ComPTR<IWICImagingFactory> pIWICFactory;		
	
		THR(pIWICFactory.CreateInstance(CLSID_WICImagingFactory));

		ComPTR<IWICBitmap> pWICBitmap;

		THR(pIWICFactory->CreateBitmap(
			width,
			height, 
			GUID_WICPixelFormat32bppPRGBA,
			WICBitmapCacheOnDemand,
			&pWICBitmap
			));

		ComPTR<ID2D1Factory> pD2DFactory;

		THR(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&pD2DFactory
			));

		D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED
			);

		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
		//props.pixelFormat = pixelFormat;

		ComPTR<ID2D1RenderTarget> pRT;

		// Create a Direct2D render target.
		THR(pD2DFactory->CreateWicBitmapRenderTarget(
			pWICBitmap,
			props,
			&pRT
			));


		pRT.AddRef();
		*out = pRT;

		return true;
	}
	catch(...)
	{
		return false;
	}
}



bool BitmpaToIWICBitmap(ID2D1Bitmap* pBitmap, IWICBitmap** out)
{
	try
	{
		ComPTR<ID2D1Factory> pD2DFactory;
		ComPTR<IWICBitmap> pWICBitmap;
		ComPTR<ID2D1RenderTarget> pRT;
		ComPTR<IWICImagingFactory> pIWICFactory;

		//WICPixelFormatGUID format = GUID_WICPixelFormat32bppPBGRA;

		pBitmap->GetFactory(&pD2DFactory);
	
		THR(pIWICFactory.CreateInstance(CLSID_WICImagingFactory));

		

	
		UINT sc_bitmapWidth = (UINT)pBitmap->GetSize().width;
		UINT sc_bitmapHeight = (UINT)pBitmap->GetSize().height;

		{
			THR(pIWICFactory->CreateBitmap(
				sc_bitmapWidth,
				sc_bitmapHeight,
				GUID_WICPixelFormat32bppPRGBA,
				WICBitmapCacheOnLoad,
				&pWICBitmap
				));
		}
	
		{
			D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
	
			THR(pD2DFactory->CreateWicBitmapRenderTarget(
				pWICBitmap,
				rtProps,
				&pRT
				));
		}
	
		{
			pRT->BeginDraw();	
			pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));	
			pRT->DrawBitmap(pBitmap);	
			THR(pRT->EndDraw());		
		}

		pWICBitmap.AddRef();
		*out = pWICBitmap;

		return true;
	}
	catch(...)
	{
		return false;
	}
}


bool SaveBitmapToFile(LPCWSTR fnm, ID2D1Bitmap* pBitmap)
{
	try
	{
		ComPTR<IWICBitmap> pWICBitmap;
		ComPTR<IWICBitmapEncoder> pEncoder;
		ComPTR<IWICBitmapFrameEncode> pFrameEncode;
		ComPTR<IWICStream> pStream;
		ComPTR<IWICImagingFactory> pIWICFactory;
	
		// conert ID2D1Bitmap to IWICBitmap
		BitmpaToIWICBitmap(pBitmap, &pWICBitmap);

		UINT sc_bitmapWidth = (UINT)pBitmap->GetSize().width;
		UINT sc_bitmapHeight = (UINT)pBitmap->GetSize().height;

		THR(CoCreateInstance(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&pIWICFactory)
				));

		WICPixelFormatGUID format = GUID_WICPixelFormat32bppPBGRA;
		
		THR(pIWICFactory->CreateStream(&pStream));
		THR(pStream->InitializeFromFilename(fnm, GENERIC_WRITE));
		THR(pIWICFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder));
		THR(pEncoder->Initialize(pStream, WICBitmapEncoderNoCache));
		THR(pEncoder->CreateNewFrame(&pFrameEncode, NULL));
		THR(pFrameEncode->Initialize(NULL));
		THR(pFrameEncode->SetSize(sc_bitmapWidth, sc_bitmapHeight));
		THR(pFrameEncode->SetPixelFormat(&format));
		THR(pFrameEncode->WriteSource(pWICBitmap, NULL));
		THR(pFrameEncode->Commit());
		THR(pEncoder->Commit());
		return true;
	}
	catch( ...)
	{
		return false;

	}
}