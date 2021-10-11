#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"

using namespace V6;

#include "trcell.h"

struct WhiteBoard
{
	DelegateDrawFunc drawFunc;
	DelegateProcFunc procFunc;
	int typ;
	D2DMat mat;
};
struct CapureObjTab1
{
	D2DMat mat;
	WhiteBoard wboard;
	std::shared_ptr<TRCellRow> row;
	std::shared_ptr<TRCellRow> row2;
	UIHandleWin hwin;
	UIHandle hcs;
	UIHandle tx;

};

void CreateScrollControlBar(UIHandleWin hwin, UIHandle hcs);
void CreateTest(UIHandleWin hwin, UIHandle hcs);


ComPTR<ID2D1SolidColorBrush> MkBrush( D2DContext& cxt, ColorF clr)
{
	ComPTR<ID2D1SolidColorBrush> br1;
	(*cxt)->CreateSolidColorBrush( clr, &br1);
	return br1;
}
ComPTR<ID2D1SolidColorBrush> MkBrush( D2DContext& cxt, int r, int g, int b, int a)
{
	return MkBrush(cxt,  D2RGBA(r,g,b,a));
}

void FillFRectF( D2DContext& cxt, FRectF& rc, ColorF clr)
{
	ComPTR<ID2D1SolidColorBrush> br1;
	(*cxt)->CreateSolidColorBrush( clr, &br1);
	(*cxt)->FillRectangle(rc, br1 );
}


void CreateControl0(UIHandleWin hwin, UIHandle hcs)
{
	   /* FRectF rccmb(50, 100, FSizeF(120, 26));
	    UIHandle cb1 = D2DCreateDropdownListbox(hwin, hcs, rccmb,  STAT_VISIBLE | STAT_ENABLE, L"comb1", 0);
	    D2DAddItem(cb1, 0, L"scale1.0");
	    D2DAddItem(cb1, 1, L"scale1.2");
	    D2DAddItem(cb1, 2,L"scale0.8");*/


		//CreateTest(hwin,hcs);
		CreateScrollControlBar(hwin, hcs);

}

void CreateTest(UIHandleWin hwin, UIHandle hcs)
{
	static CapureObjTab1 c;

	c.hwin = hwin;
	c.hcs = hcs;		

	c.wboard.procFunc = [](LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)->HRESULT {

		switch(message )
		{
		case WM_D2D_CREATE:
		{
			FRectF rc[7];
			FRectF rck(10,200,110,226);
			for(int i =0; i < 7; i++ )
			{
				rc[i] = rck;
				rck.Offset(100,0);
			}

			c.row = std::make_shared<TRCellRow>( rc, 7 );
			c.row2 = std::make_shared<TRCellRow>( rc, 7, 0 );

			UIHandle hcs = *(UIHandle*)lParam;

			FRectF rc2(10, 226, FSizeF(100, 26));
			c.tx = D2DCreateTextbox(c.hwin, hcs, rc2, false, STAT_VISIBLE | STAT_ENABLE, L"input_editor");
			D2DSetColor( c.tx, D2RGB(2,232,60), D2RGB(0,0,0),D2RGB(0,0,0));
			//D2DSetText(c.tx, L"test");


			return 1;
		}
		break;
		case WM_D2D_TEXTBOX_KEYDOWN:
		{
			if ( wParam == VK_RETURN )
			{

				FRectF rc  = D2DGetRect( c.tx );

				rc.Offset(rc.Size().width,0);

				D2DSetRect( c.tx, rc );

				return 1;

			}
		}
		break;
		};




		return 0;
	};

	c.wboard.drawFunc =[](LPVOID captureobj, D2DContext& cxt) {


		D2DMatrix mat(*cxt);

		mat.PushTransform();

		mat.Offset(50,110);

		c.row->Draw(cxt);

		mat.Offset(0,26);
		c.row2->Draw(cxt);


		mat.PopTransform();

	};

	FRectF rc(50, 110, FSizeF(700, 600));
	D2DCreateWhiteControls(&c, c.wboard.drawFunc, c.wboard.procFunc, hwin, hcs, rc, STAT_VISIBLE | STAT_ENABLE, L"tab1_whb1", 1000);

}

struct SimpleBox
{
	SimpleBox():clr(0){}
	FSizeF sz;
	ColorF clr;

};

struct CapureObjTab1_1
{
	D2DMat mat;
	WhiteBoard wboard;
	UIHandleWin hwin;
	UIHandle hcs;

	UIHandle hwhite;
	FRectF rc;

	std::vector<std::shared_ptr<SimpleBox>> childs_;
	float offset_x,inner_size, thumb_size;
	int mouse_mode;

};
#define CLIENT_BOX_W 100
void CreateScrollControlBar(UIHandleWin hwin, UIHandle hcs)
{
	static CapureObjTab1_1 c;

	c.hwin = hwin;
	c.hcs = hcs;		
	c.offset_x = 50;

	c.wboard.procFunc = [](LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)->HRESULT {

		CapureObjTab1_1& c = *(CapureObjTab1_1*) captureobj;
		HRESULT r = 0;
		switch( message )
		{
			case WM_D2D_CREATE:
			{
				c.hwhite = *(UIHandle*)lParam;
				c.rc = D2DGetRect(c.hwhite);
				FSizeF sz = c.rc.Size();
				sz.width = CLIENT_BOX_W;
				
				c.inner_size = 10*sz.width;
				c.thumb_size = 10;

				for(int i = 0; i<10; i++ )
				{
					auto box = std::make_shared<SimpleBox>();
					box->sz = sz;

					
					box->clr = D2RGB(155, (100+10*i), 155 );
					c.childs_.push_back(box);
				}
			}
			return 1;

			case WM_LBUTTONDOWN:
			{				
				FRectF rc(0,0, c.rc.Size());
				rc.top = rc.bottom - 12;

				MouseParam* mp = (MouseParam*)lParam;
				auto pt = c.mat.DPtoLP(mp->pt);

				if ( rc.PtInRect(pt) )
				{
					D2DSetCapture(c.hwhite);
					c.mouse_mode = 1;
					r = 1;
				}
				else if ( c.rc.PtInRect(pt))
				{
					D2DSetCapture(c.hwhite);
					c.mouse_mode = 2;
					

					int idx = (pt.x+c.offset_x ) / CLIENT_BOX_W;


					r = 1;
				}
			}
			break;
			case WM_MOUSEMOVE:
			{
				if ( D2DIsCapture(c.hwhite) )
				{
					MouseParam* mp = (MouseParam*)lParam;
					auto pt = c.mat.DPtoLP(mp->pt);
					auto pt2 = c.mat.DPtoLP(mp->ptprv);

					if ( c.mouse_mode == 1 )
						c.offset_x = max(0, pt.x - pt2.x+c.offset_x);

					b.bRedraw = true;
					r = 1;
				}
			}
			break;
			case WM_LBUTTONUP:
			{				
				FRectF rc(0,0, c.rc.Size());
				rc.top = rc.bottom - 12;

				if ( D2DIsCapture(c.hwhite) )
				{
					D2DReleaseCapture();
					c.mouse_mode = 0;
					r = 1;
				}
			}
			break;

		}
		return r;
	};

	c.wboard.drawFunc =[](LPVOID captureobj, D2DContext& cxt) {

		CapureObjTab1_1& c = *(CapureObjTab1_1*) captureobj;
		D2DMatrix mat(*cxt);
		(*cxt)->PushAxisAlignedClip( c.rc, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE );		

		mat.PushTransform();
		
		mat.Offset(c.rc);
		c.mat = mat;

		mat.PushTransform();
		mat.Offset( -c.offset_x, 0 );

		FRectF rc(0,0, c.rc.Size());
		(*cxt)->DrawRectangle( rc, cxt.black_);


		rc.SetRect(0,0,0,0);
		for(auto&it : c.childs_ )
		{			
			rc.SetSize(it->sz );
			FillFRectF(cxt, rc, it->clr);
			rc.Offset(rc.Size().width, 0); 
			
		}
		mat.PopTransform();
		{
			// draw scroll bar 
			rc.SetRect(0,0, c.rc.Size());
			rc.top = rc.bottom - 12;
			FillFRectF(cxt, rc, D2RGB(170,170,170));

			rc.left = c.offset_x;
			rc.right = rc.left + 5;
			(*cxt)->FillRectangle(rc, cxt.black_);
		}
		
		mat.PopTransform();
		(*cxt)->PopAxisAlignedClip();
	};


	FRectF rc(10, 50, FSizeF(700, 150));
	D2DCreateWhiteControls(&c, c.wboard.drawFunc, c.wboard.procFunc, hwin, hcs, rc, STAT_VISIBLE | STAT_ENABLE, L"scroll_control_bar", 2000);
}

