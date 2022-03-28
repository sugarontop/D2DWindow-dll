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

		void UpdateContents();

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

#define HEIGHT_TITLE	45.0f
#define HEIGHT_ROW	25.0f

void D2DObjectList::Draw(D2DContext& cxt)
{	
	D2DMatrix mat(*cxt);
	
	D2DRectFilter fil(cxt, rc_);


	mat_ = mat.PushTransform();
	cxt.DDrawRect(rc_, ColorF::WhiteSmoke, D2RGB(36,64,92));

	

	mat.Offset(rc_);

	
	
	
	FRectF rc(0,0,FSizeF(rc_.Size().width, HEIGHT_TITLE));
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




		rc.Offset(0,HEIGHT_ROW);
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

void D2DObjectList::UpdateContents()
{

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


	auto h = rows_.size() * HEIGHT_ROW + HEIGHT_TITLE;
	rc_.SetHeight(h);



	parent_control_->SendMesage(WM_D2D_SET_SIZE,3,0);

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
		case WM_LBUTTONDOWN:
		{
			float xpos[] = { 200, 250, 150,100,100 };
			
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);
			
			if ( rc_.PtInRect(pt) )
			{
				D2DMat mat = mat_;

				mat.Offset(rc_.left, rc_.top);

				pt = mat.DPtoLP(pm.pt);

				if ( pt.y < HEIGHT_TITLE )
				{
					if (pt.x < xpos[0] )
					{					
						std::stable_sort(rows_.begin(), rows_.end(), [](const Item& it1, const Item& it2)->bool{
			
							return it1.name < it2.name;
			
						});
					}
					else if ( xpos[0]+xpos[1] < pt.x && pt.x < xpos[0]+xpos[1]+xpos[2] )
					{
						std::stable_sort(rows_.begin(), rows_.end(), [](const Item& it1, const Item& it2)->bool{
				
							return it1.parent_name < it2.parent_name;
			
						});
					}
				}

				r = 1;
			}



			
		}
		break;





	}


	return r;
}

D2DControls* CreateObjectList(D2DControls* ctrl,  FSizeF size, LPCWSTR name )
{
	UIHandle hctrl={};
	hctrl.p = ctrl;


	auto btn = D2DCreateButton(hctrl, FRectF(50,10,FSizeF(100,25)), STAT_DEFAULT, L"update list", 99 );


	DelegateClick on = [](void* sender,LPCWSTR funcnm, void* p)->DWORD {

		auto ctrl = dynamic_cast<D2DControl*>( (D2DControl*)sender);

		std::vector<D2DControl*> ar;
		if ( ctrl->GetParent()->GetControlFromName(L"objlist", ar ))
		{
			for(auto& it : ar )
			{
				auto ls = dynamic_cast<D2DObjectList*>(it);

				if ( ls )
					ls->UpdateContents();
			}
		}

		return 0;
	};

	D2DSetOnClick(btn, on );



	auto rapcontrol = D2DCreateControlsWithScrollbar(hctrl,FRectF(50,50,size),STAT_DEFAULT, name, 100);
	
	auto ls = std::make_shared<D2DObjectList>();

	ls->CreateControl(ctrl->GetParent(), (D2DControls*)rapcontrol.p, FRectF(0,0,FSizeF(size.width, size.height+1000)), STAT_DEFAULT, L"objlist",101 );

	((D2DControls*)rapcontrol.p)->Add(ls);

	return (D2DControls*)rapcontrol.p;


}
