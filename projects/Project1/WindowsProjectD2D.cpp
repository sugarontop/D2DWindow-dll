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

void DrawTabButton( D2DContext& cxt,  FSizeF tabbtn, LPCWSTR* pps,  int btncnt, int activeidx )
{
    FRectF rc(0,0,tabbtn);

    for(int i = 0; i < btncnt; i++ )
    {        
        LPCWSTR s = pps[i];

        if ( i == activeidx )
        {
            cxt.DDrawRect( rc, D2RGB(220,220,220), D2RGB(50, 50, 255));                
            (*cxt)->DrawText(s, wcslen(s), cxt.textformat_, rc.OffsetRect(8,3), cxt.white_ );
        }
        else 
        {
            cxt.DDrawRect( rc, D2RGB(220,220,220), D2RGB(255, 255, 255));                
            (*cxt)->DrawText(s, wcslen(s), cxt.textformat_, rc.OffsetRect(8,3), cxt.black_ );
        }

        rc.Offset(tabbtn.width, 0 );
    }
}

#define COMBOBOX_ID_1 10

void CreateControl(HWND hWnd)
{
    hwin = D2DCreateMainHWnd(hWnd, 14);
    
    auto root = D2DGetRootControls(hwin);

    FRectF rctextbox(10, 40, FSizeF(400, 700));
    UIHandle htextbox = D2DCreateTextbox(hwin, root, rctextbox, true, STAT_VISIBLE | STAT_ENABLE, L"textbox1");
    D2DSetText(htextbox, L"Hello world");


//    FRectF rccmb(500, 800, FSizeF(100, 26));
//    UIHandle cb1 = D2DCreateDropdownListbox(hwin, root, rccmb,  STAT_VISIBLE | STAT_ENABLE, L"comb1", COMBOBOX_ID_1);
//    D2DAddItem(cb1, 0, L"scale1.0");
//    D2DAddItem(cb1, 1, L"scale1.2");
//    D2DAddItem(cb1, 2,L"scale0.8");



    struct WhiteBoard
    {     
        DelegateDrawFunc f1;
        DelegateProcFunc f2;
        int typ;
        D2DMat mat;
    };
    struct CaptureObj1
    {
        FRectF rc;
        UIHandle cc1;
        UIHandle cc2;
        int active_idx;
        D2DMat mat;
        WhiteBoard wboard;
    };

    static CaptureObj1 obj;
    obj.active_idx = 0;
    obj.wboard.typ = 0;
    obj.wboard.f1 = [](LPVOID captureobj, D2DContext& cxt) {
     
        CaptureObj1* obj = (CaptureObj1*)captureobj;
        WhiteBoard& wb = obj->wboard;

            D2DMatrix mat(*cxt);
            mat.PushTransform();

            auto rc = obj->rc;
            wb.mat = mat.Offset(rc);
            obj->mat = wb.mat;
            {
                cxt.DDrawRect(rc.ZeroRect(), D2RGB(0,0,0), D2RGB(255, 255, 255));

                // draw top buttons

                FSizeF tabbtn(100,26 );
                LPCWSTR str[] = { L"tab1", L"tab2" };

                DrawTabButton( cxt, tabbtn, str, 2, obj->active_idx );

            }
            mat.PopTransform();
    };
    obj.wboard.f2 = [](LPVOID captureobj,AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)->LRESULT {

        CaptureObj1* obj = (CaptureObj1*)captureobj;
        LRESULT r = 0;
        switch( message )
        {
            case WM_LBUTTONDOWN:
            {
                MouseParam* mp = (MouseParam*)lParam;
                auto pt = obj->mat.DPtoLP(mp->pt);

                FRectF rcs[] = { FRectF(0,0,100,26), FRectF(100,0,200,26) };
                if ( rcs[0].PtInRect( pt ) )
                {
                    obj->active_idx = 0;
                    D2DSetStat(obj->cc2, 0);
                    D2DSetStat(obj->cc1, STAT_VISIBLE | STAT_ENABLE);

                    r = 1;
                }
                else if ( rcs[1].PtInRect( pt ) )
                {
                    obj->active_idx = 1;
                    D2DSetStat(obj->cc1, 0);
                    D2DSetStat(obj->cc2, STAT_VISIBLE | STAT_ENABLE);
                    r = 1;
                }
            }
            break;
        }        
        return r;
    };

    FRectF rc(500, 40, FSizeF(500, 700));
    auto whb2 = D2DCreateWhiteControls(&obj, obj.wboard.f1, obj.wboard.f2, hwin, root, rc, STAT_VISIBLE | STAT_ENABLE, L"whb2", 110);

    
    obj.rc = rc;

    obj.cc1 =  D2DCreateControls(hwin, whb2, FRectF(0, 0, 0, 0), STAT_VISIBLE | STAT_ENABLE, L"cs1", 112);
    obj.cc2 = D2DCreateControls(hwin, whb2, FRectF(0, 0, 0, 0), 0, L"cs2", 113);

    FRectF rc2(10, 40, FSizeF(400, 600));
    UIHandle x2 = D2DCreateTextbox(hwin, obj.cc2, rc2, true, STAT_VISIBLE | STAT_ENABLE, L"textbox2");
    D2DSetText(x2, L"Hello world1");


    D2DSetFont(x2, L"メイリオ", 10.0f);


}


static float scale = 1.0f;
void CopyPasteTEXT(HWND hWnd, UIHandle uh, bool copy);
#include "D2DSquarePaper.h"

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
			D2DInitail((INT_PTR)k );

            CreateControl(hWnd);
			D2DForceWndProc(hwin, app, WM_D2D_RESOURCES_UPDATE, 2, 0);
            return ::DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    
        case WM_SIZE:
        {
            D2DForceWndProc(hwin, app, message, wParam,lParam);
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
        case WM_KEYUP:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MOUSEWHEEL:
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

                if ( wParam == COMBOBOX_ID_1 )
                {   
                    D2DNMHDR& n = *(D2DNMHDR*)lParam;

                    int idx = n.prm1;

                    scale = 1.0f;
                    if ( idx == 1 ) scale = 1.2f;
                    else if ( idx == 2 ) scale = 0.8f;

                }


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
