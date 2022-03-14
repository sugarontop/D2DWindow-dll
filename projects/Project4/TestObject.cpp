#include "pch.h"
#include "CJsValueRef.h"
#include "javasc.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"
#include "D2DColor.h"
#include "D2DWhiteWindowControl.h"
#include "TestObject.h"

using namespace V6;
#define  APP (D2DApp::GetInstance())

struct AliceInstance
{
	AliceInstance()
	{
		clr[0] = ColorF::AliceBlue;
		clr[1] = ColorF::AliceBlue;
		clr[2] = ColorF::AliceBlue;
		
		hme={};
		hprv={};
		hactive={};
		prc = nullptr;
		child = nullptr;
		opt = 0;
	};

	FRectF* prc;
	FRectF orgrc;
	D2DColor clr[3];
	D2DMat mat;
	UIHandle hme;
	UIHandle hprv;

	static UIHandle hactive;
	D2DControls* child;
	int opt;

	FRectF scale_rc;

};

UIHandle AliceInstance::hactive;

bool ef1(LPVOID captureobj, D2DContext& cxt)
{
	AliceInstance* m = (AliceInstance*)captureobj;

	auto stat = D2DGetStat(m->hme);
	if ( BITFLG2( stat, STAT_VISIBLE))
	{
		D2DMatrix mat(*cxt);
		mat.PushTransform();

		FRectF rc = *(m->prc);
		FPointF cpt = rc.CenterPt();

		auto cptdev = mat.LPtoDP(cpt);

		float rto = m->scale_rc.Width() / 50.0f;

		mat.Scale(rto, rto);
		
		auto cp2 = mat.DPtoLP(cptdev);

		m->mat = mat.Offset(cp2.x-cpt.x,cp2.y-cpt.y);


		D2DRectFilter fil(cxt, rc);
		mat.Offset(rc);
	
		FRectF rc1 = rc.ZeroRect();
		cxt.DDrawRect(rc1, m->clr[1], ColorF::DarkCyan);
		
		(*cxt)->DrawLine(FPointF(0,rc1.Height()/2), FPointF(rc1.right,rc1.Height()/2), cxt.black_);
		(*cxt)->DrawLine(FPointF(rc1.Width()/2, 0), FPointF(rc1.Width()/2, rc1.bottom), cxt.black_);

	
		BSTR s = D2DGetLocalName(m->hme);
		cxt.DText(rc1.LeftTop(), s, ColorF::White);
		::SysFreeString(s);

		mat.PopTransform();
	}
	return false;
}


LRESULT ef2(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	AliceInstance* m = (AliceInstance*)captureobj;
	auto me = (D2DControl*)b.card;


	LRESULT r = 0, r2 = 0;

	switch( message )
	{
		case WM_D2D_CREATE:
		{
			m->hme = *(UIHandle*)lParam;						
			m->orgrc = D2DGetRect(m->hme);
			m->prc = (FRectF*)D2DGetRect2(m->hme);
			m->child = nullptr;
			m->scale_rc = m->orgrc;

			r =1;
		}
		break;
		case WM_D2D_DESTROY:
		{
			delete m;
			r= 1;
		}
		break;
		case WM_LBUTTONDOWN:
		{
			MouseParam& pm = *(MouseParam*)lParam;

			auto pt = m->mat.DPtoLP(pm.pt);

			FRectF rc = *(m->prc);

			if ( rc.PtInRect(pt) )
			{
				r = 1;

				APP.SetCapture(me);
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			if ( APP.IsCapture(me))
			{
				APP.ReleaseCapture();

				m->hactive = m->hme;
			}

			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = m->mat.DPtoLP(pm.pt);
			FRectF rc = *(m->prc);
			if ( rc.PtInRect(pt) )
			{
				r=1;
			}
			else
			{
				m->hactive = {0};
				r = 1;
			}
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = m->mat.DPtoLP(pm.pt);
			FRectF rc = *(m->prc);

			auto hwin = D2DGetWindow(m->hme);

			if ( rc.PtInRect(pt) )
			{
				if ( m->opt == 0 )
				{
					rc.SetHeight(400);
					rc.SetWidth(400);
					m->opt = 1;
					D2DSmoothRect(1,101,m->hme,&m->scale_rc, rc);

					D2DSetTopControl(m->hme);

				}
				else
				{
					rc.SetHeight(50);
					rc.SetWidth(50);
					m->opt = 0;
					D2DSmoothRect(1,100,m->hme, &m->scale_rc, rc);
				}
				r = 1;
			}
		}
		break;
	}

	if ( r == 0 )
		r = D2DDefControlProc(m->hme,b,message,wParam,lParam);

	return r+r2;
}

UIHandle V6::CreateTestObj(UIHandle parent, FRectF rc, LPCWSTR nm, int id)
{
	return D2DCreateWhiteControls( (LPVOID)new AliceInstance(), ef1,ef2, parent, rc, STAT_DEFAULT, nm, id);
}