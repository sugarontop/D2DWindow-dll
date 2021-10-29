#pragma once

#include "D2DApp.h"
#include "D2DWindowControl.h"

namespace V6
{

class D2DTextbox;
class D2DStatic : public D2DControl
{
	public :
		D2DStatic();

		void SetText( std::wstring str );
		std::wstring GetText() const { return str_;}
		void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
	public :
		virtual void Draw(D2DContext& cxt) override;
		virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
		virtual const FRectF& GetRect() const override { return rc_; }
		virtual void SetRect(const FRectF& rc)  override { rc_ = rc; }
	private :
		ComPTR<IDWriteTextLayout> text_;		
		std::wstring str_;
		FRectF rc_;
		bool editable_;
		D2DTextbox* editor_;

};

};
