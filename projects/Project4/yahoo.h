#pragma once


#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
#include "inet.h"
namespace V6 {



	class yahoo_finance : public D2DControls
	{
		public :
			
			yahoo_finance():info_(nullptr){};
			virtual ~yahoo_finance();

			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return 0; }
			virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
	

			InternetInfo* info_;
			FRectF rc_;

			std::vector<float> y1_;
			std::vector<std::string> dates_;


			bool ConvCsv(IStream* ism);
	};




};

