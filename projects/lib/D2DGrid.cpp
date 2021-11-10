#include "pch.h"
#include "D2DGrid.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())


D2DGrid::D2DGrid()
{ 

}

void D2DGrid::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id )
{
	D2DControls::CreateControl(parent, pacontrol,rc,stat,name,local_id);

	rc_ = rc;
}
HRESULT D2DGrid::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = 0;

	switch(message)
	{
		case WM_D2D_CREATE:
			hr = 1;
		break;



	}
	return hr;

}


class D2DText
{
	public :
		enum HTYP { LEFT,CENTER,RIGHT };

		D2DText():offx_(0){}
		D2DText(D2DWindow* parent, float width, LPCWSTR str, HTYP center_typ, int ty )
		{
			auto& cxt = parent->cxt;					
			std::wstring cstr = convert(str, ty );
			cxt.tsf_wfactory_->CreateTextLayout(cstr.c_str(), cstr.length(),cxt.textformat_, 1000,1000,&layout_);
			DWRITE_TEXT_METRICS t;
			layout_->GetMetrics(&t);

			if ( center_typ == HTYP::RIGHT && width > 0)
				offx_ = width - t.width;
			else if (center_typ == HTYP::CENTER && width > 0)
				offx_ = (width - t.width)/2.0f;
			else
				offx_ = 0;
		}

		std::wstring convert(LPCWSTR str, int typ )
		{
			
			if ( typ & 0x1 )
			{
				// 3keta comma , "1000"->"1,000"
				WCHAR cb[64];

				WCHAR* p = (WCHAR*)str;

				while (!( ('0' <= *p && *p <= '9') || *p == '-' )) p++;
				const WCHAR* p1 = p;

				LPCWSTR str1 = (p[0]=='-' ? p+1 : p );

				p = (WCHAR*)str1;
				short len = 0;
				short comma_idx = 0, comma_len=0;
				short k=0, j=0;
				while(*p !=0)
				{ 
					if (comma_idx)
						comma_len++;
					else if ( *p == '.' )
						comma_idx = k;
					else
						 len++; 


					p++;
					k++;
				}

				j=0;
				k=0;
		

				for(short i=len-1; i >= 0; i-- )
				{
					cb[k++] = str1[i];
				}

				WCHAR db[64]={};
		
				for(short i=0; i <len; i++ )
				{
					if (i%3 == 0 && i > 0)
					{				
						db[j]= L',';
						j++;				
					}
					db[j++] = cb[i];
				}
		
				k=0;
				len = (short)wcslen(db);
				for(short i=len-1; i >= 0; i-- )
				{
					cb[k++] = db[i];
				}

				if ( comma_idx )
				{
					for(short i = 0; i < comma_len+1; i++ )
						cb[k++] = str1[comma_idx+i];

				}

				cb[k]=0;


				if ( p1[0]=='-' )
				{
					db[0]='-';
					memcpy(db+1, cb, (k+1)*sizeof(WCHAR));
					return db;
				}


				return cb;
			}

			return str;
		}



		
		void DText(D2DContext& cxt, const FPointF& pt, ID2D1Brush* br )
		{
			FPointF xpt(pt.x+offx_, pt.y);									
			(*cxt)->DrawTextLayout(xpt,layout_,br);
		}
	private:
		ComPTR<IDWriteTextLayout> layout_;
		float offx_;

};




void D2DGrid::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();
	mat.Offset(rc_);
	auto rc = rc_.ZeroRect();

	cxt.DFillRect(rc, theWhite);


	int colc = 3;
	int colr = 10;


	float w = 100;
	float h = 19;

	FPointF pt(-2,0);

	D2DText t(parent_window_, w, L"1250000", D2DText::HTYP::RIGHT, 0x1);

	for(int ir = 0; ir < colr; ir++ )
	{
		mat.PushTransform();

	
		for(int ic = 0; ic < colc; ic++ )
		{
			FRectF rc(1,1,1+w,1+h);
			cxt.DFillRect(rc, theBlack);

			//cxt.DText(pt, L"スタートアップ", theWhite);

			t.DText(cxt, pt, cxt.white_ );
			
			mat.Offset(1+w,0);
		}

		mat.PopTransform();


		mat.Offset(0,1+h);
	}



	mat.PopTransform();
}
//void D2DGrid::Draw(D2DContext& cxt)
//{
//	D2DMatrix mat(*cxt);
//
//	mat_ = mat.PushTransform();
//	mat.Offset(rc_);
//	auto rc = rc_.ZeroRect();
//
//	cxt.DFillRect(rc, theBlack);
//
//
//	int colc = 3;
//	int colr = 3;
//
//
//	float w = (rc.Width() - (colc+1))/ (float)colc;
//	float h = (rc.Height() - (colr+1))/(float)colr;
//
//
//	for(int ir = 0; ir < colr; ir++ )
//	{
//		mat.PushTransform();
//		for(int ic = 0; ic < colc; ic++ )
//		{
//			FRectF rc(1,1,1+w,1+h);
//			cxt.DFillRect(rc, theWhite);
//			
//			mat.Offset(1+w,0);
//		}
//
//		mat.PopTransform();
//
//
//		mat.Offset(0,1+h);
//	}
//
//
//
//	mat.PopTransform();
//}