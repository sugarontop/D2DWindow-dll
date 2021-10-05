///////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2014, Yukihiro Furuya.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
///////////////////////////////////////////////////////////////////////

#pragma once

#include <d2d1.h>
#include <d2d1helper.h>


using namespace D2D1;

#pragma warning(disable: 4482)

#define ROUND(x) ((int)(x+0.5f))
#define D2RGBA(r,g,b,a) ColorF(r/255.0f, g/255.0f, b/255.0f, a/255.0f ) //  薄い(0) <- A <- 濃い(255)
#define D2RGB(r,g,b) ColorF((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, 1.0f )

#define D2DRGBADWORD(R,G,B,A) (DWORD)((R<<24)+(G<<16)+(B<<8)+A)
#define D2DRGBA(dw) ColorF(((dw&0xFF000000)>>24)/255.0f, ((dw&0x00FF0000)>>16)/255.0f, ((dw&0xFF00)>>8)/255.0f, (dw&0xFF)/255.0f )

#define D2WHITE ColorF(1.0f,1.0f,1.0f,1.0f)
#define D2LTGRAY D2RGB(225,225,225)
#define D2GRAY D2RGB(180.0f,180.0f,180.0f)


//
// D2DMisc.h helper library
// 2009.10.04

namespace V6
{

class FSizeF : public D2D1_SIZE_F
{
	public :
		FSizeF()
		{
			width = 0; height=0;
		}
		
		FSizeF( float cx, float cy )
		{
			width = cx; height=cy;
		}
		FSizeF( const SIZE& sz )
		{
			width = (FLOAT)sz.cx; height=(FLOAT)sz.cy;
		}
		FSizeF( const D2D1_SIZE_U& sz )
		{
			width = (FLOAT)sz.width; height=(FLOAT)sz.height;
		}	
		FSizeF( const D2D1_SIZE_F& sz )
		{
			width = (FLOAT)sz.width; height=(FLOAT)sz.height;
		}	
		
		FSizeF( LPARAM lParam )
		{
			width = (FLOAT)LOWORD(lParam); height = (FLOAT)HIWORD(lParam);
		}	
		SIZE GetSIZE()
		{
			SIZE sz;
			sz.cx = ROUND(width);
			sz.cy = ROUND(height);
			return sz;
		}
		void SetSize( float w, float h )
		{
			width = w; height = h;
		}
		
};
class FPointF : public D2D1_POINT_2F
{
	public :
		FPointF()
		{
			x = y = 0;
		}
		FPointF( float x1, float y1 )
		{
			x = x1; y = y1;
		}
		FPointF( const POINT& pt )
		{
			x = (FLOAT)pt.x; y = (FLOAT)pt.y;
		}
		FPointF( const D2D1_POINT_2F& pt )
		{
			x = pt.x; y = pt.y;
		}
		FPointF( LPARAM lParam )
		{			
			//x = static_cast<float>(GET_X_LPARAM(lParam));
			//y = static_cast<float>(GET_Y_LPARAM(lParam));

			x = LOWORD(lParam);
			y = HIWORD(lParam);
		}
		
		POINT GetPOINT()
		{
			POINT pt;
			pt.x = ROUND(x);
			pt.y = ROUND(y);
			return pt;
		}
		bool operator ==( const FPointF& pt )
		{
			return ( pt.x == x && pt.y== y );
		}

		FPointF& operator += ( const FSizeF& cs )
		{
			x += cs.width;
			y += cs.height;
			return *this;
		}
		FPointF& operator -= ( const FSizeF& cs )
		{
			x -= cs.width;
			y -= cs.height;
			return *this;
		}
		FPointF& operator -= ( const FPointF& p1 )
		{
			x -= p1.x;
			y -= p1.y;
			return *this;
		}
		void Offset( float offx, float offy )
		{
			x += offx;
			y += offy;
		}
};
class FRectF : public D2D1_RECT_F
{
	public :
		FRectF(){ left=top=bottom=right=0; }
		FRectF( const RECT& rc )
		{
			SetRect(rc);
		}
		FRectF( const D2D1_RECT_F& rc )
		{
			SetRect(rc);
		}

		FRectF ( const FPointF& pt, const FSizeF sz )
		{			
			left=pt.x; top=pt.y; right=pt.x + sz.width; bottom= pt.y + sz.height;
		}
		FRectF ( float l, float t, const FSizeF sz )
		{			
			left=l; top=t; right=l + sz.width; bottom= t + sz.height;
		}
		FRectF( const FPointF& p1, const FPointF& p2 )
		{
			SetRect(p1,p2);
		}
		FRectF( float _left, float _top, float _right, float _bottom )
		{
			left=_left;top=_top;right=_right;bottom=_bottom;
		}
		FRectF( const FRectF& rc )
		{
			SetRect(rc);
		}
		FRectF( float cx, float cy )
		{
			left=top=0;
			right=cx; bottom=cy;
		}
		FRectF(FRectF&& rc) noexcept
		{
			*this = std::move(rc);
		}
		FRectF& operator=(FRectF&& rc )  noexcept
		{
			if ( this != &rc )
			{
				left = rc.left;
				right = rc.right;
				top = rc.top;
				bottom = rc.bottom;
			}
			return *this;
		}
		FRectF& operator=(const FRectF& rc ) 
		{
			if ( this != &rc )
			{
				left = rc.left;
				right = rc.right;
				top = rc.top;
				bottom = rc.bottom;
			}
			return *this;
		}
		
		
		float Width()  const{ return right-left; }
		float Height()  const{ return bottom-top; }
		
		FSizeF Size() const{ return FSizeF( right-left, bottom-top ); }
		
		void SetRect( const RECT& rc )
		{
			left=(FLOAT)rc.left;
			right=(FLOAT)rc.right;
			bottom=(FLOAT)rc.bottom;
			top=(FLOAT)rc.top;		
		}
		RECT GetRECT() const
		{
			::RECT rc;
			rc.left = ROUND(left);
			rc.right = ROUND(right);
			rc.top  = ROUND(top);
			rc.bottom = ROUND(bottom);
			return rc;		
		}
		void SetRect( const D2D1_RECT_F& rc )
		{
			left=rc.left;
			right=rc.right;
			bottom=rc.bottom;
			top=rc.top;	
		}
		void SetRect( const FPointF& p1, const FPointF& p2 )
		{
			left   = min( p1.x, p2.x );
			right  = max( p1.x, p2.x );
			top    = min( p1.y, p2.y );
			bottom = max( p1.y, p2.y );	
		}
		void SetRect( const FPointF& p1, const FSizeF& sz )
		{
			left   = p1.x;
			right  = left + sz.width;
			top    = p1.y;
			bottom = top + sz.height;
		}
		void SetRect( float l, float t, const FSizeF& sz )
		{
			left   = l;
			right  = left + sz.width;
			top    = t;
			bottom = top + sz.height;
		}
		void SetRect( float l,float t,float r ,float b )
		{
			left = l;
			right = r;
			top = t;
			bottom = b;

		}
		void InflateRect( float cx, float cy )
		{
			left -= cx; top -= cy;
			right += cx; bottom += cy;
		}
		void SetEmpty()
		{
			left = right = top = bottom = 0;		
		}
		FRectF& Offset( float cx, float cy )
		{
			left += cx;
			right += cx;
			top += cy;
			bottom += cy;
			return *this;
		}
		FRectF OffsetRect( float cx, float cy )
		{
			FRectF rc(*this);
			rc.Offset(cx,cy);
			return rc;
		}
		void Offset( const FSizeF& sz )
		{
			Offset(sz.width, sz.height);
		}
		
		BOOL PtInRect( const FPointF& pt ) const
		{
			if ( pt.x < left || right < pt.x ) return FALSE;
			if ( pt.y < top || bottom < pt.y ) return FALSE;			
			
			return TRUE;
		}
		bool PtInRect2( const FPointF& pt ) const
		{
			if ( pt.x < left || right < pt.x ) return false;
			if ( pt.y < top || bottom < pt.y ) return false;			
			
			return true;
		}
		bool RectInRect( const FRectF& rc ) const
		{
			return PtInRect2( rc.LeftTop() ) && PtInRect2( rc.RightBottom() );
		}

		FRectF ZeroRect() const
		{
			return FRectF( 0,0,right-left,bottom-top );
		}
		FPointF LeftTop() const
		{
			return FPointF( left,top );
		}
		FPointF LeftBottom() const
		{
			return FPointF( left,bottom );
		}
		FPointF RightTop() const
		{
			return FPointF( right,top );
		}
		FPointF RightBottom() const
		{
			return FPointF( right,bottom );
		}
		FPointF CenterPt() const
		{
			return FPointF( (left+right)/2.0f, (top+bottom)/2.0f );
		}
		void SetSize( const FSizeF& sz )
		{
			right = left + sz.width;
			bottom = top + sz.height;
		}
		void SetSize( float cx, float cy )
		{
			right = left + cx;
			bottom = top + cy;
		}
		void SetPoint( const FPointF& pt )
		{
			SetPoint(pt.x, pt.y);
		}
		void SetPoint( float x, float y )
		{
			right += (x - left);
			bottom += (y - top);

			left = x;
			top = y;
		}
		void SetHeight( float cy )
		{
			bottom = top + cy;
		}
		void SetWidth( float cx )
		{
			right = left + cx;
		}
		FSizeF GetSize() const
		{
			return FSizeF( right-left, bottom-top );
		}

		void SetCenter( const FRectF& rc )
		{	
			FPointF pt = rc.CenterPt();
			FPointF xpt = CenterPt();

			Offset( pt.x - xpt.x, pt.y - xpt.y );
		}

		FRectF CenterRect() const
		{
			FPointF pt = CenterPt();
			FSizeF sz = Size();
			FRectF r;
			r.left = pt.x - sz.width/2;
			r.right= pt.x + sz.width/2;
			r.top = pt.y - sz.height/2;
			r.bottom= pt.y + sz.height/2;
			return r;

		}

		static FRectF InflateRect( const FRectF& rc, float cx, float cy )
		{
			FRectF rc1(rc);
			rc1.InflateRect( cx, cy );
			return rc1;
		}

		void MoveCenter( const FRectF& rc )
		{
			FPointF dpt = rc.CenterPt();
			FPointF spt = CenterPt();

			Offset( dpt.x-spt.x, dpt.y-spt.y );
		}
		FRectF Round()
		{
			FRectF rc;
			rc.left = (float)(int)(0.5f+left);
			rc.right = (float)(int)(0.5f+right);
			rc.top = (float)(int)(0.5f+top);
			rc.bottom = (float)(int)(0.5f+bottom);
			return rc;
		}

		D2D1_RECT_U RECT() const
		{
			D2D1_RECT_U r={(UINT32)left,(UINT32)top,(UINT32)right,(UINT32)bottom}; 
			return r;
		}

		bool ZeroPtInRect(const FPointF& pt ) const
		{
			// faster than "rc_.ZeroRect().PtInRect( pt ) ".

			return ( pt.x <= (right-left) && pt.y <= (bottom-top) && 0 <= pt.x && 0 <= pt.y );
		}
		void Clear()
		{
			left=0;
			right=0;
			bottom=0;
			top=0;
		}
};


// 線幅が１つに制限している以外は、HTMLのボックスモデルと同じ。
class FRectFBoxModel : public FRectF
{
	public :
		FRectFBoxModel():FRectF((float)0,(float)0,(float)0,(float)0)
		{
			BoderWidth_ = 0;			
		}
		FRectFBoxModel( float l,float t,float r,float b ):FRectF(l,t,r,b)
		{
			BoderWidth_ = 0;
		}
		FRectFBoxModel(const FRectF& rc)
		{
			BoderWidth_ = 0;
			SetFRectF(rc);
		}
		FRectFBoxModel(float l, float t, const FSizeF& sz) :FRectF(l, t, l+sz.width, t+sz.height)
		{
			BoderWidth_ = 0;
		}
		FRectFBoxModel(FRectFBoxModel&& rc)  noexcept
		{
			*this = std::move(rc);
		}
		FRectFBoxModel(const FRectFBoxModel& rc)
		{
			*this = rc;
		} 
		FRectFBoxModel& operator=(FRectFBoxModel&& rc )  noexcept
		{			
			left = rc.left;
			right = rc.right;
			top = rc.top;
			bottom = rc.bottom;
			Padding_ = rc.Padding_;
			Margin_ = rc.Margin_;
			BoderWidth_ = rc.BoderWidth_;
			
			return *this;
		}
		FRectFBoxModel& operator=(const FRectFBoxModel& rc )
		{
			if ( this != &rc )
			{
				left = rc.left;
				right = rc.right;
				top = rc.top;
				bottom = rc.bottom;
				Padding_ = rc.Padding_;
				Margin_ = rc.Margin_;
				BoderWidth_ = rc.BoderWidth_;
			}
			return *this;
		}

		void SetFRectF(const FRectF& rc )
		{
			left = rc.left;
			right = rc.right;
			top = rc.top;
			bottom = rc.bottom;


		}
		FRectFBoxModel& operator=( const FRectF& rc )
		{
			SetFRectF( rc );
			return *this;
		}

		struct dif 
		{ 
			dif(){ l=r=b=t=0.0; }
			float l,r,b,t; 
			void Set( float v ){ l=r=b=t=v; }
			void SetTB( float v ){ b=t=v; }
			void SetRL( float v ){ l=r=v; }
		};

		FRectF GetContentRect() const
		{
			FRectF rc;
			rc.left = left+Margin_.l+BoderWidth_+Padding_.l;
			rc.top = top+Margin_.t+BoderWidth_+Padding_.t;
			rc.right = right-Margin_.r-BoderWidth_-Padding_.r;
			rc.bottom = bottom-Margin_.b-BoderWidth_-Padding_.b;
			return rc;
		}
		FRectF GetPaddingRect() const
		{
			FRectF rc;
			rc.left = left+Margin_.l+BoderWidth_;
			rc.top = top+Margin_.t+BoderWidth_;
			rc.right = right-Margin_.r-BoderWidth_;
			rc.bottom = bottom-Margin_.b-BoderWidth_;
			return rc;
		}
		FRectF GetBorderRect() const
		{
			FRectF rc;
			rc.left = left+Margin_.l;
			rc.top = top+Margin_.t;
			rc.right = right-Margin_.r;
			rc.bottom = bottom-Margin_.b;
			return rc;
		}
		FRectF GetMarginRect() const
		{
			return *this;
		}

		FRectF GetContentRectZero() const
		{
			FRectF rc;
			rc.left = Margin_.l+BoderWidth_+Padding_.l;
			rc.top = Margin_.t+BoderWidth_+Padding_.t;
			rc.right = (right-left)-Margin_.r-BoderWidth_-Padding_.r;
			rc.bottom = (bottom-top)-Margin_.b-BoderWidth_-Padding_.b;
			return rc;
		}
		FRectF GetPaddingRectZero() const
		{
			FRectF rc;
			rc.left = Margin_.l+BoderWidth_;
			rc.top = Margin_.t+BoderWidth_;
			rc.right = (right-left)-Margin_.r-BoderWidth_;
			rc.bottom = (bottom-top)-Margin_.b-BoderWidth_;
			return rc;
		}
		FRectF GetBorderRectZero() const
		{
			FRectF rc;
			rc.left = Margin_.l;
			rc.top = Margin_.t;
			rc.right = (right-left)-Margin_.r;
			rc.bottom = (bottom-top)-Margin_.b;
			return rc;
		}
		FRectFBoxModel ZeroRect() const
		{
			FRectFBoxModel rc(*this);			
			rc.Offset( -left, -top );
			return rc;			
		}
		void SetContentSize( const FSizeF& sz )
		{					
			float width = Padding_.l + BoderWidth_ + Margin_.l + sz.width + Padding_.r + BoderWidth_ + Margin_.r;
			float height = Padding_.t + BoderWidth_ + Margin_.t + sz.height + Padding_.b + BoderWidth_ + Margin_.b;

						
			right = left + width;
			bottom = top + height;
		}
		
		dif Padding_;
		dif Margin_;
		float BoderWidth_;
};

class FPointU : public D2D1_POINT_2U
{
	public :
		FPointU(){x = 0;y = 0;}
		FPointU(UINT32 uX, UINT32 uY) {x = uX;y = uY;}
		bool operator ==(const FPointU& p) const { return (x ==p.x && y ==p.y); }
};
class FSizeU : public D2D1_SIZE_U
{
	public :
		FSizeU(){width= 0;height = 0;}
		FSizeU(UINT32 w, UINT32 h) {width= w;height = h;}
};
class FRectU : public D2D1_RECT_U
{
	public :
		FRectU(){left=top=right=bottom= 0;}
		FRectU(UINT32 l,UINT32 t, UINT32 r, UINT32 b) {left = l;top = t;right=r;bottom= b;}

		FSizeU Size() const { return FSizeU(right-left, bottom-top); }
		UINT32 Width() const { return (right-left); }
		UINT32 Height() const { return (bottom-top); }
};






#pragma once

	class D2DMat : public D2D1_MATRIX_3X2_F
	{
		public :
			D2DMat()
			{
				Reset();
			}
			D2DMat( const D2D1_MATRIX_3X2_F& m )
			{
				*this = m;
			}

			FPointF LPtoDP( const FPointF& ptlog ) const 
			{
				return LPtoDP( *this, ptlog );
			}
			FRectF LPtoDP( const FRectF& rc ) const 
			{
				FPointF p1 =  LPtoDP( *this, FPointF( rc.left, rc.top) );
				FPointF p2 =  LPtoDP( *this, FPointF( rc.right, rc.bottom) );
			
				return FRectF( p1.x, p1.y, p2.x, p2.y );
			}
			FSizeF LPtoDP( const FSizeF& sz ) const 
			{
				FPointF p1 =  LPtoDP( *this, FPointF( sz.width, sz.height) );				
				return FSizeF( p1.x - _31, p1.y - _32 );
			}

			FPointF DPtoLP( LPARAM lp ) const 
			{
				return DPtoLP( *this, FPointF(lp) );
			}
			FPointF DPtoLP( const FPointF& ptdev ) const 
			{
				return DPtoLP( *this, ptdev );
			}
			FRectF DPtoLP( const FRectF& rc ) const 
			{
				FPointF p1 =  DPtoLP( *this, FPointF( rc.left, rc.top) );
				FPointF p2 =  DPtoLP( *this, FPointF( rc.right, rc.bottom) );
			
				return FRectF( p1.x, p1.y, p2.x, p2.y );
			}
			FSizeF DPtoLP( const FSizeF& sz ) const 
			{
				FPointF p1 =  DPtoLP( *this, FPointF( sz.width, sz.height) );
				FPointF p2 =  DPtoLP( *this, FPointF( 0,0) );			
				return FSizeF( p1.x-p2.x, p1.y-p2.y );
			}

			static FPointF LPtoDP( const D2D1_MATRIX_3X2_F& mat, const FPointF& ptlog )
			{	
				Matrix3x2F m( mat._11, mat._12, mat._21, mat._22, mat._31, mat._32 );				
				return m.TransformPoint( ptlog ); // this is ptdev.
			}

			static FPointF DPtoLP(  const D2D1_MATRIX_3X2_F& mat, const FPointF& ptdev )
			{	
				Matrix3x2F m( mat._11, mat._12, mat._21, mat._22, mat._31, mat._32 );
				m.Invert();
				
				return m.TransformPoint( ptdev ); // this is ptlog.
			}
			static FRectF LPtoDP(  const D2D1_MATRIX_3X2_F& mat, const FRectF& rc )
			{
				FPointF p1 =  LPtoDP( mat, FPointF( rc.left, rc.top) );
				FPointF p2 =  LPtoDP( mat, FPointF( rc.right, rc.bottom) );
			
				return FRectF( p1.x, p1.y, p2.x, p2.y );
			}
			static FRectF DPtoLP( D2D1_MATRIX_3X2_F& mat, const FRectF& rc )
			{
				FPointF p1 =  DPtoLP( mat, FPointF( rc.left, rc.top) );
				FPointF p2 =  DPtoLP( mat, FPointF( rc.right, rc.bottom) );
			
				return FRectF( p1.x, p1.y, p2.x, p2.y );
			}
		
			void Offset( float cx, float cy )
			{
				FSizeF sz = LPtoDP( FSizeF(cx,cy));

				OffsetDev( sz.width, sz.height );
			}
			void OffsetDev( float cx, float cy )
			{
				_31 += cx;
				_32 += cy;			
			}
			D2D1_MATRIX_3X2_F CalcOffset( float cx, float cy )
			{
				D2DMat d(*this);
				d.Offset(cx,cy);
				return d;
			}
			D2DMat& operator = ( const D2D1_MATRIX_3X2_F& mat )
			{
				_11 = mat._11; _21 = mat._21;
				_12 = mat._12; _22 = mat._22;
				_31 = mat._31; _32 = mat._32;
				return *this;
			}

			FPointF Multi( const FPointF& pt )
			{
				FPointF rval;
				rval.x = _11 * pt.x + _21 * pt.y + _31;
				rval.y = _21 * pt.x + _22 * pt.y + _32;
				return rval;
			}
			
			void Rotate( float rad )
			{
				_11 = cos( rad );
				_12 = sin( rad );
				_21 = -_12;		//sin(rad);
				_22 = _11;		//cos(rad);
			}
			
			void Reset( int typ = 0 )
			{				
				_11 = _22 = 1;
				_21 = _12 = 0;
				_31 = _32 = 0;		

				if ( typ == 1 )
				{
					_11 = _22 = 0;					
				}
				else if ( typ == 2 )
				{
					// matが表示前にPtInRectでtrueをかえさないように、存在しないであろう座標を_31,_32へ設定する。
					// 表示時にmatは正確に設定される
					_11 = _22 = 0;	
					_31 = _32 = 100000;		
				}
			}
			D2D1_MATRIX_3X2_F Copy() const
			{
				D2D1_MATRIX_3X2_F r;
				r._11 = _11;
				r._22 = _22;
				r._12 = _12;
				r._21 = _21;
				r._31 = _31;
				r._32 = _32;
				return r;
			}

			static void Multi(D2D1_MATRIX_3X2_F &ou, const D2D1_MATRIX_3X2_F &a, const D2D1_MATRIX_3X2_F &b )
			{
				ou._11 = a._11 * b._11 + a._12 * b._21;
				ou._12 = a._11 * b._12 + a._12 * b._22;
				ou._21 = a._21 * b._11 + a._22 * b._21;
				ou._22 = a._21 * b._12 + a._22 * b._22;
				ou._31 = a._31 * b._11 + a._32 * b._21 + b._31;
				ou._32 = a._31 * b._12 + a._32 * b._22 + b._32;
			}
			
			
	};
	///////////////////////////////////////////////////////////////
	// D2DMat with stack

	class D2DMatrix : public D2DMat
	{
		public :
			
			D2DMatrix( const D2D1_MATRIX_3X2_F& m ):D2DMat(m),st_i_(0){}
			//D2DMatrix( D2DContext* cxt ):g_(cxt->target_),st_i_(0){}
			D2DMatrix( ID2D1RenderTarget* g ):g_(g),st_i_(0){ GetTransform(); }

			void Push()
			{
				_ASSERT( st_i_ < 4 );
				stack_[st_i_++] = *this;
			}

			void Pop()
			{
				_ASSERT( st_i_ > 0 );
				*this = stack_[--st_i_];
			}
			D2DMatrix& operator = ( const D2D1_MATRIX_3X2_F& mat )
			{
				_11 = mat._11; _21 = mat._21;
				_12 = mat._12; _22 = mat._22;
				_31 = mat._31; _32 = mat._32;
				return *this;
			}

			void PushTransformInCaputre(const D2DMat& mat )
			{				
				g_->SetTransform( &mat );
				PushTransform();				
			}
			D2DMatrix& GetTransform()
			{			
				g_->GetTransform( this );					
				return *this;
			}

			D2DMat& PushTransform()
			{			
				g_->GetTransform( this );
				Push();			
				return *this;
			}
			void PopTransform()
			{
				Pop();			
				g_->SetTransform( *this );			
			}
			D2DMat& Offset( const FRectF& rc )
			{
				return Offset( rc.left, rc.top );
			}
			D2DMat& Offset( const FPointF& pt )
			{
				return Offset( pt.x, pt.y );
			}
			D2DMat& Offset( float cx1, float cy1 )
			{
				GetTransform();
				D2DMat::Offset(cx1,cy1);
				g_->SetTransform( *this );
				return *this;
			}
			D2DMatrix& Scale( float scx, float scy )
			{
				_11 = _11 * scx;
				_22 = _22 * scy;
				g_->SetTransform( *this );
				return *this;

			}
			void SetTransform()
			{
				g_->SetTransform( *this );
			}
			void ReplaceTransform(const D2D1_MATRIX_3X2_F& mat)
			{
				this->operator=( mat );
				g_->SetTransform( *this );
			}
			void NewTransform(const D2D1_MATRIX_3X2_F& mat)
			{
				this->operator=( mat );
				g_->SetTransform( *this );
			}
			void Multi( const D2D1_MATRIX_3X2_F &a )
			{
				D2DMat::Multi( *this, *this, a );
				g_->SetTransform( *this );
			}

		protected :
			int st_i_;
			D2D1_MATRIX_3X2_F stack_[4]; // 4個あれば十分だと思う。
			ID2D1RenderTarget* g_;
	};

	struct MouseParam
	{
		FPointF pt;
		FPointF ptprv;
		void* target;

		WORD fwKeys;
		INT32 zDelta;

		D2D1_MATRIX_3X2_F mat;
		
	};


 #define FR(l,t,r,b) V6::FRectF(l,t,r,b)
 #define FRB(l,t,cx,cy) V6::FRectF(l,t,l+cx,t+cy)

};

