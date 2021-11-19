#pragma once

#include "D2DApp.h"
#include "D2DWindowControl.h"

namespace V6
{
	class D2DButton: public D2DControl
	{
		public :
			D2DButton();
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);

			void SetText(LPCWSTR str);
		public:
			// D2DCaptureObject interface
			virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual int GetTypeid() const override{ return TYP_BUTTON; }
		public:
			// D2DControl
			virtual const FRectF& GetRect() const override { return rc_; }
			virtual void SetRect(const FRectF& rc)  override { rc_ = rc; }
			virtual void Draw(D2DContext& cxt)  override;

		protected :
			FRectF rc_;
			int part_stat_;
			std::wstring text_;

			ComPTR<IDWriteTextLayout> textlayout_;
			FPointF ptText_;
		

	};




	class InnerMessageBox : public D2DControl
	{
	public :
		InnerMessageBox(){}

		virtual const FRectF& GetRect() const { return rc_; }
		virtual void SetRect(const FRectF& rc) {rc_ = rc; }

		void ModalShow(LPCWSTR text, LPCWSTR title);

		virtual void Draw(D2DContext& cxt) override;		

		virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;


		FRectF rc_;
		wstring msg_,title_;
		int mode_;

	};

	class InnerFloatingMenu : public D2DControl
	{
	public :
		InnerFloatingMenu(){}

		virtual const FRectF& GetRect() const { return rc_; }
		virtual void SetRect(const FRectF& rc) {rc_ = rc; }

		void ModalShow(LPVOID sender, std::vector<MenuItem>& ar);

		virtual void Draw(D2DContext& cxt) override;		

		virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;


		std::vector<MenuItem> items_;

	private:
		FRectF rc_;
		int floating_idx_;
		std::vector<FRectF> itemrc_;
		LPVOID sender_;

	};
};
