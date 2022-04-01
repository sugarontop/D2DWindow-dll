#pragma once
#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
#include "D2DColor.h"
namespace V6 {

class D2DChildWindow;

class MinimumWindow
{
	public :
		MinimumWindow( D2DChildWindow* parent, FRectF rc ):parent_(parent),rc_(rc){};
		virtual void Draw(D2DContext& cxt);
		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);

	protected :
		D2DChildWindow* parent_;
		FRectF rc_;
		D2DMat mat_;
};



class D2DChildWindow : public D2DControls
{
	friend class MinimumWindow;
public :
	D2DChildWindow();	

	enum CLR { TITLEBAR=0, BACKCOLOR=1 };

	virtual void Draw(D2DContext& cxt) override;
	virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;

	virtual std::wstring GetTreeTyp(USHORT* typ) override;

	virtual void Add(std::shared_ptr<D2DControl> p) override;

	D2DMat GetMat() const { return this->mat_; }
protected :
	void DrawTitlebar(D2DContext& cxt);
	LRESULT TitleBarProc(AppBase& b, UINT message, MouseParam& pm);
	void DrawMinimum(D2DContext& cxt, const FRectF& rc);
	
	int mode_;
	USHORT title_bar_mode_, window_mode_;
	std::shared_ptr<MinimumWindow> mini_window_;
	std::wstring title_;
	D2DColor colors_[2];
	
};





};
