
#include "pch.h"
#include "D2DContext.h"
#include "comptr.h"
using namespace V6;

#define SOK(a)   (S_OK == a)

void D2DContext::CreateDeviceIndependentResources( float fontheight)
{
	ComPTR<IDWriteFactory> dwriteFactory;	
	textformat_.Release();
	default_font_height_ = fontheight;



	if ( SOK(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**) &dwriteFactory)))
	{
		wfactory_ = dwriteFactory;
		if (SOK(dwriteFactory->CreateTextFormat(L"MS–¾’©", NULL, DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,fontheight, L"ja-jp", & textformat_)))
			{
				tsf_text_format_ = textformat_;
				tsf_wfactory_ = dwriteFactory;
			}
			
	}

	

}

void D2DContext::CreateDeviceResources(ID2D1DeviceContext* target)
{
	target_->CreateSolidColorBrush( D2RGB(0,0,0), &black_);
	target_->CreateSolidColorBrush( D2RGB(255,255,255), &white_);
	target_->CreateSolidColorBrush( D2RGBA(0,0,0,0), &transparent_);

}

void D2DContext::InitWICImagingType(UINT cx, UINT cy, IWICBitmap** pwicbitmap)
{
	_ASSERT(target_ == nullptr);
	ComPTR<IWICImagingFactory> wic;
	ComPTR<ID2D1Factory> fct;
	ComPTR<ID2D1RenderTarget> bmpRt;
	LRESULT hr;

	hr = ::CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_ALL, IID_IWICImagingFactory, (void**)&wic);


    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &fct);

    // this is the bitmap we'll draw to
    hr = wic->CreateBitmap(cx, cy, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, pwicbitmap);

    hr = fct->CreateWicBitmapRenderTarget(*pwicbitmap, D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),96.0f, 96.0f),
        &bmpRt);

	
	CreateDeviceResources(target_); // ????
}
void D2DContext::CreateDeviceResources(HWND hWnd, UINT cx, UINT cy)
{
	_ASSERT(target_ == nullptr);
	#ifdef USE_ID2D1DEVICECONTEXT
	
	UINT width = cx;
	UINT height = cy;

	ComPTR<ID2D1DeviceContext> d2d1DeviceContext;
	ComPTR<IDXGISwapChain1> dxgiSwapChain1;
	ComPTR<ID2D1Device> d2d1Device;
	ComPTR<ID2D1Bitmap1> d2d1Bitmap;

	ComPTR<ID3D11DeviceContext> d3d11DeviceContext;
	ComPTR<ID3D11Device> d3d11Device;
	ComPTR<IDXGISurface> dxgiSurface;
	ComPTR<IDXGIAdapter> dxgiAdapter;
	ComPTR<IDXGIFactory2> dxgiFactory;
	ComPTR<IDXGIDevice1> dxgiDevice;

	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; 
	
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
 
	D3D_FEATURE_LEVEL returnedFeatureLevel;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	ComPTR<ID2D1Factory1> d2d1Factory;
	D2D1_FACTORY_OPTIONS options;	
	options.debugLevel = D2D1_DEBUG_LEVEL_NONE;

	THR( D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED,__uuidof(ID2D1Factory1),&options,(void**)&d2d1Factory ));


	THR( D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
					&d3d11Device, &returnedFeatureLevel, &d3d11DeviceContext));


	THR(d3d11Device->QueryInterface(&dxgiDevice));
	THR(d2d1Factory->CreateDevice(dxgiDevice.p, &d2d1Device));
	THR(d2d1Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2d1DeviceContext));
	THR(dxgiDevice->GetAdapter(&dxgiAdapter));
	THR(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;	
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	THR(dxgiFactory->CreateSwapChainForHwnd(d3d11Device,
													hWnd,
													&swapChainDesc,
													nullptr,
													nullptr,
													&dxgiSwapChain1)); //Resource‰ð•ú–Y‚ê‚ª‚ ‚é‚Æ‚±‚±‚Å—Ž‚¿‚éB


	

	_ASSERT( dxgiSwapChain1 );
	dxgiSwapChain_ = dxgiSwapChain1;

	target_ = d2d1DeviceContext; 

	THR(dxgiDevice->SetMaximumFrameLatency(1));
	InitializeRenderTarget();

	CreateDeviceResources(target_);

#endif
}
void D2DContext::InitializeRenderTarget()
{
	ComPTR<ID2D1Bitmap1> d2d1RenderTarget;
	ComPTR<IDXGISurface2> dxgiSurface;
	auto hr = dxgiSwapChain_->GetBuffer(	0, IID_PPV_ARGS(&dxgiSurface));
	
	hr = target_->CreateBitmapFromDxgiSurface(
		dxgiSurface.p,
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)),
		&d2d1RenderTarget);
	
	target_->SetTarget(d2d1RenderTarget.p);
}

void D2DContext::SwapChain()
{
	_ASSERT(dxgiSwapChain_);
	dxgiSwapChain_->Present(1, 0);
}

bool D2DContext::CreateBrush(ColorF clr, ID2D1SolidColorBrush** pbr)
{	
	return (S_OK == target_->CreateSolidColorBrush(clr, pbr));

}

void D2DContext::DrawBlack(float x, float y, float w, float h)
{
	D2D1_RECT_F rc={x,y,x+w,y+h};
	target_->FillRectangle(rc, black_);
}
void D2DContext::DrawBlackLine(D2D1_RECT_F& rc)
{	
	ComPTR<ID2D1SolidColorBrush> br;
	target_->CreateSolidColorBrush( D2RGB(200,200,200), &br);
	target_->DrawRectangle(rc, br);
}

void D2DContext::DDrawRect(const D2D1_RECT_F& rc, ColorF lineclr, ColorF fillclr)
{
	ComPTR<ID2D1SolidColorBrush> br1,br2;
	if ( S_OK == target_->CreateSolidColorBrush(lineclr, &br1))
		target_->DrawRectangle(rc, br1); 	
	if ( S_OK == target_->CreateSolidColorBrush(fillclr, &br2))	
		target_->FillRectangle(rc, br2);	
}
void D2DContext::DFillRect(const D2D1_RECT_F& rc, ColorF clr)
{
	ComPTR<ID2D1SolidColorBrush> br;
	if ( S_OK == target_->CreateSolidColorBrush( clr, &br))
		target_->FillRectangle(rc, br);
}
void D2DContext::DCircle(const D2D1_RECT_F& rc, ColorF clr)
{
	ComPTR<ID2D1SolidColorBrush> br;
	D2D1_ELLIPSE crc; 
	crc.point = FPointF((rc.left+rc.right)/2.0f, (rc.top+rc.bottom)/2.0f );
	crc.radiusX = (rc.right-rc.left)/2.0f;
	crc.radiusY = (rc.bottom-rc.top)/2.0f;
	if ( S_OK == target_->CreateSolidColorBrush(clr, &br))
		target_->FillEllipse(crc, br);
}
void D2DContext::DText(const D2D_POINT_2F& pt, LPCWSTR str, ColorF clr)
{
	FRectF rc( pt, FSizeF(1000,1000));
	ComPTR<ID2D1SolidColorBrush> br;
	if ( S_OK == target_->CreateSolidColorBrush( clr, &br))	
		target_->DrawText(str,(UINT32)wcslen(str),this->textformat_, rc, br );

}

void D2DContextEx::DoRedraw(HWND hwnd)
{

	if ( !stepups_.empty())
	{
		bool bl = false;
		std::vector<INT_PTR> erase_ar;
		for(auto& it : stepups_ )
		{
			// animation process
			bl = it.second();

			if ( !bl )
				erase_ar.push_back(it.first);
		}

		InvalidateRect(hwnd,NULL,FALSE);

		for(auto& it : erase_ar )
			stepups_.erase(it);

	}
	else if ( bRedraw_ )
    {
        InvalidateRect(hwnd,NULL,FALSE);        
    }

	bRedraw_ = false;

}

void D2DContextEx::OnSize(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	auto hr = dxgiSwapChain_->GetDesc1(&swapChainDesc);
	
	target_->SetTarget(nullptr);

	hr = dxgiSwapChain_->ResizeBuffers(
		swapChainDesc.BufferCount, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

	InitializeRenderTarget();
}
void D2DContextEx::OnDiscardGraphicsResources()
{
	black_.Release();
	white_.Release();
	transparent_.Release();
}
void D2DContextEx::OnRestructGraphicsResources()
{	
	CreateDeviceResources(target_);
}


bool D2DContext::CreateTextLayout(LPCWSTR str, D2D1_SIZE_F sz, IDWriteTextLayout** ret )
{
	return (0== tsf_wfactory_->CreateTextLayout(str, (UINT32)wcslen(str), this->textformat_, sz.width, sz.height, ret ));
}