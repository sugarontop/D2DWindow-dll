
#include "pch.h"
#include "D2DWindow.h" 
#include "D2DWhiteWindow.h"

using namespace V6;



D2DWhiteWindow::D2DWhiteWindow()
{
	vscroll_x_ = 0;
	hscroll_x_ = 0;
}

void D2DWhiteWindow::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();

	mat.Offset(rc_);

	cxt.DText(FPointF(), this->name_.c_str(), D2RGB(170,170,170));


	mat.PushTransform();
	mat.Offset(-sch_->LogicalOffset(), -scv_->LogicalOffset());

	D2DControls::Draw(cxt);

	mat.PopTransform();

	mat.PushTransform();
	mat.Offset(vscroll_x_, 0 );
	scv_->Draw2(cxt);
	mat.PopTransform();

	mat.PushTransform();
	mat.Offset(hscroll_x_, rc_.Height()-BARW );
	sch_->Draw2(cxt);
	mat.PopTransform();


	mat.PopTransform();
}

HRESULT D2DWhiteWindow::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT r = 0;

	switch( message )
	{
		case WM_SIZE:
		{
			FSizeF& sz = *(FSizeF*)(lParam);

			//rc_.SetSize(sz);

			if ( controls_.size() == 3 )
			{

				auto crc = this->controls_[2]->GetRect(); // 0,1 is scrollbar, 2 is child


				vscroll_x_ = rc_.right-BARW;
				hscroll_x_ = rc_.left;

				scv_->SetMaxSize( crc.Height());
				sch_->SetMaxSize( crc.Width());
				sch_->SetSize(rc_.Size());
				scv_->SetSize(rc_.Size());

				auto sz = rc_.Size();
				this->controls_[2]->WndProc(b,WM_SIZE,0,(LPARAM)&sz);
			}

			return 0;
		}
		break;

	}


	if ( r == 0 )
		r = D2DControls::WndProc(b,message,wParam,lParam);




	return r;

}
void D2DWhiteWindow::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);
	rc_ = rc;


	FRectF rc1(0,0,BARW,100);
	auto scV = std::make_shared<D2DScrollbar>();
	scV->CreateControl(parent, this, rc1,STAT_ENABLE, NONAME );
	this->Add(scV);
	scv_ = scV;


	rc1.SetRect(0,0,100, BARW);
	auto scH = std::make_shared<D2DScrollbar>();
	scH->CreateControl(parent, this, rc1,STAT_DEFAULT, NONAME );
	this->Add(scH);
	sch_ = scH;

	vscroll_x_ = 0;
	hscroll_x_ = 0;

	scV->SetMaxSize(rc_.Height());
	scH->SetMaxSize(rc_.Width());
}

// ///////////////////////////////////////////////////////////////////
