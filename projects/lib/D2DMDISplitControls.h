#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
#include "D2DScrollbar.h"
namespace V6 {

	class D2DMDISplitFrame : public D2DControls
	{
		friend class D2DMDISplitChild;
		public:
			D2DMDISplitFrame();
			
			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return TYP_MDISPLIT_CONTROLS; }
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;

			void CreateChildView(int typ, float left_width=300.0f);


			void SetTopMDIChild(short idx);

	
		protected:
			std::map<short, FRectF> prvRect_;
			short largest_idx_, active_idx_;
			UINT_PTR top_;
			int child_typ_;

			float split_line_x_;
	};

	class D2DMDISplitChild : public D2DControls
	{
	public:
			D2DMDISplitChild();
			
			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return TYP_MDISPLIT_CONTROLS; }
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
	};




};