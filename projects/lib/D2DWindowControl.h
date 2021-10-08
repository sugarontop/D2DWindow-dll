#pragma once

#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindow.h"
namespace V6 {


class D2DControls;

class D2DControl : public D2DCaptureObject
{
public:
	D2DControl();
	virtual ~D2DControl();

	virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);


	int GetStat()  const { return stat_; }
	void SetStat(int s) { stat_ = s; }

	virtual const FRectF& GetRect() const  = 0;
	virtual void SetRect(const FRectF& rc) = 0;
	virtual void Draw(D2DContext& cxt) = 0;



	//virtual void UpdateScrollbar(D2DScrollbar* bar) {};
	//bool IsThroughMessage(UINT message);

	//virtual void UpdateStatus(D2DControls* p) {}
	//virtual D2DControl* UnPack() { return this; }
	//virtual void StatActive(bool bActive);
	//virtual bool IsAutoResize() { return auto_resize_; }
	//virtual void DestroyControl();


	void Enable(bool bEnable);
	void Visible(bool bVisible);
	bool IsEnable()  const { return ((stat_&STAT_ENABLE) > 0 ); }
	bool IsVisible()  const { return ((stat_ & STAT_VISIBLE) > 0); }
	
	virtual void OnHostage(D2DControls* p, bool block) {}
	virtual void DestroyControl();

	D2DMat Matrix() const { return mat_; }
	wstring GetName() const { return name_; }
	int GetID() const { return id_; }
	void SetName(LPCWSTR nm) { name_ = nm; }

	D2DWindow* GetParent() const { return parent_window_; }

	
protected:
	D2DWindow* parent_window_;
	D2DControls* parent_control_;
	void* target_;

protected:
	virtual void OnCreate() {}

	void InnerCreateWindow(D2DWindow* parent, D2DControls* pacontrol, int stat, LPCWSTR name, int controlid);

protected:
	D2DMat mat_;
	DWORD stat_;
	wstring name_;
	int id_;
};

class D2DControls : public D2DControl
{
	friend class D2DControl;
public:
	D2DControls();
	virtual ~D2DControls();
	virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
	virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual void Draw(D2DContext& cxt) override;
	void ForceWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int GetTypeid() const override{ return TYP_CONTROLS; }

	void Add(std::shared_ptr<D2DControl> p);
	D2DControl* GetControl( std::wstring name );
	D2DControl* GetControlFromID( int id ) const;
protected :
	virtual HRESULT DefWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
	
	void InnerDraw(D2DContext& cxt);

	
	std::shared_ptr<D2DControl> Detach(D2DControl* target);

	virtual const FRectF& GetRect() const override { return rc_; }
	virtual void SetRect(const FRectF& rc)  override { rc_ = rc; }

	

protected :
	static std::map<std::wstring, D2DControl*> control_map_;
	std::vector<std::shared_ptr<D2DControl>> controls_;
	FRectF rc_;

};
};