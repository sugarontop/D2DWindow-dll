#pragma once


#include "D2DApp.h"
#include "D2DWindowControl.h"


namespace V6
{
	class D2DListboxItemBase
	{
		public :
			D2DListboxItemBase() : idx_(-1){}
			D2DListboxItemBase(int idx): idx_(idx){}
			
			virtual void Draw(D2DContext& cxt, float width, float height) = 0;

		protected :
			int idx_;
	};

	class D2DListboxItemString : public D2DListboxItemBase
	{
		public :
			D2DListboxItemString(){}
			D2DListboxItemString(int idx, std::wstring title):D2DListboxItemBase(idx), title_(title){}
			virtual void Draw(D2DContext& cxt, float width, float height) override;

		protected :			
			std::wstring title_;

	};

	class D2DSimpleListbox : public D2DControl
	{
		friend class D2DDropdownListbox;
		public:
			D2DSimpleListbox();

			virtual const FRectF& GetRect() const { return rc_; }
			virtual void SetRect(const FRectF& rc) { rc_ = rc; }
			virtual void Draw(D2DContext& cxt);

			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
			virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;

			int GetSelectedIdx() const { return selected_idx_; }
	
	protected:

			bool OnEscape();
			void OnClick();

		
			bool sc_MouseMove(FPointF& pt);
			float sc_barThumbHeight();
			float sc_barTotalHeight();
			float sc_dataHeight();

		protected :
			int selected_idx_;
			FRectF rc_;
			int mouse_stat_;
			float offbar_y_;
			int float_idx_;
			int scstat_;

			float scbarThumbHeight_;
			float scdata_;
			float scbai_;

			std::vector< std::shared_ptr<D2DListboxItemBase> > items_;
	};

};