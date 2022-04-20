#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
#include "D2DScrollbar.h"
#include "D2DColor.h"

#define DEFAULT_TAB_COLOR	D2RGB(110,110,110)
#define DEFAULT_TAB_ACTIVE_COLOR	D2RGB(110,110,210)

namespace V6 {

	class D2DControls_with_Scrollbar : public D2DControls
	{
		public:
			D2DControls_with_Scrollbar();
			
			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return TYP_CONTROLS; }
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
			virtual std::wstring GetTreeTyp(USHORT* typ) override;

			D2DControls* GetMainControls();
			void SetViewMaxSize(FSizeF sz);
		protected :
			float vscroll_x_,hscroll_x_;
			std::shared_ptr<D2DScrollbar> scv_, sch_;
			D2DColor backcolor_;

	}; 



	class D2DControls_with_Move : public D2DControls
	{
		public :
			D2DControls_with_Move():prv_controls_(0),target_(0){}

			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return TYP_CONTROLS; }
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;


			D2DControls* prv_controls_;
			D2DControl* target_;
	};


};
