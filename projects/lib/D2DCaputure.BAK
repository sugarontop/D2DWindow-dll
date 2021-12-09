#pragma once

#include "AppBase.h"

#define STAT_VISIBLE		0x1
#define STAT_MODAL			0x2
#define STAT_CAPTURED  0x4
#define STAT_DEAD			0x8
#define STAT_ENABLE			0x16



#define BITFLG(x)	((stat_&x)==x)
#define BITFLG2(a, x) ((a&x)==x)
namespace V6
{
	class D2DContext;
	class D2DCaptureObject
	{
	protected :
		DWORD stat_;
		D2DCaptureObject* parent_;
	public :
		D2DCaptureObject():stat_(STAT_VISIBLE), parent_(0){}

		virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) = 0;
		virtual D2DCaptureObject* GetParent(){ return parent_; }
		virtual bool OnChangeFocus(bool bActive, D2DCaptureObject* pnew) { return false; }


		virtual void OnGraphicsResources(D2DContext& cxt, bool bDestroyOrCreate){};
		virtual bool IsDialog() const { return BITFLG(STAT_MODAL); }
		
		virtual void DestroyEx(){Destroy(); }
		 

		DWORD GetStat() const { return stat_; }
		void Destroy(){ stat_ = STAT_DEAD; }
		void Enable(bool bEnable=true) { stat_ = (bEnable ? (STAT_ENABLE|stat_) : (stat_&~STAT_ENABLE)); }
	};


};
