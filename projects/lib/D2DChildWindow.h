#pragma once
#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
namespace V6 {

class D2DChildWidow;

class MinimumWindow
{
	public :
		MinimumWindow( D2DChildWidow* parent, FRectF rc ):parent_(parent),rc_(rc){};
		virtual void Draw(D2DContext& cxt);
		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);

	protected :
		D2DChildWidow* parent_;
		FRectF rc_;
		D2DMat mat_;
};


class D2DChildWidow : public D2DControls
{
	friend class MinimumWindow;
public :
	D2DChildWidow(){};	

	virtual void Draw(D2DContext& cxt) override;
	virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
	
	//void OpenCloseBar(bool bOpen);
	virtual std::wstring GetTreeTyp(USHORT* typ) override;

	D2DMat GetMat() const { return this->mat_; }
protected :
	void DrawTitlebar(D2DContext& cxt);
	LRESULT TitleBarProc(AppBase& b, UINT message, MouseParam& pm);
	void DrawMinimum(D2DContext& cxt, const FRectF& rc);
	
	int mode_;
	USHORT title_bar_mode_, window_mode_;
	std::shared_ptr<MinimumWindow> mini_window_;
	
};





};
