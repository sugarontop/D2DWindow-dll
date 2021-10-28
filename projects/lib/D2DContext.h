#pragma once

#include "comptr.h"


namespace V6 {




class D2DContext 
{
	friend class D2DMatrix;
	
	public :
		D2DContext():target_(0),bRedraw_(false), temp_(0){}


		void InitWICImagingType(UINT cx, UINT cy,IWICBitmap** pwicbitmap);
		void CreateDeviceResources(HWND hWnd, UINT cx, UINT cy);
		void InitializeRenderTarget();

		//bool Init(ID2D1RenderTarget* target, float fontheight=13);

		void SwapChain();
		void Redraw(){	bRedraw_=true;}

		void CreateDeviceIndependentResources( float fontheight);
		void CreateDeviceResources(ID2D1DeviceContext* target);

		ID2D1DeviceContext* operator *(){ return target_; }

		void DrawBlack(float x, float y, float w, float h);
		void DrawBlackLine(D2D1_RECT_F& rc);
		void DFillRect(const D2D1_RECT_F& rc, ColorF clr);
		void DDrawRect(const D2D1_RECT_F& rc, ColorF lineclr, ColorF fillclr);
		void DCircle(const D2D1_RECT_F& rc, ColorF clr);

		bool CreateTextLayout(LPCWSTR str, D2D1_SIZE_F sz, IDWriteTextLayout** ret );


		ComPTR<ID2D1SolidColorBrush> black_;
		ComPTR<ID2D1SolidColorBrush> white_;
		ComPTR<IDXGISwapChain1> dxgiSwapChain_;

		ComPTR<IDWriteTextFormat> textformat_;
		ComPTR<IDWriteFactory> wfactory_;

		bool bRedraw_;
		void* temp_;

		ComPTR<IDWriteTextFormat> tsf_text_format_;
		ComPTR<IDWriteFactory> tsf_wfactory_;

	protected :
		ComPTR<ID2D1DeviceContext> target_;
		float default_font_height_;
		
};

class D2DContextEx : public D2DContext
{
	public :
		D2DContextEx(){};		
		void DoRedraw(HWND hwnd);
		void OnSize(HWND hWnd);
		void OnDiscardGraphicsResources();
		void OnRestructGraphicsResources();

	typedef std::function<bool(void)> FuncStepup;

	std::map<INT_PTR,FuncStepup> stepups_;

};


struct D2DRectFilter
{
	D2DRectFilter(D2DContext& cxt1, const FRectF& rc ):cxt(cxt1)
	{
		(*cxt)->PushAxisAlignedClip( rc, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE );		
		cnt = 1;
	}

	D2DRectFilter(D2DContext& cxt1, FRectF rc, float plusw, float plush=0 ):cxt(cxt1)
	{
		rc.right += plusw;
		rc.bottom += plush;
		(*cxt)->PushAxisAlignedClip( rc, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE );		
		cnt = 1;
	}

	~D2DRectFilter()
	{
		if ( cnt == 1 )
			(*cxt)->PopAxisAlignedClip();
	}
	void Off()
	{		
		if ( cnt == 1 )
		{
			(*cxt)->PopAxisAlignedClip();
			cnt = 0;
		}
	}

private :
	D2DContext& cxt;
	int cnt;
};

inline void ThrowIfFailed( HRESULT hr, LPCWSTR msg, UINT line, LPCSTR fnm )
{
	if (FAILED(hr))
	{		
		//General access denied error 0x80070005 


		int a = 0;
	}
}
#define THR(hr) ThrowIfFailed(hr,nullptr, __LINE__, __FILE__)

inline HRESULT NoThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        // Set a breakpoint on this line to catch Win32 API errors.
		//TRACE( L"NoThrowIfFailed %x\n", hr );        
    }

	return hr;
}

}; // v6