#pragma once
#include "D2DApp.h"
#include "D2DWindowControl.h"
#include "D2DColor.h"
namespace V6
{
	class D2DXXXControls: public D2DControls
	{
		public :
			D2DXXXControls(){};

			virtual void Draw(D2DContext& cxt)  override;
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
			virtual FRectF GetRect() const	 override { return rc_; }
			virtual void SetRect(const FRectF& rc) override { rc_ = rc; }
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;

		protected :
			FRectF rc_;
			int mode_;
			D2DColor back_color_;
	};


	class D2DSimpleControls : public D2DControls
	{
		public :
			D2DSimpleControls(){};

			virtual void Draw(D2DContext& cxt)  override;
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
			virtual FRectF GetRect() const	 override { return rc_; }
			virtual void SetRect(const FRectF& rc) override { rc_ = rc; }
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;

		protected :
			FRectF rc_;
			D2DColor back_color_;



	};


};