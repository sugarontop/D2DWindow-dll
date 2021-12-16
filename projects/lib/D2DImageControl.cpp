#include "pch.h"
#include "D2DImageControl.h"
#include "FilePackTool.h"


using namespace V6;
using namespace FILEPACK;

#define  APP (D2DApp::GetInstance())


void D2DImageControl::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControl::CreateControl(parent, pacontrol, rc, stat, name, local_id);

    SetRect(rc);
}


LRESULT D2DImageControl::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;

	switch(message)
	{
		case WM_D2D_SET_RESOURCE:
		{
			LPCWSTR name = (LPCWSTR)lParam;
			SetImage(name);
			ret = 1;
		}
		break;
	}
	return ret;
}

void D2DImageControl::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat.PushTransform();

	mat.Offset(rc_);

	if ( img_ )
		(*cxt)->DrawImage(img_,FPointF(0,0), rc_.ZeroRect());

	mat.PopTransform();
}

void D2DImageControl::SetImage(LPCWSTR name )
{
	std::vector<Item> ar;
	
	IStream* sm;
	if ( 1 == parent_window_->SendMessage(WM_D2D_GET_RESOURCE_BINARY, 0, (LPARAM)&sm))
	{

		ListContents(sm, ar);
	
		img_.Release();
	
		for(auto& it : ar )
		{
			if ( it.fnm == name )
			{
				ComPTR<IStream> sm1;

				if (DeSerialize( sm, it, &sm1 ))
					Stream2Bitmap(sm1,*(parent_window_->GetContext()), &img_);
				
				img_name_ = name;
				break;
			}
		}
	}
}
void D2DImageControl::ResourceUpdate(bool bCreate, D2DContext& cxt)
{
	if (bCreate)
	{
		SetImage(img_name_.c_str());
	}
	else
	{
		img_.Release();

	}

}

void D2DImageControl::Stream2Bitmap( IStream* sm, ID2D1RenderTarget* target, ID2D1Bitmap** bmp)
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