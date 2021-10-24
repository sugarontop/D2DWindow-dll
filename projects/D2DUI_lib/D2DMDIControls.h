#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
namespace V6 {



	class D2DMDIFrame : public D2DControls
	{
		public:
			D2DMDIFrame();
			virtual ~D2DMDIFrame();

			//virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return TYP_CONTROLS; }



	};


	class D2DMDIChild : public D2DControls
	{
		friend class D2DMDIFrame;
		public:
			D2DMDIChild();
			virtual ~D2DMDIChild();

			//virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return TYP_CONTROLS; }




	};



};