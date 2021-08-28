

## in WindowsProjectD2D.cpp

## Windows(HWND) base source code,  4lines  
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,  _In_opt_ HINSTANCE hPrevInstance,  _In_ LPWSTR    lpCmdLine,  _In_ int   nCmdShow);  
ATOM MyRegisterClass(HINSTANCE hInstance);  
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);  
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);  

## make UI Control  
void CreateControl(HWND hWnd);  
- textbox
- tabcontrol
    - page1: (empty)
    - page2: textbox
## Clipboard,text copy or paste
void CopyPasteTEXT(HWND hWnd, UIHandle uh, bool bPaste );  

## tab control button
void DrawTabButton( D2DContext& cxt,  FSizeF tabbtn, LPCWSTR* pps,  int btncnt, int activeidx );  

