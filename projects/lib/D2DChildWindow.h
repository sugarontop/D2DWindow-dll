#pragma once
#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
namespace V6 {
class D2DChildWidow : public D2DControls
{
public :
	D2DChildWidow(){};	

	virtual void Draw(D2DContext& cxt) override;
	virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
	
	//void OpenCloseBar(bool bOpen);
protected :
	void DrawTitlebar(D2DContext& cxt);
	LRESULT TitleBarProc(AppBase& b, UINT message, MouseParam& pm);
	
	int mode_;
	USHORT title_bar_mode_;
	
};

};
