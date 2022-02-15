#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
#include "D2DColor.h"
namespace V6 {
class D2DLogin : public D2DControls
{
	public :
		D2DLogin();	

		virtual void Draw(D2DContext& cxt) override;
		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
		virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
		virtual FRectF GetRect() const override { return rc_; }
		virtual void SetRect(const FRectF& rc)  override { rc_ = rc; }


		std::function<DWORD(void*, void*)> on_try_login_;
	protected :
		FRectF rc_;
		
		UIHandle usercd_,message_;
		UIHandle password_;
		D2DColor clr_[3];
};


};