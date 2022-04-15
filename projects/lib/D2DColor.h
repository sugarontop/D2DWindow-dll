#pragma once

#include "D2DApp.h"
#include "D2DMisc.h"

#include <vector>

namespace V6 
{


#define COLOR_IDX_BACK		0
#define COLOR_IDX_FORE		1
#define COLOR_IDX_BORDER	2
#define COLOR_IDX_SELECT	3
#define COLOR_IDX_FLOAT		4

class D2DColor
{
	public :
		D2DColor():clr_(ColorF::Black){};
		D2DColor(ColorF clr):clr_(clr){}

		D2DColor(DWORD dwRGBA, int):clr_(ColorF::Black)
		{
			//  ColorF(((dw&0xFF000000)>>24)/255.0f, ((dw&0x00FF0000)>>16)/255.0f, ((dw&0xFF00)>>8)/255.0f, (dw&0xFF)/255.0f )
			clr_ = D2DRGBA(dwRGBA);		
		}
		D2DColor(DWORD dwRGB):clr_(ColorF::Black)
		{
			clr_ = ColorF(((dwRGB&0xFF0000)>>16)/255.0f, ((dwRGB&0x00FF00)>>8)/255.0f, ((dwRGB&0xFF))/255.0f, 1.0f );
		}
		D2DColor(LPCWSTR sharp_str):clr_(ColorF::Black)
		{
			// #000000-#FFFFFF

			DWORD dwRGB;
			if ( ColorFromString(sharp_str, &dwRGB))

			if ( lstrlen(sharp_str) < 8 )
				clr_ = ColorF(((dwRGB&0xFF0000)>>16)/255.0f, ((dwRGB&0x00FF00)>>8)/255.0f, ((dwRGB&0xFF))/255.0f, 1.0f );
			else
				clr_ = D2DRGBA(dwRGB);		
		}
		DWORD ToInt() const
		{
			DWORD r =(DWORD)(clr_.r * 255.0f+0.499f);
			DWORD g =(DWORD)(clr_.g * 255.0f+0.499f);
			DWORD b =(DWORD)(clr_.b * 255.0f+0.499f);
			DWORD a =(DWORD)(clr_.a * 255.0f+0.499f);

			auto ret =  ((r << 24)|(g << 16) | (b << 8) | a);

			return ret;
		}
		void FromInt(DWORD dw)
		{
			clr_ = ColorF(((dw&0xFF000000)>>24)/255.0f, ((dw&0x00FF0000)>>16)/255.0f, ((dw&0xFF00)>>8)/255.0f, (dw&0xFF)/255.0f );
		}


		operator ColorF() const
		{
			return clr_;
		}
		D2DColor& operator =(ColorF& clr)
		{
			clr_ = clr;
			return *this;
		}

		bool Create(ID2D1RenderTarget* rt, ID2D1SolidColorBrush** ppbr)
		{
			return (S_OK == rt->CreateSolidColorBrush(clr_, ppbr));
		}

		static std::vector<D2DColor> Palettes(int typ)
		{
			std::vector<D2DColor> ar;


			ar.push_back(D2DColor(0x998a9e));
			ar.push_back(D2DColor(0x8a8d9e));
			ar.push_back(D2DColor(0x8a9e94));
			ar.push_back(D2DColor(0x9e9e8a));


			return ar;
		}

		static bool ColorFromString( LPCWSTR s, DWORD* ret);

	private :
		ColorF clr_;
};


};