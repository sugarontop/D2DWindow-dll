#pragma once

#include "D2DWindowControl.h"
#include "D2DScrollbar.h"

namespace V6 {

class D2DWhiteWindow : public D2DControls
{
public:
	D2DWhiteWindow();

	virtual void Draw(D2DContext& cxt) override;
	virtual int GetTypeid() const override { return TYP_MDISPLIT_CONTROLS; }
	virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;

protected :
	float vscroll_x_,hscroll_x_;
	std::shared_ptr<D2DScrollbar> scv_, sch_;

};

};