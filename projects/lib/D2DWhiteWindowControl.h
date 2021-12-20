#pragma once
#include "D2DWindowControl.h"
namespace V6 {


	class MovableWithMouse
	{
		public :
			MovableWithMouse(){ mstat_ = STAT::NORMAL; }

			const float TitleBarHeight = 30.0f;

			bool IsMilitarybase( int mode , FRectF rc, FPointF pt )
			{
				if ( mode == 0 )
					return rc.PtInRect(pt);
				else if ( mode == 1 )
				{
					rc.SetHeight(TitleBarHeight);
					return rc.PtInRect(pt);
				}

				return false;
			}
			virtual LRESULT WndProc( AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
		protected :
			enum STAT { NORMAL, MOVING };
			STAT mstat_;
	};



	class D2DWhiteWindowControls : public D2DControls, public MovableWithMouse
	{
		public :
		D2DWhiteWindowControls();

		virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
		virtual void Draw(D2DContext& cxt) override;
		virtual std::wstring GetTreeTyp(USHORT* typ) override;
		
	};

	



};