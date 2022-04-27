// WindowsProjectD2D.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"
#include "D2DApp.h"
#include "D2DColor.h"

using namespace V6;

#pragma comment(lib,"D2DUI_1.lib") // $(OutDir)

UIHandleWin hwin;



void ClipboardCopyPasteText(HWND hWnd, UIHandle uh, bool copy);


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
    WNDCLASSEXW wcex;

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


void test( UIHandle h)
{
	//auto hc = D2DCreateSimpleControls(h, FRectF(0,0, FSizeF(1000,500)),STAT_DEFAULT, L"t" );

	D2DColor clr(D2RGB(170,170,170));

	//auto hchart = D2DCreateControlsWithScrollbar(hc,FRectF(), STAT_DEFAULT, L"td_chart_sc");
	//
	//auto hc1 = D2DCreateSimpleControls(hchart, FRectF(0,0, FSizeF(1100,600)),STAT_DEFAULT, L"td_chart" );
	//
	//D2DSendMessage(hc1, WM_D2D_SET_COLOR, COLOR_IDX_BACK,(LPARAM)&clr);
	//D2DSendMessage(hchart, WM_D2D_SET_SIZE, 5,0);





			auto hchart = D2DCreateControlsWithScrollbar(h,FRectF(0,0, FSizeF(1000,500)), STAT_DEFAULT, L"td_chart_sc");


			auto hc1 = D2DCreateSimpleControls(hchart, FRectF(0,0, FSizeF(1100,600)),STAT_DEFAULT, L"td_chart" );
	
			D2DSendMessage(hc1, WM_D2D_SET_COLOR, COLOR_IDX_BACK,(LPARAM)&clr);

			/*chart->CreateControl( ctrl2, FRectF(0,0, FSizeF(1100,600)), STAT_DEFAULT, NR(L"td_chart",nm) );
			ctrl2->Add(chart);
			
			base->chart = ctrl2;

			left_bar->chart_ = chart.get();
			top_bar->chart_ = chart.get();*/


			D2DSendMessage(hchart, WM_D2D_SET_SIZE, 3,0);



}


static void CreateControl(HWND hWnd)
{
    hwin = D2DCreateMainHWnd(hWnd, 14);
    
    auto root = D2DGetRootControls(hwin);

    FRectF rctextbox(100, 40, FSizeF(400, 700));
   // UIHandle htextbox = D2DCreateTextbox(root, rctextbox, true, STAT_DEFAULT, L"textbox1");
   // D2DSetText(htextbox, L"Hello world one\nHello world two\nHello world three\n");


	///////////////////////////////////////

	test(root);

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    AppBase app;
    app.hWnd = hWnd;
    LRESULT r = 0;

    switch (message)
    {    
        case WM_CREATE:
        {
            static std::shared_ptr<D2DApp> _app;
			_app = std::make_shared<D2DApp>(D2DApp());
			auto k = _app.get();
			D2DApp::SetD2DAppForDLL(k);
			D2DInstanceInitail((INT_PTR)k );

            CreateControl(hWnd);
			D2DForceWndProc(hwin, app, WM_D2D_RESOURCES_UPDATE, 2, 0); //<--OnRestructGraphicsResources(cxt,hWnd);
        }
        break;
    
        case WM_SIZE:
        {
            D2DForceWndProc(hwin, app, message, wParam,lParam);
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

                mat._11 = 1.0f;
                mat._22 = 1.0f;

                cxt->SetTransform(mat);
                cxt->Clear(D2RGB(255,255,255));

                D2DDraw(hwin, hWnd);

                auto hr = cxt->EndDraw();

                if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
                {
                    D2DForceWndProc(hwin, app, WM_D2D_RESOURCES_UPDATE, 0, 0);  //<--OnDiscardGraphicsResources(cxt);
                    D2DForceWndProc(hwin, app, WM_D2D_RESOURCES_UPDATE, 1, 0);  // create Context
                    D2DForceWndProc(hwin, app, WM_D2D_RESOURCES_UPDATE, 2, 0);   //<--OnRestructGraphicsResources(cxt,hWnd);
                }
                else
                {
                    D2DSwapChain(hwin, hWnd);
                }
            }
            EndPaint(hWnd, &ps);
			return 0;
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
                    ClipboardCopyPasteText(hWnd, c, true );
                    r =  1;
                }
                else if (wParam == 0x43 && bCtrl) // ctrl-C
                {
                    ClipboardCopyPasteText(hWnd, c, false);
                    r = 1;
                }
            }

            if ( r == 0)
                r = D2DDefWndProc(hwin, app, message, wParam,lParam);
        }
        break;
        case WM_KILLFOCUS:
		case WM_SETFOCUS:
        case WM_CHAR:
        {
            r =  D2DDefWndProc(hwin, app, message, wParam,lParam);
        }
        break;
		case WM_KEYUP:
        case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
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
			return 0;
        }
         break;

        
		 case WM_D2D_ONIME_ONOFF:
		 {
			D2DDefWndProc(hwin ,app, message, wParam, lParam);

		 }
		 break;
    }

	
	r = ::DefWindowProc(hWnd, message, wParam, lParam);

    if ( app.bRedraw )
        InvalidateRect(hWnd, NULL, FALSE);
    
    return r; 
}

void ClipboardCopyPasteText(HWND hWnd, UIHandle uh, bool bPaste )
{
   if (OpenClipboard(hWnd))
    {
        if (bPaste)
        {
            HANDLE h = GetClipboardData(CF_UNICODETEXT);
            LPCWSTR s1a = (LPCWSTR)GlobalLock(h);

            D2DInsertText(uh, s1a, wcslen(s1a), -1);
           
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
void V6::app_catch_throw( LRESULT hr, LPCWSTR msg, UINT line, LPCSTR fnm )
{

}