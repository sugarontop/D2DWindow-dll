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

	class yahoo_chart;
	class yahoo_table;
	class D2DControls_with_Scrollbar;


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

			std::vector<Rousoku> y1_;
			std::vector<std::string> dates_;
			yahoo_chart* chart_;
			yahoo_table* table_;
			D2DControls_with_Scrollbar* sc_control_;


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

			static ULONG Period(int yyyy, int mm, int dd );
			void SetParentRect(FRectF* prc);

			FRectF rc_;
			yahoo_finance* finance_;
			FRectF* prc_;
			FPointF mouse_pt_;

	};

	class yahoo_table : public D2DControl
	{
		public :
			yahoo_table(){};

			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return 0; }
			virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;

			virtual const FRectF& GetRect() const { return rc_; }
			virtual void SetRect(const FRectF& rc) { rc_ = rc; }

			void Update(float cx);
			FRectF rc_;
			yahoo_finance* finance_;

	};

};

