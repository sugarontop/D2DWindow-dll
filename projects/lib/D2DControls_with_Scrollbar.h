#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
#include "D2DScrollbar.h"
namespace V6 {

	class D2DControls_with_Scrollbar : public D2DControls
	{
		public:
			D2DControls_with_Scrollbar();
			
			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return TYP_CONTROLS; }
			virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;

			D2DControls* GetMainControls();

		protected :
			float vscroll_x_,hscroll_x_;
			std::shared_ptr<D2DScrollbar> scv_, sch_;

	}; 

};
