#pragma once


#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
#include "inet.h"
namespace V6 {

	struct Rousoku
	{
		float ymax,ymin;
		float ystart,yend;
	};


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
			//FRectF rc_;

			std::vector<Rousoku> y1_;
			std::vector<std::string> dates_;


			bool ConvCsv(IStream* ism);
			void StartDownload();
			void InetComplete(InternetInfo* );
	};

	class yahoo_chart : public D2DControls
	{
		public :
			yahoo_chart();

			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return 0; }
			virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
			virtual const FRectF& GetRect() const { return rc_; }
			virtual void SetRect(const FRectF& rc) { rc_ = rc; }

			FRectF rc_;
			yahoo_finance* finance_;

	};


};

