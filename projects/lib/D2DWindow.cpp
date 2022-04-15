#include "pch.h"
#include "D2DWindow.h" 
#include "D2DWindowControl.h"
#include "D2DContext.h"
#include "D2DTextbox.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())

#undef THR
static void THR( HRESULT x){ if (x!= S_OK) throw x; }

//int D2DWindow::MessageBox(const FRectF& rc, LPCWSTR text, LPCWSTR title) <-- D2Dbutton.cpp

D2DWindow::D2DWindow()
{
	::InitializeCriticalSection(&message_lock_);
}
D2DWindow::D2DWindow(HWND hwnd) :hWnd_(hwnd)
{
	::InitializeCriticalSection(&message_lock_);

	TSFInit(hWnd_);
}
D2DWindow::~D2DWindow()
{
	::DeleteCriticalSection(&message_lock_);
	TSFExit();
}
bool D2DWindow::CreateResource(FSizeF* size)
{
	RECT rc;
	::GetClientRect(hWnd_, &rc);

	cxt_.CreateDeviceResources(hWnd_, rc.right, rc.bottom);

	size->height = (float)rc.bottom;
	size->width = (float)rc.right;

	return true;
}
bool D2DWindow::TSFInit(HWND hWnd)
{
	try
	{
		THR(CoCreateInstance(CLSID_TF_ThreadMgr, NULL, CLSCTX_INPROC_SERVER, IID_ITfThreadMgr2, (void**)&tsf_.pThreadMgr));
		THR(tsf_.pThreadMgr->Activate(&tsf_.TfClientId));
		THR(tsf_.pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&tsf_.pKeystrokeMgr));
		THR(CoCreateInstance(CLSID_TF_DisplayAttributeMgr,	NULL, 	CLSCTX_INPROC_SERVER, IID_ITfDisplayAttributeMgr, (void**)&tsf_.DisplayAttributeMgr));

		auto ctrl = new TSF::CTextEditorCtrl();
		ctrl->Create( hWnd, tsf_.pThreadMgr, tsf_.TfClientId );	

		tsf_.ctrl = ctrl;

		return true;
	}
	catch( HRESULT hr )
	{
		return false;
	}
}
void D2DWindow::TSFExit()
{
	auto k = (TSF::CTextEditorCtrl*)tsf_.ctrl;
	delete k;
	tsf_.ctrl = nullptr;

}



//FRectF D2DWindow::GetClientRect() const
//{
//	RECT rc;
//	::GetClientRect(hWnd_, &rc);
//	return FRectF( (float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom );
//}

void D2DWindow::ForceWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
		case WM_SIZE :
		{
			auto rc = GetClientRect();

			if (rc.left < rc.right && rc.top < rc.bottom)
			{
				cxt_.OnSize(b.hWnd);
			}
		} 
		break;
		case WM_D2D_RESOURCES_UPDATE:
		{
			switch( wParam )
			{
				case 0:
				{
					cxt_.OnDiscardGraphicsResources();
					ResourceUpdate(false);
					top_control_->ResourceUpdate(false, cxt_);
					return;
				}
				break;
				case 1:
				{
					FSizeF sz;
					auto bl = CreateResource(&sz);
					_ASSERT( bl );
					return;
				}
				break;
				case 2:
				{
					cxt_.OnRestructGraphicsResources();
					ResourceUpdate(true);
					top_control_->ResourceUpdate(true, cxt_);
					return;
				}
				break;

			}
		}
		break;
	}

	top_control_->ForceWndProc(b, message, wParam, lParam);
}
void D2DWindow::ResourceUpdate(bool bCreate)
{

	
}

//LRESULT D2DWindow::SendMessage(UINT msg, WPARAM wp, LPARAM lp)
//{
//	AppBase b(hWnd_, nullptr);
//	return WndProc(b, msg, wp, lp);
//}
//LRESULT D2DWindow::PostMessage(UINT msg, WPARAM wp, LPARAM lp)
//{
//	EnterCriticalSection( &message_lock_ );
//	{
//		MSG m(msg,wp,lp);
//		post_message_queue_.push_back( m );
//	}
//	LeaveCriticalSection(&message_lock_);
//
//	return 0;
//}


