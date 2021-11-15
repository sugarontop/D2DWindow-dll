
#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
namespace V6 {
class D2DTabControls : public D2DControls
{
	public :
		D2DTabControls(){};	



		virtual void Draw(D2DContext& cxt) override;
		virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
		virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;

		float DrawTab(D2DContext& cxt, USHORT tabidx);
		D2DControl* GetControlFromIdx(USHORT idx);
		void SizeFix(){ size_fix_ = true;};
	protected :
		
		std::vector<FRectF> tabrects_;
		USHORT tab_idx_;
		bool size_fix_;
		


};

};
