#include "pch.h"
#include "D2DLogin.h"
#include "D2D1UI_1.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())

D2DLogin::D2DLogin()
{

}
void D2DLogin::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();

	cxt.DFillRect(rc_, D2RGB(255,255,255));


	mat.Offset(rc_);

	
	InnerDraw(cxt);


	mat.PopTransform();

}
LRESULT D2DLogin::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;

	switch( message )
	{
		case WM_NOTIFY:
		{
			D2DNMHDR* p = (D2DNMHDR*)lParam;
			if ( p->sender_parent == this )
			{
				if (wParam == 1)
				{	
					// LOGIN
					auto cd = D2DGetText(usercd_, true );
					auto pwd = D2DGetText(password_, true );


					if ( on_try_login_ )
					{
						BSTR b[3];
						b[0] = cd;
						b[1] = pwd;
						b[2] = nullptr;
						
						if ( 0 == on_try_login_(this, b ) )
						{
							APP.ReleaseCapture();
							DestroyControl();
						}

						if ( b[2] )
							D2DSetText(message_, b[2]);

					}
					else if( !wcscmp(cd,pwd) )
					{
#ifdef _DEBUG
						APP.ReleaseCapture();
						DestroyControl();
#endif
					}
				}
				else if (wParam == 2)
				{
					// CANCEL
					APP.ReleaseCapture();
					DestroyControl();
				}

				r = 1;
			}
		}
		break;
		case WM_KEYDOWN:
		{
			auto key = 0xff & wParam;

			if ( key == VK_ESCAPE && APP.IsCapture(this) )
			{
				APP.ReleaseCapture();

				DestroyControl();
				return 1;
			}
		}
		break;

	}


	if ( r == 0 )
		r = D2DControls::WndProc(b,message,wParam,lParam);

	return r;

}
void D2DLogin::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControls::CreateControl(parent, pacontrol,rc, stat, name, local_id);
	rc_ = rc;

	UIHandle c={};
	UIHandleWin w={};
	w.p = parent;
	c.p = this;

	usercd_  = D2DCreateTextbox(c, FRectF(150,100, FSizeF(100,20)), false, STAT_DEFAULT, NONAME);
	password_ = D2DCreateTextbox(c, FRectF(150,130, FSizeF(100,20)), false, STAT_DEFAULT, NONAME, -1,1);
	message_  = D2DCreateTextbox(c, FRectF(20,250, FSizeF(300,20)), false, STAT_DEFAULT, NONAME,-1,2);

	auto btn_logon = D2DCreateButton(c,FRectF(50, 180, FSizeF(250,20)), STAT_DEFAULT, NONAME, 1);
	//auto btn_cancel = D2DCreateButton(w,c,FRectF(200, 180, FSizeF(100,20)), STAT_DEFAULT, NONAME, 2);

	auto usercde_t  = D2DCreateStatic(c, FRectF(50,100, FSizeF(100,26)), STAT_DEFAULT,L"  USERCD:", NONAME);
	auto password_t = D2DCreateStatic(c, FRectF(50,130, FSizeF(100,26)), STAT_DEFAULT,L"PASSWORD:", NONAME);

	D2DSetText(btn_logon, L"Sign in");
	//D2DSetText(btn_cancel, L"cancel");

	D2DSetText(usercd_, L"admin");
	D2DSetText(password_, L"admin");


	D2DSetText(message_, L"----");
}
