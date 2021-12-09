#pragma once

#include "D2DWindowControl.h"
#include "D2DInterface.h"
namespace V6 {

class D2DClientControls : public D2DControls
{
public:	

	typedef std::function<void(LPVOID, D2DContext& cxt)> DrawFunc;
	typedef std::function<LRESULT(LPVOID,AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)> ProcFunc;

	D2DClientControls(LPVOID captureobj,  DelegateDrawFunc func1, DelegateProcFunc func2);
	virtual ~D2DClientControls();

	virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
	virtual FRectF GetRect() const	 override { return rc_; }
	virtual void SetRect(const FRectF& rc) override { rc_ = rc; }
	virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual int GetTypeid() const override{ return TYP_WHITE_CONTROL; }
protected :
	DrawFunc DrawFunc_;
	ProcFunc ProcFunc_;
	FRectF rc_;

	LPVOID captureobj_;

	virtual void Draw(D2DContext& cxt) override;

};

};