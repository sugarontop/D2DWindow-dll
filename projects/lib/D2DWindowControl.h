#pragma once

#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindow.h"
namespace V6 {

#define BARW 10.0f

class D2DControl;
class D2DControls;



class D2DControl : public D2DCaptureObject
{
public:
	D2DControl(){};
	virtual ~D2DControl(){};

	virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);


	int GetStat()  const { return stat_; }
	void SetStat(int s) { stat_ = s; }

	virtual FRectF GetRect() const  = 0;
	virtual void SetRect(const FRectF& rc) = 0;
	virtual void Draw(D2DContext& cxt) = 0;
	virtual FRectF& GetRectSmooth() =0;
	virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) = 0;


	virtual std::wstring GetTreeTyp(USHORT* typ){ *typ=1; return L"D2DControl";}
	virtual void ListUp(std::vector<ControlMapItem>& ar, int* row, int* col);
	virtual void ResourceUpdate(bool bCreate, D2DContext& cxt){}

	void Enable(bool bEnable);
	void Visible(bool bVisible);
	bool IsEnable()  const { return ((stat_&STAT_ENABLE) > 0 ); }
	bool IsVisible()  const { return ((stat_&STAT_VISIBLE) > 0); }
	
	virtual void OnHostage(D2DControls* p, bool block) {}
	virtual void DestroyControl();
	

	D2DMat Matrix() const { return mat_; }
	wstring GetName() const { return name_; }
	int GetID() const { return id_; }
	void SetName(LPCWSTR nm) { name_ = nm; }

	D2DWindow* GetParent() const { return parent_window_; }
	D2DControls* GetParentControls() const { return parent_control_; }
	//virtual LRESULT DefWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
	void SetNewParent(D2DControls* newparent);
	std::wstring GetLocalName() const;
protected:
	D2DWindow* parent_window_;
	D2DControls* parent_control_;
	void* target_;

protected:
	virtual void OnCreate() {}

	void InnerCreateWindow(D2DWindow* parent, D2DControls* pacontrol, int stat, LPCWSTR name, int controlid);

protected:
	D2DMat mat_;
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
	virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual LRESULT SendMesage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void Draw(D2DContext& cxt) override;
	void ForceWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int GetTypeid() const override{ return TYP_CONTROLS; }
	D2DControl* SetFirstControl(D2DControl* p, bool blast=false);
	virtual void ResourceUpdate(bool bCreate, D2DContext& cxt);
	virtual std::wstring GetTreeTyp(USHORT* typ){ *typ=10; return L"D2DControls";}

	void Add(std::shared_ptr<D2DControl> p);
	D2DControl* GetControl( std::wstring name );
	D2DControl* GetControlFromID( int id ) const;
	virtual FRectF GetRect() const override { return rc_; }
	virtual FRectF& GetRectSmooth() override { return rc_; }
	virtual void SetRect(const FRectF& rc)  override { rc_ = rc; }

	std::shared_ptr<D2DControl> GetItem(UINT idx){ return controls_[idx]; }
	UINT ChildCount() const { return controls_.size(); }
	virtual void ListUp(std::vector<ControlMapItem>& ar, int* row, int* col) override;
	LRESULT InnerWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);

	void InnerDraw(D2DContext& cxt);
	
protected :
	virtual LRESULT DefWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
	
	std::shared_ptr<D2DControl> Detach(D2DControl* target);

protected :	
	std::vector<std::shared_ptr<D2DControl>> controls_;
	FRectF rc_;

};


ComPTR<ID2D1SolidColorBrush> CreateBrush1(D2DContext& cxt, byte r,byte g,byte b);

#define Brush(r,g,b) CreateBrush1(cxt, r,g,b);


};