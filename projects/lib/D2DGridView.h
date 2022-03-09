#pragma once

#include "D2DApp.h"
#include "D2DWindowControl.h"
#include "D2DColor.h"
namespace V6
{
	class D2DGridView : public D2DControl
	{
		public :
			D2DGridView(){};
			virtual ~D2DGridView();

			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
			virtual FRectF GetRect() const { return rc_; }
			virtual void SetRect(const FRectF& rc){ rc_ = rc; }
			virtual void Draw(D2DContext& cxt);
			virtual FRectF& GetRectSmooth(){ return rc_; };
			void Clear();
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
		protected :
			FRectF rc_;


			typedef IDWriteTextLayout* PIDWriteTextLayout;
			struct Row
			{				
				PIDWriteTextLayout* value;
				WORD colcnt;
			};

			std::vector<Row> Items_;

			D2DColor fore_,back_;


	};
};