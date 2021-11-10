#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
#include "D2DScrollbar.h"
namespace V6 {

	class D2DMDIChild;

	class D2DMDISplitFrame : public D2DControls
	{
		friend class D2DMDISplitChild;
		public:
			D2DMDISplitFrame();
			
			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return TYP_MDISPLIT_CONTROLS; }
			virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;

			//std::shared_ptr<D2DControls> Add(int typ, float width, DWORD stat, LPCWSTR name, int id);

			void CreateChildView(int typ);


			void SetTopMDIChild(short idx);

			D2DMDISplitChild* GetChild(short idx) const;
		
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
			virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
		
		protected :
			float vscroll_x_,hscroll_x_;
			std::shared_ptr<D2DScrollbar> scv_, sch_;

	};

	class D2DMDISplitChildEx : public D2DControls
	{
	public:
			D2DMDISplitChildEx();
			
			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return TYP_MDISPLIT_CONTROLS; }
			virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
	};


	class D2DControls_with_Scrollbar : public D2DControls
	{
		public:
			D2DControls_with_Scrollbar();
			
			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return TYP_CONTROLS; }
			virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;

		protected :
			float vscroll_x_,hscroll_x_;
			std::shared_ptr<D2DScrollbar> scv_, sch_;

	};


};