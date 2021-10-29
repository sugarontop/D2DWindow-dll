#include "pch.h"
#include "D2DStatic.h"
#include "D2DTextbox.h"
using namespace V6;

#define  APP (D2DApp::GetInstance())


D2DStatic::D2DStatic():editor_(nullptr),editable_(false)
{

}
void D2DStatic::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rc_ = rc;
	editor_ = nullptr;

	editable_  = true; // test
}

void D2DStatic::Draw(D2DContext& cxt)
{
	_ASSERT(text_);
	(*cxt)->DrawTextLayout(rc_.LeftTop(), text_, cxt.black_ );

	if ( editor_ )
		editor_->Draw(cxt);	
	
}
HRESULT D2DStatic::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT r = 0;
	
	if ( editable_ )
	{
		bool bl = false;
		if ( message == WM_LBUTTONDOWN && editor_ == nullptr )
		{

			editor_ = new D2DTextbox();
			editor_->CreateControl(parent_window_, parent_control_, IBridgeTSFInterface::SINGLELINE, rc_, STAT_DEFAULT, NONAME );
			parent_control_->Add( std::shared_ptr<D2DTextbox>(editor_) );	


			editor_->SetBackColor(D2RGBA(0,0,0,0));
			editor_->SetBorderColor(D2RGBA(0,0,0,0));

			editor_->SetText(str_.c_str(), str_.length() );
		}
		else if ( message == WM_LBUTTONDOWN && editor_ )
			bl = true;
			
		
		if ( editor_ )
			r = editor_->WndProc(b,message,wParam,lParam);

		if ( message == WM_RBUTTONDOWN )
		{
			
			if ( editor_ != APP.GetCapture() )
			{
				editor_->DestroyControl();

				editor_ = nullptr;
			}



		}
	}



	return r;
}

void D2DStatic::SetText( std::wstring str )
{
	str_ = str;
	text_ = nullptr;

	auto& cxt = parent_window_->cxt;

	auto sz = rc_.Size();
	cxt.CreateTextLayout(str.c_str(), sz, &text_);

}
