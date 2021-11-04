
#include "pch.h"
#include "D2DWindow.h" 
#include "D2DSquarePaper.h"
#include "D2DDropdownListbox.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())



void D2DSquarePaper::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();


	cxt.DFillRect(rc_, D2RGB(105,200,100)); 

	cxt.DText(FPointF(), this->name_.c_str(), D2RGB(170,170,170));

	float y = pitch_;
	while( y < rc_.Size().height )
	{

		WCHAR cb[256];
		wsprintf(cb,L"-->%d", (int)y);

		cxt.DText(FPointF(0,y), cb, theWhite);
		cxt.DFillRect(FRectF(0,y,rc_.Size().width,y+1), theWhite);


		y+=pitch_;
	}

	float x = pitch_;
	while( x < rc_.Size().width )
	{

		cxt.DFillRect(FRectF(x,0,x+1, rc_.Size().height), theWhite);
		x += pitch_;
	}


	D2DControls::Draw(cxt);

	mat.PopTransform();

} 
HRESULT D2DSquarePaper::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT r = 0;

	switch( message )
	{
		case WM_NOTIFY:
		{

			D2DNMHDR* p = (D2DNMHDR*)lParam;


			

			if ( 1 == wParam && p->sender_parent == this )
			{
				auto id = p->prm1;

				D2DDropdownListbox* p1 = (D2DDropdownListbox*)p->sender.p;
				
				int x = p1->xGetSelect();

				if ( x == 0 )
					pitch_ = 50.0f;
				if ( x == 1 )
					pitch_ = 100.0f;
				if ( x == 2 )
					pitch_ = 150.0f;
				if ( x == 3 )
					pitch_ = 200.0f;

				r = 1;
			}



		}
		break;



		default :
			r = 0;


	}

	if ( r == 0 )
		r = D2DControls::WndProc(b,message,wParam,lParam);

	return r;
}
void D2DSquarePaper::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);
	rc_ = rc;


	FRectF rc1(70,50,FSizeF(80,20));
	auto ls = std::make_shared<D2DDropdownListbox>();
	ls->CreateControl(parent,this,rc1,STAT_DEFAULT,NONAME,1);





	ls->AddItem(0,L"50");
	ls->AddItem(1,L"100");
	ls->AddItem(2,L"150");
	ls->AddItem(3,L"200");

	ls->xSetSelect(0);
	pitch_ = 50.0f;

	this->Add(ls);
}




