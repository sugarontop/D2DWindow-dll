#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
#include "D2DColor.h"
#include "inet.h"
#include "yahoo.h"
namespace V6 
{
	class TDBase : public D2DControls
	{
		public :
			TDBase();
			virtual void CreateControl(D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void Draw(D2DContext& cxt) override;


			D2DControl* td;
			D2DControl* top;
			D2DControl* txt;
			D2DControl* tr;
			D2DControl* ls;
			D2DControl* chart;

	};

		class TDChart : public D2DControls
		{
			public :
				TDChart();
				virtual ~TDChart();
				virtual void CreateControl(D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
				virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
				virtual void Draw(D2DContext& cxt) override;


				void SetInfo(InternetInfo* info);
				void GenerateGraph();
				void Clear();
				void Error(std::wstring err);

			protected :
				InternetInfo* info_;
				std::wstring memo_, error_;

				std::vector<Rousoku> rousoku_ar_;
		};

		class TDChartButtons: public D2DControls
		{
			public :
				TDChartButtons();
				virtual void CreateControl( D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id , D2DColor clr);
				virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
				virtual void Draw(D2DContext& cxt) override;
				void OnClick(int mode);
				

				D2DColor bkclr_;
				std::vector<FRectF> btnar_;
				std::wstring key_;
				int running_mode_;
		};

		class TDList : public D2DControls
		{
			public :
				TDList();
				virtual void CreateControl(D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
				virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
				virtual void Draw(D2DContext& cxt) override;
		};







	D2DControls* CreateStockChart(D2DControls* ctrl,  FSizeF size, LPCWSTR key );







};
