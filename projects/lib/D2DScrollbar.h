
#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
namespace V6 {
class D2DScrollbar : public D2DControl
{
public :
	D2DScrollbar();	



	virtual void Draw(D2DContext& cxt) override;
	virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
	virtual FRectF GetRect() const override { return FRectF(0,0,sz_); }
	virtual void SetRect(const FRectF& rc)  override { sz_ = rc.Size(); }
	void SetSize(const FSizeF& sz);
	FSizeF GetSize() const;

	float Thumheight() const;
	float LogicalOffset() ;
	void SetMaxSize(float h){ max_size_=h; }
	void Draw2(D2DContext& cxt);
	void Offset( float off );
protected :

	bool bVertical_;
	FSizeF sz_;

	float offset_; // device
	float max_size_;// device
	float view_size_;// device
	float thumb_size_;
	float other_scrollbar_size_; // 


};

};
