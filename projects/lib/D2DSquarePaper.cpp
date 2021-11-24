
#include "pch.h"
#include "D2DWindow.h" 
#include "D2DSquarePaper.h"
#include "D2DDropdownListbox.h"
#include "D2DStatic.h"
#include "D2DButton.h"
#include "D2D1UI_1.h"
using namespace V6;

#define  APP (D2DApp::GetInstance())



void D2DSquarePaper::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();

	mat.Scale(scale_,scale_);

	cxt.DFillRect(rc_, backcolor_); 

	mat.PushTransform();

		mat.Offset(rc_);

#ifdef _DEBUG
		cxt.DText(FPointF(), this->name_.c_str(), D2RGB(170,170,170));
#endif

		FRectF rc(0,0,0,0);
		rc = mat.LPtoDP(rc);

		float y  = pitch_;

		if ( rc.top < 0 )
		{
			while( y+pitch_ < -rc.top )
				y += pitch_;
		}

		auto sz = rc_.Size();
		
		while( y < sz.height)
		{

			WCHAR cb[256];
			wsprintf(cb,L"-->%d", (int)y);

			cxt.DText(FPointF(0,y), cb, theWhite);
			cxt.DFillRect(FRectF(0,y,rc_.Size().width,y+1), forecolor_);


			y+=pitch_;
		}

		float x = pitch_;
		while( x < sz.width)
		{

			cxt.DFillRect(FRectF(x,0,x+1, rc_.Size().height), forecolor_);
			x += pitch_;
		}


	mat.PopTransform();

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

			if ( 2 == wParam && p->sender_parent == this )
			{
				auto id = p->prm1;

				D2DDropdownListbox* p1 = (D2DDropdownListbox*)p->sender.p;

				int x = p1->xGetSelect();

				if ( x == 0 )
					scale_ = 1.0f;
				if ( x == 1 )
					scale_ = 0.8f;
				if ( x == 2 )
					scale_ = 0.5f;
				if ( x == 3 )
					scale_ = 1.2f;
				

				r = 1;
			}
			else if ( 3 == wParam )
			{
				int a = 0;

				UIHandleWin hwin;
				hwin.p = parent_window_;
				FRectF rc(0,0,300,100);

				D2DMessageBox(hwin, rc, L"no problem", L"this is sample text");


				r = 1;
			}
		}
		break;
		case WM_D2D_SET_COLOR:
		{
			if ( wParam == 0 )
				backcolor_ = *(ColorF*)lParam;
			else if ( wParam == 1 )
				forecolor_ = *(ColorF*)lParam;

			r = 1;
			

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
	scale_ = 1.0f;

	{
		FRectF rc0(70,0,FSizeF(200,20));
		auto t1 = std::make_shared<D2DStatic>(); 
		t1->CreateControl(parent,this, rc0,STAT_DEFAULT,NONAME);
		t1->SetText(L"–Ú·‚è");
		this->Add( t1);

		FRectF rc1(70,20,FSizeF(80,20));
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
	///////////////////////////////////////////////////////////


	{
		FRectF rc0(170,0,FSizeF(200,20));
		auto t1 = std::make_shared<D2DStatic>(); 
		t1->CreateControl(parent,this, rc0,STAT_DEFAULT,NONAME);
		t1->SetText(L"SCALE");
		this->Add( t1);

		FRectF rc1(170,20,FSizeF(80,20));
		auto ls = std::make_shared<D2DDropdownListbox>();
		ls->CreateControl(parent,this,rc1,STAT_DEFAULT,NONAME,2);

		ls->AddItem(0,L"100");
		ls->AddItem(1,L"80");
		ls->AddItem(2,L"50");
		ls->AddItem(3,L"120");

		ls->xSetSelect(0);
		

		this->Add(ls);
	}

	{
		auto btn = std::make_shared<D2DButton>();
		btn->CreateControl(parent,this, FRectF(300,20,FSizeF(100,25)), STAT_DEFAULT,L"DialogBox",3);
		this->Add(btn);

		btn = std::make_shared<D2DButton>();
		btn->CreateControl(parent,this, FRectF(450,20,FSizeF(100,25)), STAT_DEFAULT,L"1000 button", 1000);
		this->Add(btn);

	}


}




