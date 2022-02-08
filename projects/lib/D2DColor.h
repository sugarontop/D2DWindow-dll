#pragma once

#include "D2DApp.h"
#include "D2DMisc.h"

#include <vector>

namespace V6 
{


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
		operator ColorF() const
		{
			return clr_;
		}
		D2DColor& operator =(ColorF& clr)
		{
			clr_ = clr;
			return *this;
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

	private :
		ColorF clr_;
};


};