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

void CreateEmptyControl(UIHandleWin hwin, UIHandle hcs);

void CreateControl2(UIHandleWin hwin, UIHandle hcs ); // tab3.cpp

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

	FRectF rc(50, 110, FSizeF(900, 1000));
	D2DCreateWhiteControls(&c, c.wboard.drawFunc, c.wboard.procFunc, hwin, hcs, rc, STAT_VISIBLE | STAT_ENABLE, L"tab1_whb1", 1000);

}

// ////////////////////////////////////////////////////////////////////////////////////////


struct SimpleBox
{
	SimpleBox():select(false),clr(0){}
	SimpleBox(int id):clr(0), idx(id),select(false){}
	FSizeF sz;
	ColorF clr;
	int idx;
	bool select;
};
struct SimpleBoxEx : public SimpleBox
{
	SimpleBoxEx(SimpleBox& parent, FRectF r):rc(r)
	{
		idx = parent.idx;
		clr = parent.clr;
	}

	FRectF rc;


};


struct XResize
{
	XResize(){}

	void StartStep(WORD cnt1,FRectF rc1, FRectF rc2 )
	{
		rc = std::shared_ptr<FRectF[]>(new FRectF[cnt1]);
		no = 0;
		cnt = cnt1;

		RectAnimation(rc1,rc2, &rc[0], cnt);
	}

	std::shared_ptr<FRectF[]> rc;
	WORD no, cnt;
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

	std::shared_ptr<SimpleBoxEx> target;

	

};

XResize xresize;


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
					auto box = std::make_shared<SimpleBox>(i);
					box->sz = sz;

					if (!box->select )
						box->clr = D2RGB(155, (100+10*i), 155 );
					else
						box->clr = D2RGB(0, (100+10*i), 195 );

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
				else if ( c.rc.ZeroRect().PtInRect(pt))
				{
					D2DSetCapture(c.hwhite);
					c.mouse_mode = 2;
					

					int idx = (pt.x+c.offset_x ) / CLIENT_BOX_W;

					for(auto& it : c.childs_ )
					{
						it->select = (it->idx == idx);

						if ( it->select )
						{
							FRectF rc1(pt.x- it->sz.width/2, pt.y- it->sz.height/2, it->sz);
							

							c.target = std::make_shared<SimpleBoxEx>(*it, rc1);

						}

					}
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
					else if ( c.mouse_mode == 2 )
					{
						float offx = pt.x-pt2.x;
						float offy = pt.y-pt2.y;

						c.target->rc.Offset( offx, offy);
					}

					b.bRedraw = true;
					r = 1;
				}
			}
			break;
			case WM_LBUTTONUP:
			{				
				if ( D2DIsCapture(c.hwhite) )
				{
					D2DReleaseCapture();

					auto rc1 = c.target->rc;
					FRectF rc2(rc1.left,rc1.top, FSizeF(rc1.Size().width*8, rc1.Size().height*4));

					xresize.StartStep(30,rc1,rc2);
					
					c.target->rc = rc2;
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
		
		c.mat = mat.Offset(c.rc);
		

		mat.PushTransform();
		mat.Offset( -c.offset_x, 0 );

		FRectF rc(0,0, c.rc.Size());
		(*cxt)->DrawRectangle( rc, cxt.black_);


		rc.SetRect(0,0,0,0);
		for(auto&it : c.childs_ )
		{			
			rc.SetSize(it->sz );
			if (it->select )
				FillFRectF(cxt, rc, D2RGB(190,190,190));
			else
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
		(*cxt)->PopAxisAlignedClip();
		

		if ( xresize.rc && xresize.no < xresize.cnt )
		{
			auto& rc4 = xresize.rc[xresize.no++];
			FillFRectF(cxt, rc4, c.target->clr);
			cxt.Redraw();
		}
		else if ( c.target )
			FillFRectF(cxt, c.target->rc, c.target->clr); // D2RGB(100,50,80));

		mat.PopTransform();
	};


	FRectF rc(10, 50, FSizeF(700, 150));
	auto hcs1 = D2DCreateWhiteControls(&c, c.wboard.drawFunc, c.wboard.procFunc, hwin, hcs, rc, STAT_VISIBLE | STAT_ENABLE, L"scroll_control_bar", 2000);

	//CreateEmptyControl(hwin, hcs1);


}
// ////////////////////////////////////////////////////////////////////////////////////////
void CreateEmptyControl(UIHandleWin hwin, UIHandle hcs)
{
	struct CapureObjTab1_2
	{
		D2DMat mat;
		WhiteBoard wboard;
		UIHandleWin hwin;
		UIHandle hcs;
		FRectF rc;
	};

	static CapureObjTab1_2 c;

	c.wboard.procFunc = [](LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)->HRESULT {
		HRESULT hr = 0;
		CapureObjTab1_2& c = *(CapureObjTab1_2*) captureobj;

		switch( message )
		{
			case WM_D2D_CREATE:
				auto h = *(UIHandle*)lParam;
				c.rc = D2DGetRect(h);


				hr = 1;
			break;


		}



		return hr;
	};


	c.wboard.drawFunc =[](LPVOID captureobj, D2DContext& cxt) {
		
		// empty

	};

	FRectF rc(10, 300, FSizeF(700, 600));
	auto hcs1 = D2DCreateClientControls(&c, c.wboard.drawFunc, c.wboard.procFunc, hwin, hcs, rc, STAT_VISIBLE | STAT_ENABLE, L"empty", 2001);


	//CreateControl2(hwin, hcs1 );
}