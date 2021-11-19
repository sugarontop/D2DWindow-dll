// WindowsProjectD2D.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"

using namespace V6;

#pragma comment(lib,"D2DUI_1.lib") // $(OutDir)

UIHandleWin hwin;

//////////////////////////////////////

#define MAX_LOADSTRING 100

#define  APP (D2DApp::GetInstance())

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	auto thid = ::GetCurrentThreadId();

	_tsetlocale(LC_ALL, L"Japanese_Japan.932");
	 
   

    // グローバル文字列を初期化する3
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECTD2D, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if ( 0!=::CoInitialize(0) )
       {
        return FALSE;
       }
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF|_CRTDBG_LEAK_CHECK_DF);


    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECTD2D));

    MSG msg;



    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex={};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECTD2D));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = NULL;
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECTD2D);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      300, 50, 1100, 1400, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//

//void DrawTabButton( D2DContext& cxt,  FSizeF tabbtn, LPCWSTR* pps,  int btncnt, int activeidx )
//{
//    FRectF rc(0,0,tabbtn);
//
//    for(int i = 0; i < btncnt; i++ )
//    {        
//        LPCWSTR s = pps[i];
//
//        if ( i == activeidx )
//        {
//            cxt.DDrawRect( rc, D2RGB(220,220,220), D2RGB(50, 50, 255));                
//            (*cxt)->DrawText(s, (UINT)wcslen(s), cxt.textformat_, rc.OffsetRect(8,3), cxt.white_ );
//        }
//        else 
//        {
//            cxt.DDrawRect( rc, D2RGB(220,220,220), D2RGB(255, 255, 255));                
//            (*cxt)->DrawText(s, (UINT)wcslen(s), cxt.textformat_, rc.OffsetRect(8,3), cxt.black_ );
//        }
//
//        rc.Offset(tabbtn.width, 0 );
//    }
//}

//#define COMBOBOX_ID_1 10
//void CreateControl0(UIHandleWin hwin, UIHandle hcs);
//void CreateControl1(UIHandleWin hwin, UIHandle hcs);
//void CreateControl2(UIHandleWin hwin, UIHandle hcs);
//bool LoadTextFile( LPCWSTR fnm, std::wstring* str );
//bool SaveTextFile( LPCWSTR fnm, LPCWSTR str );

//void CreateControl(HWND hWnd)
//{
//    hwin = D2DCreateMainHWnd(hWnd, 14);
//    
//    auto root = D2DGetRootControls(hwin);
//
//    //FRectF rctextbox(10, 40, FSizeF(400, 700));
//    //UIHandle htextbox = D2DCreateTextbox(hwin, root, rctextbox, true, STAT_VISIBLE | STAT_ENABLE, L"textbox1");
//    //D2DSetText(htextbox, L"Hello world");
//
//
//
//
//    struct WhiteBoard
//    {     
//        DelegateDrawFunc f1;
//        DelegateProcFunc f2;
//        int typ;
//        D2DMat mat;
//    };
//    struct CaptureObj1
//    {
//        FRectF rc;
//        UIHandle page[3];
//        int active_idx;
//        D2DMat mat;
//        WhiteBoard wboard;
//		UIHandle me;
//    };
//
//    static CaptureObj1 obj;
//    obj.active_idx = 0;
//    obj.wboard.typ = 0;
//    obj.wboard.f1 = [](LPVOID captureobj, D2DContext& cxt) {
//     
//        CaptureObj1* obj = (CaptureObj1*)captureobj;
//        WhiteBoard& wb = obj->wboard;
//
//            D2DMatrix mat(*cxt);
//            mat.PushTransform();
//
//			FRectF rc = obj->rc;
//            wb.mat = mat.Offset(rc);
//            obj->mat = wb.mat;
//            {
//                cxt.DDrawRect(rc.ZeroRect(), D2RGB(0,0,0), D2RGB(255, 255, 255));
//
//                // draw top buttons
//
//                FSizeF tabbtn(100,26 );
//                LPCWSTR str[] = { L"tab1", L"tab2", L"tag3" };
//
//                DrawTabButton( cxt, tabbtn, str, 3, obj->active_idx );
//
//            }
//            mat.PopTransform();
//    };
//    obj.wboard.f2 = [](LPVOID captureobj,AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)->HRESULT {
//
//        CaptureObj1* obj = (CaptureObj1*)captureobj;
//        HRESULT r = 0;
//        switch( message )
//        {
//			case WM_D2D_CREATE:
//			{
//				obj->me = *(UIHandle*)lParam;
//
//
//				
//
//				r = 1;
//			}
//			break;
//			case WM_LBUTTONDOWN:
//            {
//                MouseParam* mp = (MouseParam*)lParam;
//                auto pt = obj->mat.DPtoLP(mp->pt);
//
//                FRectF rcs[] = { FRectF(0,0,100,26), FRectF(100,0,200,26),FRectF(200,0,300,26) };
//                if ( rcs[0].PtInRect( pt ) )
//                {
//                    obj->active_idx = 0;                   
//                    D2DSetStat(obj->page[0], STAT_VISIBLE | STAT_ENABLE);
//                    D2DSetStat(obj->page[1], 0);
//                    D2DSetStat(obj->page[2], 0);
//
//                    r = 1;
//                }
//                else if ( rcs[1].PtInRect( pt ) )
//                {
//                    obj->active_idx = 1;
//                    D2DSetStat(obj->page[0], 0);
//                    D2DSetStat(obj->page[1], STAT_VISIBLE | STAT_ENABLE);
//                    D2DSetStat(obj->page[2], 0);
//                    r = 1;
//                }
//                else if (rcs[2].PtInRect(pt))
//                {
//                    obj->active_idx = 2;
//                    D2DSetStat(obj->page[0], 0);
//                    D2DSetStat(obj->page[1], 0);
//                    D2DSetStat(obj->page[2], STAT_VISIBLE | STAT_ENABLE);
//                    r = 1;
//                }
//            }
//            break;
//			case WM_NOTIFY:
//			{
//				if ( wParam == 400 )
//				{
//					
//					auto htx = D2DGetControlFromName(hwin, L"textbox_fnm");
//					BSTR fnm = D2DGetText(htx,true);
//					
//					// load
//					std::wstring str;
//					if ( LoadTextFile( fnm , &str ) )
//					{						
//						
//						auto h2 = D2DGetControlFromName(hwin, L"textbox200");
//
//						D2DSetText(h2, str.c_str());
//					}
//					r = 1;
//
//					::SysFreeString(fnm);
//				}
//				else if ( wParam == 401 )
//				{
//					// save
//					auto h2 = D2DGetControlFromName(hwin, L"textbox200");
//
//					BSTR bs = D2DGetText(h2, true);
//
//					SaveTextFile(L"script.txt", bs );
//
//					::SysFreeString(bs);
//					r = 1;
//				}
//				else if ( wParam == 2002 )
//				{
//					int cx,cy;					
//					b.GetClientRect(&cx,&cy);
//					FRectF rc(0,0,FSizeF(450,200));
//					FPointF pt = rc.CenterPt();
//					rc.Offset( -pt.x+cx/2, -pt.y+cy/2 );
//
//
//					D2DMessageBox(hwin, rc, L"sample", L"This is a message box. push Escape key.");
//					
//					r = 1;
//				}
//			}
//			break;
//			case WM_SIZE :
//			{
//				// 画面内最大化
//				UINT width = LOWORD(lParam);
//				UINT height = HIWORD(lParam);
//
//				obj->rc.left = obj->rc.top = 0;
//				obj->rc.right = (float)width;
//				obj->rc.bottom = (float)height;
//				b.bRedraw = true;
//
//				D2DSetRect(obj->me, obj->rc);
//
//
//				D2DSetRect( obj->page[0], obj->rc );
//				D2DSetRect( obj->page[1], obj->rc );
//				D2DSetRect( obj->page[2], obj->rc );
//
//
//			}
//			break;
//        }        
//        return r;
//    };
//
//	RECT rcclient;
//	::GetClientRect(hWnd,&rcclient);
//
//    FRectF rc(0, 0, FSizeF( (float)rcclient.right, (float)rcclient.bottom));
//    auto whb2 = D2DCreateWhiteControls(&obj, obj.wboard.f1, obj.wboard.f2, hwin, root, rc, STAT_VISIBLE | STAT_ENABLE, L"whb2000", 110);
//    
//    obj.rc = rc;
//
//    obj.page[0] =  D2DCreateControls(hwin, whb2, FRectF(0, 0, rc.GetSize()), STAT_VISIBLE | STAT_ENABLE, L"tab1", 112);
//    obj.page[1] = D2DCreateControls(hwin, whb2, FRectF(0, 0, rc.GetSize()), 0, L"tab2", 113);
//    obj.page[2] = D2DCreateControls(hwin, whb2, FRectF(0, 0, rc.GetSize()), 0, L"tab3", 113);
//
//	CreateControl0( hwin, obj.page[0]);
//
//	CreateControl1( hwin, obj.page[1]);
//
//    CreateControl2( hwin, obj.page[2]);
//
//
//}


#include "D2DControls_with_Scrollbar.h"
#include "D2DSquarePaper.h"

void CreateControl(HWND hWnd);
//{
//	hwin = D2DCreateMainHWnd(hWnd, 14,0);    
//    auto root = D2DGetRootControls(hwin);
//
//	auto center = std::make_shared<D2DControls_with_Scrollbar>();
//	auto ctrl =  (D2DControls*)root.p;
//	center->CreateControl((D2DWindow*)hwin.p, ctrl, FRectF(0,0,1000,1000), STAT_DEFAULT, NONAME);
//	ctrl->Add(center);
//	
//	//auto center = dynamic_cast<D2DControls_with_Scrollbar*>(  (D2DControls*)root.p);
//
//	UIHandle hctrls;
//	hctrls.p = center.get();
//	auto hd = D2DCreateSquarePaper(hwin, hctrls,  FRectF(0,0,9000,9000), STAT_DEFAULT, NONAME,-1);
//
//
//	auto p1 = center->GetMainControls();
//
//
//}

static float scale = 1.0f;
void CopyPasteTEXT(HWND hWnd, UIHandle uh, bool copy);


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    AppBase app;
    app.hWnd = hWnd;
    LRESULT r = 0;

    switch (message)
    {    
        case WM_CREATE:
        {
            CreateControl(hWnd);
            return ::DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    
        case WM_SIZE:
        {
			FRectF rc(0,0,(float)LOWORD(lParam), (float)HIWORD(lParam));          
			D2DForceWndProc(hwin, app, message, 0, (LPARAM)&rc);
            return ::DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
        case WM_PAINT:
        {
                auto cxt = D2DGetDeviceContext(hwin);

                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);
                {
                   cxt->BeginDraw();
                    D2D1_MATRIX_3X2_F mat = {0};

                    mat._11 = scale;
                    mat._22 = scale;

                    cxt->SetTransform(mat);
                    cxt->Clear(D2RGB(255,255,255));

                    D2DDraw(hwin, hWnd);

                    auto hr = cxt->EndDraw();

                    if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
                    {
                        D2DForceWndProc(hwin, app, WM_D2D_RESOURCES_UPDATE, 0, 0);  //<--OnDiscardGraphicsResources(cxt);
                        D2DForceWndProc(hwin, app, WM_D2D_RESOURCES_UPDATE, 1, 0);  // create resource
                        D2DForceWndProc(hwin, app, WM_D2D_RESOURCES_UPDATE, 2, 0);   //<--OnRestructGraphicsResources(cxt,hWnd);
                    }
                    else
                    {
                        D2DSwapChain(hwin, hWnd);
                    }
                }
                EndPaint(hWnd, &ps);
            }
            break;
        case WM_KEYDOWN:
        {
            bool bShift   = ((GetKeyState(VK_SHIFT)& 0x80) != 0);       
            bool bCtrl = ((GetKeyState(VK_CONTROL) & 0x80) != 0);

            auto c = D2DGetCapture();

            if ( c.typ == TYP_TEXTBOX)
            {                
                if (wParam == 0x56 && bCtrl) // ctrl-V
                {
                    CopyPasteTEXT(hWnd, c, true );
                    r =  1;
                }
                else if (wParam == 0x43 && bCtrl) // ctrl-C
                {
                    CopyPasteTEXT(hWnd, c, false);
                    r = 1;
                }
            }

            if ( r == 0)
                r = D2DDefWndProc(hwin, app, message, wParam,lParam);
        }
        break;
        case WM_CHAR:
        {
            r =  D2DDefWndProc(hwin, app, message, wParam,lParam);
        }
        break;
        
        case WM_LBUTTONDOWN:
			r = D2DDefWndProc(hwin, app, message, wParam,lParam);
		break;
        case WM_LBUTTONUP:
			r = D2DDefWndProc(hwin, app, message, wParam,lParam);
		break;
		case WM_KEYUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MOUSEWHEEL:
		case WM_LBUTTONDBLCLK:
        {
            r = D2DDefWndProc(hwin, app, message, wParam,lParam);
        }
        break;

        case WM_MOUSEMOVE:
        {
            r =  D2DDefWndProc(hwin, app, message, wParam,lParam);
        }
        break;

        case WM_DESTROY:
        {        
            D2DDestroyWindow(hwin);
            PostQuitMessage(0);
        }
         break;

         case WM_NOTIFY:
         {
                // child control events         

               

         }
         break;

        default:
       
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    if ( app.bRedraw )
        InvalidateRect(hWnd, NULL, FALSE);
    
    return r; 
}

void CopyPasteTEXT(HWND hWnd, UIHandle uh, bool bPaste )
{
    if (OpenClipboard(hWnd))
    {
        if (bPaste)
        {
            HANDLE h = GetClipboardData(CF_UNICODETEXT);
            LPCWSTR s1a = (LPCWSTR)GlobalLock(h);

			if ( s1a )
				D2DInsertText(uh, s1a, (UINT)wcslen(s1a), -1);
           
            GlobalUnlock(h);
        }
        else
        {
            BSTR bs = D2DGetText(uh, false );
            int len = ::SysStringLen(bs);
            if (len)
            {
                HANDLE h = GlobalAlloc(GMEM_DDESHARE, (len + 1) * sizeof(WCHAR));
                if (h)
                {
                    LPWSTR buf = (LPWSTR)GlobalLock(h);
                    wcscpy_s(buf, (len + 1), bs);
                    GlobalUnlock(h);
                    EmptyClipboard();
                    SetClipboardData(CF_UNICODETEXT, h);
                }
            }

        }
        ::CloseClipboard();
    }
}
#include <fstream>
bool LoadTextFile( LPCWSTR fnm, std::wstring* str )
{
	std::wifstream fs;
	std::wstring& s = *str;
	fs.open(fnm, std::ios::in);
	int i = 0;
	if ( fs ) 
	{
		while( !fs.eof())
		{
			std::wstring xx;
			std::getline( fs, xx );	

			if ( i++ != 0)
				s += '\n';
			s += xx;
			
		}
		return true;
	}
	return false;
}
bool SaveTextFile( LPCWSTR fnm, LPCWSTR str )
{
	std::wofstream fs;
	fs.open( fnm, std::ios::trunc);

	if ( fs )
	{
		fs.write(str, wcslen(str));
		fs.close();
		return true;
	}
	return false;
}