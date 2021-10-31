#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
namespace V6 {

	class D2DMDIChild;

	class D2DMDISplitFrame : public D2DControls
	{
		friend class D2DMDISplitChild;
		public:
			D2DMDISplitFrame();
			virtual ~D2DMDISplitFrame();

			
			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return TYP_MDISPLIT_CONTROLS; }
			virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;

			std::shared_ptr<D2DMDISplitChild> Add(int typ, DWORD stat, LPCWSTR name, int id);

			void SetTopMDIChild(short idx);

			D2DMDISplitChild* GetChild(short idx) const;
		
		protected:
			std::map<short, FRectF> prvRect_;
			short largest_idx_, active_idx_;
			UINT_PTR top_;

			float split_line_x_;
	};

	#define BARW 10.0f

	class D2DScrollbar : public D2DControl
	{
		public :
			D2DScrollbar();	

			
		
			virtual void Draw(D2DContext& cxt) override;
			virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
			virtual const FRectF& GetRect() const override { return FRectF(0,0,sz_); }
			virtual void SetRect(const FRectF& rc)  override { sz_ = rc.Size(); }

			float Thumheight() const;
			float LogicalOffset() ;

			bool bVertical_;
			FSizeF sz_;

			float offset_; // device
			float max_size_;// device
			float view_size_;// device
			float thumb_size_;

	};

	class D2DMDISplitChild : public D2DControls
	{
		friend class D2DMDISplitFrame;
		public:
			D2DMDISplitChild();
			D2DMDISplitChild(short idx);
			virtual ~D2DMDISplitChild();

			
			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return TYP_MDISPLIT_CONTROLS; }
			virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;

			void SetViewSize(FSizeF sz){ sz_ = sz;}
			

			short GetIdx() const { return idx_; }
		protected :
			
		protected :
			short idx_;
			FSizeF sz_;

			std::shared_ptr<D2DScrollbar> scv_;

	};



};