
#include "pch.h"
#include "D2DContext.h"
#include "D2DMisc.h"
#include "D2D1UI_1.h"
#include "D2DColor.h"
#include "D2DApp.h"
#include "D2DMessage.h"
#include "imagetool.h"


using namespace V6;

struct BobInstance
{
	BobInstance()
	{
		clr[0] = ColorF::AliceBlue;
		clr[1] = ColorF::AliceBlue;
		clr[2] = ColorF::AliceBlue;
		
		hme={};
		hprv={};
		hactive={};
		prc = nullptr;
		child = nullptr;
	};

	FRectF* prc;
	FRectF orgrc;
	D2DColor clr[3];
	D2DMat mat;
	UIHandle hme;
	UIHandle hprv;
	UIHandle hp1c;


	std::vector<std::function<void(std::wstring)>> event_ar;

	UIHandle hactive;
	D2DControls* child;
	FSizeF voff;

	UIHandle hp1;
	static bool df1(LPVOID captureobj, D2DContext& cxt);
	static LRESULT df2(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
};

void CreatePage2(UIHandle h2);

UIHandleWin CreateControl(HWND hWnd)
{
	UIHandleWin hwin = D2DCreateMainHWnd(hWnd, 14,0);    
    UIHandle root = D2DGetRootControls(hwin);

	RECT rc;
	::GetClientRect(hWnd, &rc);
	
	auto bob = new BobInstance();
	D2DCreateWhiteControls((LPVOID)bob,BobInstance::df1,BobInstance::df2, root, FRectF(0.0f,0.0f,(float)rc.right,(float)rc.bottom),STAT_DEFAULT,L"bitcoin",1);

	return hwin;
}



bool BobInstance::df1(LPVOID captureobj, D2DContext& cxt)
{
	BobInstance* m = (BobInstance*)captureobj;
	D2DMatrix mat(*cxt);

	m->mat = mat.PushTransform();
	FRectF& rc = *(m->prc);
	D2DRectFilter fil(cxt, rc);	
	mat.Offset(rc);	
	cxt.DFillRect(rc.ZeroRect(), m->clr[0]);


	D2DInnerDraw(m->hme, (LPVOID)&cxt);


	mat.PopTransform();

	return false;
}


LPCWSTR _strformat( LPCWSTR expression, ... )
{
	static WCHAR cb[256];
	va_list args;
    va_start( args, expression );
	StringCchVPrintf(cb, 256, expression, args );
	va_end(args);
	return cb;
}


struct CustomBtn
{
	FRectF* prc;
	FRectF orgrc;
	D2DMat mat;
	UIHandle hme;
	std::wstring nm;
	D2DColor clr;

	std::function<void(std::wstring)> click_;


	static bool df1(LPVOID captureobj, D2DContext& cxt);
	static LRESULT df2(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);

};



static void DrawRect1(D2DContext& cxt, FRectF rc, ColorF back, ColorF line)
{
	cxt.DFillRect(rc, back);
	rc.InflateRect(-3,-3);
	cxt.DDrawRect(rc, line, back);
}

static ID2D1SolidColorBrush* CreateBrush(D2DContext& cxt, ColorF clr)
{
	if ( clr.r==0 && clr.g==0 && clr.b==0 )
		return cxt.black_ ;
	else if ( clr.r==1 && clr.g==1 && clr.b==1 )
		return cxt.white_;
	else 
	{
		static ComPTR<ID2D1SolidColorBrush> br;
		
		br = nullptr;

		(*cxt)->CreateSolidColorBrush(clr, &br);
		
		return br;
	}
}

void DrawCenterText(D2DContext& cxt, FRectF rc, LPCWSTR text, ColorF font)
{
	ComPTR<IDWriteTextLayout> layout;
	
	if (SOK(cxt.wfactory_->CreateTextLayout(text, wcslen(text), cxt.textformat_, rc.Width(), rc.Height(), &layout )))
	{
		DWRITE_TEXT_METRICS t;
		layout->GetMetrics(&t);

		FPointF pt( (rc.Width()-t.width)/2.0f, (rc.Height()-t.height)/2.0f );

		auto br = CreateBrush(cxt, font);

		(*cxt)->DrawTextLayout(pt, layout, br );
	}
}




bool CustomBtn::df1(LPVOID captureobj, D2DContext& cxt)
{
	CustomBtn* m = (CustomBtn*)captureobj;
	D2DMatrix mat(*cxt);

	m->mat = mat.PushTransform();
	FRectF& rc = *(m->prc);
	D2DRectFilter fil(cxt, rc);	
	mat.Offset(rc);	

	FRectF rca(0,0,rc.Width(),40);

	DrawRect1(cxt, rca, m->clr, D2RGB(255,255,255));
	DrawCenterText(cxt, rca, m->nm.c_str(), D2RGB(255,255,255));

	mat.PopTransform();

	return false;

}
LRESULT CustomBtn::df2(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;

	CustomBtn* m = (CustomBtn*)captureobj;

	switch(message )
	{
		case WM_D2D_CREATE:
		{
			m->hme = *(UIHandle*)lParam;						
			m->orgrc = D2DGetRect(m->hme);
			m->prc = (FRectF*)D2DGetRect2(m->hme);
			m->nm = D2DGetLocalName(m->hme);
			D2DColor c1(0x998a9e);

			m->clr = c1;
		}
		break;		
		case WM_D2D_DESTROY:
			delete m;
			ret = 1;
		break;
		case WM_LBUTTONDOWN:
		{
			MouseParam& pm = *(MouseParam*)lParam;

			auto pt = m->mat.DPtoLP(pm.pt);

			if ( m->orgrc.PtInRect(pt) )
			{
				
				if ( m->click_ )
					m->click_(m->nm);


				D2DSmoothRect(100,100,m->hme,m->prc, m->orgrc);

				ret = 1;
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			MouseParam& pm = *(MouseParam*)lParam;

			auto pt = m->mat.DPtoLP(pm.pt);

			if ( m->orgrc.PtInRect(pt) )
			{
				
				
				ret= 1;
			}
		}
		break;


	}


	if ( ret == 0 )
		ret = D2DDefControlProc(m->hme,b,message,wParam,lParam);


	return ret;
}

D2DControls* CreateStockChart(D2DControls* ctrl,  FSizeF size, LPCWSTR nm );
D2DControls* CreateSolidStockChart(D2DControls* ctrl, std::wstring cd, FRectF rc, LPCWSTR nm );
D2DControls* CreateWealthNaviStockChart(D2DControls* ctrl,  FSizeF size, LPCWSTR nm );


bool CreateBitmapListboxItem(ID2D1RenderTarget* rt,UINT w, UINT h, std::wstring titlenm, ID2D1Bitmap** pbmp);
void ShowToolListBox(UIHandle h, bool bShow);
void SqlCommandRun(UIHandleWin hwin);

#define TOOL_BTN		1001
#define TOOL_LISTBOX	1002
#define SQL_RUN			1003

LRESULT BobInstance::df2(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;
	BobInstance* m = (BobInstance*)captureobj;
	switch (message)
	{
		case WM_D2D_CREATE:
		{
			m->hme = *(UIHandle*)lParam;						
			m->orgrc = D2DGetRect(m->hme);
			m->prc = (FRectF*)D2DGetRect2(m->hme);

			FRectF rc = *(m->prc);

			float height = rc.Height() / 3.0f;

			rc.bottom = rc.top + height*2;

			// 上側 --------------------------------------------------------------------------------------
			auto hP1 = D2DCreateControlsWithScrollbar(m->hme, rc, STAT_DEFAULT|STAT_IGNORE_VSIZE, L"P1" );
			m->hp1 = hP1;

				
			// Create Tab Control
			auto htab = D2DCreateTabControls(hP1,FRectF(0,0,3000,3000),STAT_DEFAULT|STAT_IGNORE_SIZE,L"P1_TABC");
			
				// Tab page1
				D2DSendMessage(htab,WM_D2D_SET_TEXT,0,(LPARAM)L"page1");

				auto htabc1 = D2DGetTab(htab,0);				
				auto hP1c = D2DCreateSquarePaper(htabc1,FRectF(0,0,3000,3000), STAT_DEFAULT|STAT_IGNORE_SIZE,L"TAB_PAGE1");

				// Tab page2
				auto htabc2 = D2DAddNewTab(htab, L"page2");
				auto htabc21 = D2DCreateSquarePaper(htabc2,FRectF(0,0,3000,3000), STAT_DEFAULT|STAT_IGNORE_SIZE,L"TAB_PAGE2");
			


			
			// 下側 --------------------------------------------------------------------------------------
			rc.top = rc.bottom;

			rc.SetHeight(height);
			auto hP2 = D2DCreateControlsWithScrollbar(m->hme, rc, STAT_DEFAULT|STAT_IGNORE_VSIZE, L"P2" );
			auto hP2c = D2DCreateXXXControls(hP2,FRectF(0,0,3000,1000), STAT_DEFAULT|STAT_IGNORE_SIZE,L"P2C");


			D2DColor clr(D2RGB(190,190,190));
			LPCWSTR cb = _strformat(L"mode=1&bkcolor=%d", clr.ToInt());
			D2DSendMessage(hP2c, WM_D2D_COMMAND_SET, (WPARAM)hP2c.p, (LPARAM)cb);



			LPCWSTR nm[] = {L"VTI",L"MSFT",L"GOOG",L"AAPL",L"AMZN",L"ROBO_ADVISER"};

			FRectF rcs(50,80,FSizeF(150,40));

			for(int i = 0; i < _countof(nm); i++ )
			{
				// カスタマイズされたボタン

				CustomBtn* btn = new CustomBtn();
				D2DCreateWhiteControls((LPVOID)btn,CustomBtn::df1,CustomBtn::df2, hP2c, rcs,STAT_DEFAULT, nm[i], 100+i);

				LPCWSTR titlenm = nm[i];

				if ( !wcscmp(titlenm, L"ROBO_ADVISER"))
				{
					//page1 ボタンをPUSHした時の挙動
					auto push1 = [hP1c, titlenm, htabc21](std::wstring)
					{
						FRectF  rc(200,25,FSizeF(1000,650));
						auto h1 = D2DCreateChildWindow(hP1c, rc, STAT_DEFAULT, L"ChildWin_chart" );
						auto h2 = D2DCreateControlsWithScrollbar(h1,FRectF(0,0,FSizeF(0,0)),STAT_DEFAULT|STAT_IGNORE_SIZE|STAT_AUTOFIT_CHILDWIN,NONAME);

						
						D2DControls* x = CreateWealthNaviStockChart((D2DControls*)h2.p, rc.Size(), titlenm );

						D2DColor clr(D2RGB(250,250,250));
						LPCWSTR cb = _strformat(L"mode=1&title=%s&bkcolor=%d",  titlenm, clr.ToInt());
						D2DSendMessage(h2, WM_D2D_COMMAND_SET, (WPARAM)h2.p,(LPARAM)cb);
						D2DSendMessage(h2, WM_D2D_SET_SIZE, 4,0);
						D2DSendMessage(h1, WM_D2D_SET_SIZE,0,0);

					};
					m->event_ar.push_back(push1);
					btn->click_ = push1;
				}
				else
				{
					//page1 ボタンをPUSHした時の挙動
					auto push1 = [hP1c, titlenm, htabc21](std::wstring)
					{
					
						// page1
						auto h1 = D2DCreateChildWindow(hP1c, FRectF(200,150,FSizeF(1200,650)), STAT_DEFAULT, L"ChildWin_chart" );
						auto h2 = D2DCreateControlsWithScrollbar(h1,FRectF(0,0,FSizeF(0,0)),STAT_DEFAULT|STAT_IGNORE_SIZE,NONAME);


						D2DControls* x = CreateStockChart((D2DControls*)h2.p,  FSizeF(1200,650), titlenm );

						D2DColor clr(D2RGB(250,250,250));
						LPCWSTR cb = _strformat(L"mode=1&title=%s&bkcolor=%d",  titlenm, clr.ToInt());
						D2DSendMessage(h2, WM_D2D_COMMAND_SET, (WPARAM)h2.p,(LPARAM)cb);
						D2DSendMessage(h2, WM_D2D_SET_SIZE, 4,0);
						D2DSendMessage(h1, WM_D2D_SET_SIZE,0,0);


						

						// page2 作り直し

						auto hm = D2DGetControlFromName(D2DGetWindow(hP1c), L"#msft");

						if ( hm.p )
							D2DDestroyControl(hm);

						h2 = D2DCreateControlsWithScrollbar(htabc21,FRectF(100,100,FSizeF(1200,1000)),STAT_DEFAULT|STAT_IGNORE_SIZE,L"#msft");

						//x = CreateSolidStockChart((D2DControls*)h2.p, L"not",  FRectF(0,0,FSizeF(1200,680)), titlenm );					
						CreatePage2(h2);
					

						D2DSendMessage(h2, WM_D2D_SET_SIZE, 4,0);
					


					};

					m->event_ar.push_back(push1);

					btn->click_ = push1;
				}

				rcs.Offset( 200, 0);
			}

			

			/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
			m->hp1c = hP1c;
			auto btn = D2DCreateButton(hP1c, FRectF(100,100,FSizeF(100,30)), STAT_DEFAULT, L"tool", TOOL_BTN );


			r = 1;
		}
		break;
		case WM_D2D_DESTROY:
			delete m;
			r = 1;
		break;
		case WM_SIZE:
		{						
			// 全体画面へ
			FRectF rc = *(FRectF*)lParam;
			*(m->prc) = rc;			
		}
		break;
		case WM_NOTIFY:
		{
			if (TOOL_BTN == wParam )
			{
				auto h = D2DGetControlFromName( D2DGetWindow(m->hp1c), L"#list_bmp");
				
				if ( h.p == nullptr )				
					ShowToolListBox(m->hp1c, true);
				else
					ShowToolListBox(m->hp1c, false); 

				r = 1;
			}
			else if ( TOOL_LISTBOX == wParam )
			{
				D2DNMHDR& pm = *(D2DNMHDR*)lParam;

				if ( pm.code == EVENTID_SELECTCHANGED )
				{
					int idx = pm.prm1;

					if ( idx < m->event_ar.size() )
						m->event_ar[idx](L"xxx");
				}

				r = 1;
			}
			else if (SQL_RUN == wParam)
			{
				
				auto h = D2DGetWindow(m->hme);
				
				SqlCommandRun(h);


				r = 1;
			}
		}
		break;
		case WM_D2D_SMOOTH_COMPLETE:
		{
			if ( lParam == 11)
			{
				auto h = D2DGetControlFromName( D2DGetWindow(m->hp1c), L"#list_bmp");

				D2DDestroyControl(h);


				h = D2DGetControlFromName( D2DGetWindow(m->hp1c), L"#list_bmp");
				_ASSERT(h.p == nullptr);



				r = 1;
			}
			else if ( lParam == 10)
			{
				
				r = 1;
			}
		}
		break;


	}

	if ( r == 0 )
		r = D2DDefControlProc(m->hme,b,message,wParam,lParam);

	return r;



}
void ShowToolListBox( UIHandle hP1c, bool bShow)
{
	FRectF rcDest(10,200,FSizeF(200,500));
	FRectF rcFirst(1000,0,FSizeF(200,0));

	if ( !bShow )
	{
		auto hP2c1 = D2DGetControlFromName( D2DGetWindow(hP1c), L"#list_bmp");
		
		D2DSmoothRect(1,11, hP1c, D2DGetRect2(hP2c1), rcFirst);
	}
	else
	{
		// listbox作成
		auto hP2c1 = D2DCreateListbox(hP1c, rcFirst, STAT_DEFAULT,L"#list_bmp", TOOL_LISTBOX);

		// リサイズ
		D2DSmoothRect(1,10, hP1c, D2DGetRect2(hP2c1), rcDest);


		// listbox内の内容
		D2DColor clrb(D2RGBA(0,0,0,0));
		D2DSendMessage(hP2c1, WM_D2D_SET_COLOR, COLOR_IDX_BORDER,(LPARAM)&clrb);


		ComPTR<ID2D1RenderTarget> rt;
			
		// メモリ上のRnderTargetを作成
		CreateMemoryRenderTarget(200,30, &rt);
			
			
		// itemの指定
		D2DColor clrs[] = {D2RGB(0,255,0),D2RGB(255,255,0),D2RGB(0,255,255),D2RGB(0,255,0),D2RGB(255,255,0),D2RGB(0,255,255)};
		LPCWSTR nmstock[] = {L"VTI",L"MSFT",L"GOOG",L"AAPL",L"AMZN",L"ROBO_ADVISER"};


		// メモリ上のRnderTargetで使用するフォント作成
		ComPTR<IDWriteFactory> pfact;
		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**) &pfact);

		ComPTR<IDWriteTextFormat> textFormat;
		std::wstring fontName(L"arial");
		(pfact->CreateTextFormat(fontName.c_str(), NULL, DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 24, L"en-us", & textFormat));

		textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			
		for(int i= 0; i < _countof(clrs); i++ )
		{
			
			// itemをメモリ上に描画
			rt->BeginDraw();
			{
				rt->Clear(D2DColor(D2RGB(255,255,255)));

				ComPTR<ID2D1SolidColorBrush> br;
				//
				rt->CreateSolidColorBrush(clrs[i], &br);

				FRectF rck(0,0,FSizeF(30,30));

				for(int k= 0; k <4; k++ )
				{
					rt->FillRectangle(rck, br);
					rck.Offset(50,0);
				}

				ComPTR<ID2D1SolidColorBrush> brblack;
				//
				rt->CreateSolidColorBrush(D2RGB(0,0,0), &brblack);

				//rt->DrawText(nm[i],wcslen(nm[i]), textFormat,FRectF(0,0,200,30),brblack);

				ComPTR<IDWriteTextLayout> tl;
				pfact->CreateTextLayout(nmstock[i],wcslen(nmstock[i]), textFormat, 200,30,&tl);
				rt->DrawTextLayout(FPointF(), tl, brblack);
			}
			auto hr = rt->EndDraw();
			_ASSERT(hr == S_OK);


			// 描画されたものをbitmap->streamに変換
			D2D1_RECT_U rcu={};

			rcu.right = 200;
			rcu.bottom = 30;

			ComPTR<ID2D1Bitmap> bmp, bmp2;
			ComPTR<IStream> sm;
			
			RenderTargetToBitmp(rt, rcu, &bmp );

			ComPTR<IStream> sm1;
			auto r = CreateStreamOnHGlobal(NULL,TRUE,&sm1 );

			BitmapToIStream(bmp, sm1);

			// bitmapをstreamから作り直し

			auto main_rt = D2DGetRenderTarget(hP1c);
			IStreamToBitmap(main_rt, sm1, &bmp2);


			// 完成したbitmapをlistboxへ追加
			D2DAddBitmapItem(hP2c1, bmp2);
		}			
	}
	




}

bool CreateBitmapListboxItem(ID2D1RenderTarget* main_rt,UINT w, UINT h, std::wstring titlenm, ID2D1Bitmap** pbmp)
{
	ComPTR<ID2D1RenderTarget> rt;
	
	// メモリ上のRnderTargetを作成
	if ( !CreateMemoryRenderTarget(w,h, &rt))
		return false;
			
			
	// itemの指定
	D2DColor clrs[] = {D2RGB(0,255,0),D2RGB(255,255,0),D2RGB(0,255,255)};
	

	// メモリ上のRnderTargetで使用するフォント作成
	ComPTR<IDWriteFactory> pfact;
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**) &pfact);

	ComPTR<IDWriteTextFormat> textFormat;
	std::wstring fontName(L"arial");
	(pfact->CreateTextFormat(fontName.c_str(), NULL, DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 24, L"en-us", & textFormat));

	textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		// itemをメモリ上に描画
		rt->BeginDraw();
		{
			rt->Clear(D2DColor(D2RGB(255,255,255)));

			ComPTR<ID2D1SolidColorBrush> br;
			//
			rt->CreateSolidColorBrush(clrs[0], &br);

			FRectF rck(0,0,FSizeF(h,h));

			D2D1_ELLIPSE maru = {};

			maru.point = FPointF(h/2,h/2);
			maru.radiusX = h/2;
			maru.radiusY = h/2;

			for(int k= 0; k <4; k++ )
			{
				rt->FillEllipse(maru, br);
				maru.point.x += 50;
			}

			ComPTR<ID2D1SolidColorBrush> brblack;
			//
			rt->CreateSolidColorBrush(D2RGB(0,0,0), &brblack);

			ComPTR<IDWriteTextLayout> tl;
			pfact->CreateTextLayout(titlenm.c_str(),titlenm.length(), textFormat, w,h,&tl);
			rt->DrawTextLayout(FPointF(), tl, brblack);
		}
		auto hr = rt->EndDraw();
		_ASSERT(hr == S_OK);


		// 描画されたものをbitmap->streamに変換
		D2D1_RECT_U rcu={};

		rcu.right = w;
		rcu.bottom = h;

		ComPTR<ID2D1Bitmap> bmp, bmp2;
		ComPTR<IStream> sm;
			
		RenderTargetToBitmp(rt, rcu, &bmp );

		ComPTR<IStream> sm1;
		auto r = CreateStreamOnHGlobal(NULL,TRUE,&sm1 );

		BitmapToIStream(bmp, sm1);

		// bitmapをstreamから作り直し

		
		IStreamToBitmap(main_rt, sm1, &bmp2);

		bmp2.AddRef();
		*pbmp = bmp2;

		return true;
}

void CreatePage2(UIHandle handle)
{
	auto h1 = D2DCreateTextbox(handle, FRectF(100,50,FSizeF(1000,300)), true, STAT_DEFAULT, L"#sql_input" );

	D2DSetText(h1, L"select * from kabu_data where cd='VTI';" );// sqlite_master;");

	auto b1 = D2DCreateButton(handle, FRectF(100,370, FSizeF(100,26)), STAT_DEFAULT, L"Run", SQL_RUN);
	
	
	auto h2 = D2DCreateTabControls(handle, FRectF(100,450,FSizeF(1000,300)), STAT_DEFAULT, L"#sql", -1 );

	auto h2_1 = D2DGetTab(h2,0);

	D2DSetTabText(h2_1, L"Result");

	auto h3 = D2DCreateSqlDataGrid(h2_1, FRectF(0,0,FSizeF(1000,300)), STAT_DEFAULT, L"#sql_output",-1 );

	auto h2_2 = D2DAddNewTab(h2,L"message");

	auto h2_2_1 = D2DCreateTextbox(h2_2, FRectF(0,0,FSizeF(1000,300)), true, STAT_DEFAULT, L"#sql_msg",-1, 2 ); // 2:read only

	//auto h2_3 = D2DAddNewTab(h2,L"test");
	//auto h2_3_1 = D2DCreateTextbox(h2_3, FRectF(0,0,FSizeF(1000,300)), true, STAT_DEFAULT, L"#sql_msg1",-1, 2 ); // 2:read only
	


}

#include "sqlite3pp.h"

void SqlCommandRun(UIHandleWin hwin)
{
	try
	{
		sqlite3pp::database db("stock.db");

		auto h1 = D2DGetControlFromName( hwin, L"#sql_input");

		auto cqry = D2DGetText(h1,true);

		std::string sql = W2A( std::wstring(cqry));

		sqlite3pp::query qry(db, sql.c_str());

		int colcnt = qry.column_count();

		std::wstringstream sm;
		for(int c = 0; c < colcnt; c++ )
		{
			if ( c!=0)
				sm << L"\t";

			sm << A2W(qry.column_name(c));			
		}
		sm << L"\n";
		
		int cnt=0;
		for(auto i = qry.begin(); i != qry.end(); i++ )
		{						
			for(int c = 0; c < colcnt; c++ )
			{
				int ctyp = (*i).column_type(c);
				
				if ( ctyp != 5 ) 
				{
					if ( c!=0)
						sm << L"\t";

					auto col = (*i).get<std::string>(c);
					sm << A2W(col);					
				}
			}
			sm << L"\n";
			cnt++;
		}

		D2DSetText(D2DGetControlFromName( hwin, L"#sql_output"), sm.str().c_str());


		WCHAR cb[64];
		StringCbPrintf(cb, 64, L"Row count is %d.", cnt );

		D2DSetText(D2DGetControlFromName( hwin, L"#sql_msg"), cb);
	}
	catch( sqlite3pp::database_error er)
	{
		auto s = A2W(er.what());

		D2DSetText(D2DGetControlFromName( hwin, L"#sql_msg"),  s.c_str());
	}

}