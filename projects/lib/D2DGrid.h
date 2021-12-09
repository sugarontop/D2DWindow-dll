#pragma once
#include "D2DWindowControl.h"
namespace V6 {

	class D2DGrid : public D2DControls
	{
	public :
		D2DGrid();

		virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
		virtual void Draw(D2DContext& cxt) override;

	private :
		FRectF rc_;
	};





};