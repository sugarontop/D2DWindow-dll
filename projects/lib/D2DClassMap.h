#pragma once

#pragma once
#include "D2DApp.h"
#include "D2DWindowControl.h"
namespace V6 {

class D2DClassMap : public D2DControl
{

	public :
		D2DClassMap(){};

		virtual void Draw(D2DContext& cxt) override;
		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
		virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
		virtual FRectF GetRect() const override { return rc_; }
		virtual void SetRect(const FRectF& rc)  override { rc_ = rc; }

		void ListUp();


		FRectF rc_;
		std::vector<ControlMapItem> ar_;

};


};
