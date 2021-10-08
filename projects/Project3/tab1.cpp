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

void CreateControl0(UIHandleWin hwin, UIHandle hcs)
{
	   /* FRectF rccmb(50, 100, FSizeF(120, 26));
	    UIHandle cb1 = D2DCreateDropdownListbox(hwin, hcs, rccmb,  STAT_VISIBLE | STAT_ENABLE, L"comb1", 0);
	    D2DAddItem(cb1, 0, L"scale1.0");
	    D2DAddItem(cb1, 1, L"scale1.2");
	    D2DAddItem(cb1, 2,L"scale0.8");*/

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

