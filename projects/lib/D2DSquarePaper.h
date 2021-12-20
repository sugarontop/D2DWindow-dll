#pragma once

#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
namespace V6 {


// for DEBUG class

class D2DSquarePaper : public D2DControls
{
	public :
		
		D2DSquarePaper():backcolor_(D2RGB(105,200,100)),forecolor_(D2RGB(255,255,255)){}

		virtual void Draw(D2DContext& cxt) override;
		virtual int GetTypeid() const override { return TYP_CONTROLS; }
		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
		virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
		virtual std::wstring GetTreeTyp(USHORT* typ) override;

		std::function<LRESULT(LPVOID,AppBase&,UINT,WPARAM,LPARAM)> import_function_;
	private :
		float pitch_;
		float scale_;
		ColorF backcolor_,forecolor_;

		


};



};