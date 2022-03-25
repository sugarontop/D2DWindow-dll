#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"
#include "D2DColor.h"
#include "D2DWindowControl.h"

using namespace V6;


//class D2DObjectList : public D2DControl
//{
//	public :
//		virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
//
//		virtual FRectF GetRect() const  = 0;
//		virtual void SetRect(const FRectF& rc) = 0;
//		virtual void Draw(D2DContext& cxt) = 0;
//		virtual FRectF& GetRectSmooth() =0;
//		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) = 0;
//
//
//};


class D2DObjectList : public D2DControls
{
	public :
		D2DObjectList(){};

		virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);

		virtual FRectF GetRect() const { return rc_; }
		virtual void SetRect(const FRectF& rc) { rc_ = rc;}
		virtual void Draw(D2DContext& cxt) ;
		virtual FRectF& GetRectSmooth(){ return rc_; }
		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);

		std::vector<std::wstring> titles_;

		struct Item
		{
			std::wstring name;
			FRectF rc;
			std::wstring parent_name;


		};
		std::vector<Item> rows_;

};

void D2DObjectList::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id )
{
	InnerCreateWindow(parent, pacontrol,stat,name,local_id);

	rc_ = rc;

	AppBase ab = {};
	WndProc(ab, WM_D2D_CREATE,  (WPARAM)0 ,(LPARAM)0);

	
}

void D2DObjectList::Draw(D2DContext& cxt)
{	
	D2DMatrix mat(*cxt);
	
	D2DRectFilter fil(cxt, rc_);


	mat_ = mat.PushTransform();
	cxt.DDrawRect(rc_, ColorF::WhiteSmoke, D2RGB(36,64,92));

	

	mat.Offset(rc_);

	
	
	
	FRectF rc(0,0,FSizeF(rc_.Size().width, 45));
	cxt.DFillRect(rc, D2RGB(53,94,143));


	float xpos[] = { 200, 250, 150,100,100 };

	FPointF pt = rc.LeftTop(13,11);
	int i = 0;
	for(auto& s : titles_ )
	{
		cxt.DText(pt,s, cxt.white_ );

		pt.x += xpos[i++];

	}

	rc.Offset(0,rc.Height());
	rc.SetHeight(25);

	i = 0;
	for(auto& it : rows_)
	{
		
		if ( i%2==0)
			cxt.DFillRect(rc, D2RGB(90,111,131));
		else
			cxt.DFillRect(rc, D2RGB(74,96,118));

		auto& row = rows_[i];

		pt = rc.LeftTop(13,4);

		cxt.DText( pt, row.name, cxt.white_); pt.x += xpos[0];
		
		cxt.DText(pt, row.rc.ToStr(), cxt.white_ ); pt.x += xpos[1];
		
		cxt.DText( pt, row.parent_name, cxt.white_); pt.x += xpos[2];

		//row.rc;




		rc.Offset(0,25);
		i++;
	}



	FRectF rc1(xpos[0],0,xpos[0]+1,rc_.Height());

	i=1;
	for(auto& s : titles_ )
	{
		cxt.DFillRect(rc1, ColorF::DarkGray);
		rc1.Offset(xpos[i++],0);
	}


	mat.PopTransform();
}

LRESULT D2DObjectList::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;

	switch(message)
	{
		case WM_D2D_CREATE:
		{
			titles_ = {L"name",L"position",L"parent",L"------",L"------"};


			auto ar = GetParent()->name_map_;

			for(auto& it : ar)
			{
				Item m;

				

				m.rc = it.second->GetRect();
				m.name = it.second->GetLocalName();

				if ( it.second->GetParentControls())
					m.parent_name = it.second->GetParentControls()->GetLocalName();

				rows_.push_back(m);

			}


			r = 1;
		}
		break;
		case WM_RBUTTONDOWN:
		{
			// ‚Æ‚è‚ ‚¦‚¸
			
			auto ar = GetParent()->name_map_;

			rows_.clear();
			for(auto& it : ar)
			{
				Item m;

				

				m.rc = it.second->GetRect();
				m.name = it.second->GetLocalName();

				if ( it.second->GetParentControls())
					m.parent_name = it.second->GetParentControls()->GetLocalName();

				rows_.push_back(m);

			}
		}
		break;





	}


	return r;
}







D2DControls* CreateObjectList(D2DControls* ctrl,  FSizeF size, LPCWSTR k )
{

	auto ls = std::make_shared<D2DObjectList>();

	ls->CreateControl(ctrl->GetParent(), ctrl, FRectF(50,50,size), STAT_DEFAULT, k );

	ctrl->Add(ls);

	return ls.get();


}
