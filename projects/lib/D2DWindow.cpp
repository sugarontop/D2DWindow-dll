#include "pch.h"
#include "D2DWindow.h" 
#include "D2DWindowControl.h"
#include "D2DContext.h"
#include "D2DTextbox.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())


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

	cxt.CreateDeviceResources(hWnd_, rc.right, rc.bottom);

	size->height = (float)rc.bottom;
	size->width = (float)rc.right;

	return true;
}
bool D2DWindow::TSFInit(HWND hWnd)
{
	try
	{
		THR(CoCreateInstance(CLSID_TF_ThreadMgr, NULL, CLSCTX_INPROC_SERVER, IID_ITfThreadMgr2, (void**)&tsf.pThreadMgr));
		THR(tsf.pThreadMgr->Activate(&tsf.TfClientId));
		THR(tsf.pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&tsf.pKeystrokeMgr));
		THR(CoCreateInstance(CLSID_TF_DisplayAttributeMgr,	NULL, 	CLSCTX_INPROC_SERVER, IID_ITfDisplayAttributeMgr, (void**)&tsf.DisplayAttributeMgr));

		auto ctrl = new TSF::CTextEditorCtrl();
		ctrl->Create( hWnd, tsf.pThreadMgr, tsf.TfClientId );	

		tsf.ctrl = ctrl;

		return true;
	}
	catch( ... )
	{
		return false;
	}
}
void D2DWindow::TSFExit()
{
	auto k = (TSF::CTextEditorCtrl*)tsf.ctrl;
	delete k;
	tsf.ctrl = nullptr;

}

HRESULT D2DWindow::InnerWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	static MouseParam mp;
	HRESULT hr = 0;

	_ASSERT(message != WM_SIZE);

	switch( message )
	{
		case WM_MOUSEMOVE :
			mp.pt = FPointF(lParam);						
			lParam = (LPARAM)&mp;
		break;
		case WM_LBUTTONDOWN:
			mp.pt = FPointF(lParam);
			mp.ptprv = mp.pt;
			b.bRedraw = true;
			lParam = (LPARAM)&mp;
		break;
		case WM_LBUTTONUP:
			mp.pt = FPointF(lParam);
			mp.ptprv = mp.pt;
			b.bRedraw = true;
			lParam = (LPARAM)&mp;
		break;
		case WM_MOUSEWHEEL:
		{
			POINT pt;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);
			::ScreenToClient(b.hWnd, &pt);
			mp.pt = FPointF((float)pt.x, (float)pt.y);
			mp.fwKeys = GET_KEYSTATE_WPARAM(wParam);
			mp.zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			b.bRedraw = true;
			lParam = (LPARAM)&mp;
		}
		break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
		{
			b.bRedraw = true;
		}
		break;
		case WM_NOTIFY:
		{
			hr = ::SendMessage(b.hWnd,WM_NOTIFY,wParam,lParam);

		}
		break;
		
	}

	if ( hr == 0 )
		hr = top_control_->WndProc(b, message, wParam, lParam);

	if ( message == WM_MOUSEMOVE )
		mp.ptprv = mp.pt;


	return hr;
}

void D2DWindow::ForceWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
		case WM_SIZE :
		{
			RECT rc;
			::GetClientRect(b.hWnd, &rc);

			if (rc.left < rc.right && rc.top < rc.bottom)
			{
				cxt.OnSize(b.hWnd);
			}
		} 
		break;
		case WM_D2D_RESOURCES_UPDATE:
		{
			switch( wParam )
			{
				case 0:
				{
					cxt.OnDiscardGraphicsResources();
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
					cxt.OnRestructGraphicsResources();
				}
				break;

			}
		}
		break;
	}


	top_control_->ForceWndProc(b, message, wParam, lParam);
}
HRESULT D2DWindow::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( !post_message_queue_.empty() )
	{
		if (TryEnterCriticalSection(&message_lock_))
		{
			AppBase bb(hWnd_, nullptr);
			for(auto& it : post_message_queue_ )
				InnerWndProc(bb, it.msg, it.wp, it.lp);

			post_message_queue_.clear();
			LeaveCriticalSection(&message_lock_);
		}
	}

	return InnerWndProc(b, message, wParam, lParam);
}
LRESULT D2DWindow::SendMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	AppBase b(hWnd_, nullptr);
	return WndProc(b, msg, wp, lp);
}
LRESULT D2DWindow::PostMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	EnterCriticalSection( &message_lock_ );
	{
		MSG m(msg,wp,lp);
		post_message_queue_.push_back( m );
	}
	LeaveCriticalSection(&message_lock_);

	return 0;
}


class InnerMessageBox : public D2DControl
{
	public :
		InnerMessageBox()
		{

		}
		virtual const FRectF& GetRect() const { return rc_; }
		virtual void SetRect(const FRectF& rc) {rc_ = rc; }

		void ModalShow(LPCWSTR text )
		{
			APP.SetCapture(this);

			msg_ = text;


		}
		virtual void Draw(D2DContext& cxt) override
		{
			if ( IsVisible())
			{
				(*cxt)->FillRectangle( rc_, cxt.black_ );

				D2DMatrix mat(*cxt);

				mat.PushTransform();
				mat.Offset( rc_ );

				FRectF rc = rc_.ZeroRect();
				(*cxt)->DrawText(msg_.c_str(), msg_.length(), cxt.textformat_, rc, cxt.white_ );

				mat.PopTransform();
			}

		}

		virtual HRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			switch( message )
			{
				case WM_KEYDOWN:
				{
					auto key = 0xff & wParam;

					if ( key == VK_ESCAPE && APP.IsCapture(this) )
					{
						APP.ReleaseCapture();

						DestroyControl();
						return 1;
					}
				}
				break;
			}
			return ( APP.IsCapture(this) ? 1 : 0 );
		}

		FRectF rc_;
		wstring msg_;

};

int D2DWindow::MessageBox(LPCWSTR text)
{
	auto msgbox = std::shared_ptr<InnerMessageBox>(new InnerMessageBox());

	FRectF rc(0,0,300,100);
	msgbox->CreateControl(this, top_control_.get(), rc, STAT_VISIBLE, L"msgbox" );

	top_control_->Add( msgbox );

	msgbox->ModalShow(text);

	

	return 0;
}
