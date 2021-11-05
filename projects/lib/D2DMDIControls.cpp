#include "pch.h"
#include "D2DWindow.h" 
#include "D2DMDIControls.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())



D2DMDIFrame::D2DMDIFrame():largest_idx_(-1),top_(0),active_idx_(0)
{

}
D2DMDIFrame::~D2DMDIFrame()
{

}
void D2DMDIFrame::Draw(D2DContext& cxt)
{
	if (stat_ & STAT_VISIBLE)
	{
		//cxt.DFillRect(rc_, D2RGB(170,170,170));	
		D2DMatrix mat(*cxt);
		mat.PushTransform();
		mat.Offset(rc_);

		auto rc = rc_.ZeroRect();
		cxt.DFillRect(rc, D2RGB(170,170,170));	
		rc.Inflate(-2, -2);
		mat.Offset(rc);

		
		if ( !controls_.empty())
		{
			top_ = (UINT_PTR)controls_.begin()->get();

			for (auto it = controls_.rbegin(); it != controls_.rend(); it++ )
			{
				(*it)->Draw(cxt);
			
			}
		}
		else
		{
			top_ = 0;
		}
		

		mat.PopTransform();
	}
}


std::shared_ptr<D2DMDIChild> D2DMDIFrame::Add(const FRectF& rc, DWORD stat, LPCWSTR name, int id)
{
	short idx = (short)prvRect_.size();
	auto r = std::make_shared<D2DMDIChild>(idx);

	r->CreateControl( this->GetParent(), this, rc, stat, name, id);

	controls_.push_back(r);

	prvRect_[r->idx_]=r->GetRect();

	return r;
}
void D2DMDIFrame::SetTopMDIChild(short idx)
{
	int i = 0;

	auto kkk = APP.GetCapture();

	std::shared_ptr<D2DControl> temp;

	for(auto it=controls_.begin(); it < controls_.end(); it++ )
	{
		auto k = dynamic_cast<D2DMDIChild*>(it->get());

		if ( k )
		{
			if ( idx == k->idx_ )
			{
				temp = controls_[i];
				controls_.erase(it);
				break;
			}
			i++;
		}
	}


	if ( temp )
	{
		controls_.insert(controls_.begin(), temp);

		auto k = dynamic_cast<D2DMDIChild*>( temp.get());
		active_idx_ = k->idx_;
		


	}
}
D2DMDIChild* D2DMDIFrame::GetChild(short idx) const
{
	for(auto& it : controls_)
	{
		auto k = dynamic_cast<D2DMDIChild*>(it.get());

		if ( k && k->idx_ == idx )
			return k;
	}
	return nullptr;
}


HRESULT D2DMDIFrame::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = 0;

	switch(message )
	{
		case WM_D2D_CREATE :
			hr = 1;
		break;
		case WM_D2D_MDI_TOPCHANGE:
			
			if (wParam == (WPARAM)this)
			{
				short idx = (short)lParam;
				SetTopMDIChild( idx );
				b.bRedraw = true;
				hr = 1;
			}
		break;
		case WM_D2D_MDI_SIZE_LARGEST:
		{
			prvRect_.clear();
			for(auto& it : controls_)
			{
				auto k = dynamic_cast<D2DMDIChild*>(it.get());
				prvRect_[k->idx_] = it->GetRect();				
			}
						
			
			if ( active_idx_ > -1 )
			{
				short idx = active_idx_;


				auto k = GetChild(idx);
				
				{
					FRectF rc(rc_);

					rc.Inflate(-2,-2);

					k->SetRect(rc);

					k->SetMTYP(D2DMDIChild::MDI_FULLSIZE);

					largest_idx_ = idx;
					b.bRedraw = true;
				}
			}
			hr = 1;
		}
		break;
		case WM_D2D_MDI_SIZE_PRV:
		{
			auto k = GetChild(largest_idx_);			
			if ( k )
			{			
				largest_idx_ = -1;

				for(auto& it : controls_)
				{
					auto k = dynamic_cast<D2DMDIChild*>(it.get());
					it->SetRect(prvRect_[k->idx_]);				
				}

				k->SetMTYP(D2DMDIChild::MDI_MOVABLE);

				b.bRedraw = true;
			}
			hr = 1;
		}
		break;
		case WM_D2D_MDI_TILE_HORZ:
		{


		}
		break;

		case WM_D2D_ONCLOSE:
		{			
			// update acive_idx_;

			short prv_idx = active_idx_;			
			for(auto& it : controls_)
			{
				if ( it.get() == (D2DControl*)lParam)
				{
					auto k = dynamic_cast<D2DMDIChild*>(it.get());
					if ( k )
					{
						if ( k->idx_ == active_idx_ )
						{
							active_idx_ = prv_idx;
							break;
						}
						prv_idx = k->idx_;
					}
				}
			}	
		}
		break;



		case WM_SIZE:
		{
			FSizeF sz = *(FSizeF*)lParam;

			rc_.SetSize(sz);

			if ( largest_idx_ != -1 )
			{
				for(auto& it : controls_)
				{
					auto k = dynamic_cast<D2DMDIChild*>(it.get());

					if ( k && k->idx_ == largest_idx_ )
					{
						FRectF rc(rc_);

						rc.Inflate(-2,-2);

						k->SetRect(rc);

						k->SetMTYP(D2DMDIChild::MDI_FULLSIZE);

						break;
					}
				}

			}


		}
		break;
	}

	if ( active_idx_ > -1 )
	{
		auto k = GetChild(active_idx_);		

		if ( k )
			hr = k->WndProc(b, message, wParam, lParam);
	}

	if ( hr == 0 && (message == WM_LBUTTONDOWN || message == WM_D2D_ONCLOSE) )
		hr = D2DControls::WndProc(b,message,wParam,lParam);


	if ( WM_SIZE == message )
		hr = D2DControls::WndProc(b,message,wParam,lParam);

	return hr;	
}

// /////////////////////////////////////////////////////////////////
static void DrawTitlebar(D2DContext& cxt, const FRectF& rc, D2DMDIChild::TitleMouse tmd, LPCWSTR title);
HRESULT TitleBarMoouseProc(D2DControls* ctrl,AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
#define bar_h 30.0f

D2DMDIChild::D2DMDIChild():idx_(-1)
{
	tmd_ = TitleMouse::MOVE;
	mtyp_ = MTYP::MDI_MOVABLE;
}
D2DMDIChild::D2DMDIChild(short idx)
{
	idx_ = idx;
	tmd_ = TitleMouse::MOVE;
	mtyp_ = MTYP::MDI_MOVABLE;
}

D2DMDIChild::~D2DMDIChild()
{

}
void D2DMDIChild::Draw(D2DContext& cxt)
{
	if (stat_ & STAT_VISIBLE)
	{
		D2DMatrix mat(*cxt);
		mat_ = mat.PushTransform();
		mat.Offset(rc_);
		auto rc = rc_.ZeroRect();
		
		if ( (UINT_PTR)this == ((D2DMDIFrame*)parent_control_)->top_ )		
			cxt.DFillRect(rc, D2RGB(0, 0, 220));
		else
			cxt.DFillRect(rc, D2RGB(220, 220, 220));
		
		
		
		rc.Inflate(-2,-2);
		mat.Offset(rc);

		
		if (mtyp_ == MTYP::MDI_MOVABLE )
		{				
			FRectF rc2(rc.ZeroRect());
			cxt.DFillRect(rc2, D2RGB(220,220,220));				
			cxt.DFillRect(rc2.Inflate(-2,-2), D2RGB(230,230,230));


			DrawTitlebar(cxt, rc, tmd_, this->name_.c_str());
		}
		else if ( mtyp_ == MTYP::MDI_FULLSIZE )
		{			
			// without titlebar
			cxt.DFillRect(rc.ZeroRect(), D2RGB(220,220,220));

		}
		
		mat.Offset(0,bar_h);
		
		for (auto it = controls_.rbegin(); it != controls_.rend(); it++ )
			(*it)->Draw(cxt);

		mat.PopTransform();
	}
}

HRESULT D2DMDIChild::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT r = 0;

	switch(message )
	{
		case WM_D2D_CREATE :
			r = 1;
		break;
		case WM_D2D_ONCLOSE:
		{
			if ( (D2DControl*)lParam == this )
			{
				int a = 0;

				r = 1;
			}
		}
		break;
		//case WM_LBUTTONDOWN:
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		{
			if ( this->mtyp_ == MTYP::MDI_MOVABLE )
				r = TitleBarMoouseProc(this,b,message,wParam,lParam);
		}
		break;
		case WM_LBUTTONDOWN:
		{
			if ( this->mtyp_ == MTYP::MDI_MOVABLE )
			{
				r = TitleBarMoouseProc(this,b,message,wParam,lParam);

				if ( r != 0)
				{
					D2DMDIFrame* frame = dynamic_cast<D2DMDIFrame*>( parent_control_ );
					parent_window_->PostMessage(WM_D2D_MDI_TOPCHANGE, (WPARAM)frame, idx_);
				}
			}
		}
		break;
		
	}

	if ( r == 0 )
		r = D2DControls::WndProc(b,message,wParam,lParam);


	// Œã‚ë‚ÌD2DMDIChild‚É“§‚¯‚È‚¢‚½‚ß‚Ìˆ—
	if ( r == 0 && message == WM_LBUTTONDOWN )
	{
		MouseParam& mp = *(MouseParam*)lParam;
		FPointF pt = mat_.DPtoLP(mp.pt);

		if ( rc_.PtInRect( pt ))
			r = 1;
	}

	return r;	
}



static void DrawTitlebar(D2DContext& cxt, const FRectF& rc, D2DMDIChild::TitleMouse tmd, LPCWSTR title)
{
	FRectF rcbar(0,0,rc.Size().width, bar_h);
	cxt.DFillRect(rcbar, D2RGB(255,255,255));

	(*cxt)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

	D2DMatrix mat(*cxt);
	mat.PushTransform();
	mat.Offset(rc.Width()-45, 0); // width:45, three buttons


	FRectF rc2(0,0,45,27);
	if ( tmd == D2DMDIChild::TitleMouse::DESTROY )
		cxt.DFillRect(rc2, D2RGB(232,17,35));

	(*cxt)->DrawLine(FPointF(11+5,11), FPointF(21+5,21), cxt.black_);
	(*cxt)->DrawLine(FPointF(21+5,11), FPointF(11+5,21), cxt.black_);



	mat.Offset(-45, 0);

	if ( tmd == D2DMDIChild::TitleMouse::LARGE )
		cxt.DFillRect(rc2, D2RGB(229,229,229));

	(*cxt)->DrawRectangle(FRectF(11,11,21,21), cxt.black_);

	mat.Offset(-45, 0);

	if ( tmd == D2DMDIChild::TitleMouse::SMALL )
		cxt.DFillRect(rc2, D2RGB(229,229,229));

	(*cxt)->DrawRectangle(FRectF(11,15,21,15), cxt.black_);

	mat.PopTransform();

	(*cxt)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);


	(*cxt)->DrawText(title, wcslen(title), cxt.textformat_, rcbar, cxt.black_);

}
HRESULT TitleBarMoouseProc(D2DControls* ctrls,AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT r = 0;
	

	D2DMDIChild* obj = (D2DMDIChild*)ctrls;

	switch(message)
	{
	case WM_LBUTTONDOWN:
	{
		MouseParam* mp = (MouseParam*)lParam;
		auto pt = obj->Matrix().DPtoLP(mp->pt);

		auto rc = obj->GetRect();

		if ( rc.PtInRect(pt))
		{			
			rc.SetHeight(bar_h);
			if ( rc.PtInRect(pt))
			{
				auto tmd = obj->GetTMD();
				if (tmd == D2DMDIChild::TitleMouse::MOVE )
				{
					APP.SetCapture(obj);
				}
				else if ( tmd == D2DMDIChild::TitleMouse::DESTROY )
				{
					obj->DestroyControl();
				}
				else if ( tmd == D2DMDIChild::TitleMouse::LARGE )
				{					
					obj->GetParent()->PostMessage(WM_D2D_MDI_SIZE_LARGEST, (WPARAM)0, obj->GetIdx());
				}
				else if ( tmd == D2DMDIChild::TitleMouse::SMALL )
				{
					obj->GetParent()->PostMessage(WM_D2D_MDI_SIZE_PRV, (WPARAM)0, 0);
				}


				r = 1;
			}
		}


	}
	break;
	case WM_MOUSEMOVE:
	{
		auto h1 = APP.GetCapture();

		if (h1 == obj )
		{
			MouseParam* mp = (MouseParam*)lParam;
			auto pt = obj->Matrix().DPtoLP(mp->pt);
			auto ptprv = obj->Matrix().DPtoLP(mp->ptprv);
			auto rc = obj->GetRect();

			if ( obj->GetTMD() == D2DMDIChild::TitleMouse::MOVE )
			{
				rc.Offset( pt.x - ptprv.x, pt.y - ptprv.y );

				obj->SetRect(rc);

			}


			b.bRedraw = true;
			r = 1;
		}
		else
		{
			obj->SetTMD(D2DMDIChild::TitleMouse::MOVE);
			MouseParam* mp = (MouseParam*)lParam;
			auto pt = obj->Matrix().DPtoLP(mp->pt);
			auto rc = obj->GetRect();
			if ( rc.PtInRect(pt))
			{
				auto tmd = obj->GetTMD();
				FRectF rc1(rc.right-45*3, rc.top, rc.right, rc.top+30);
				if ( rc1.PtInRect(pt))
				{
					if ( pt.x < rc.right-45*2 )
						obj->SetTMD(D2DMDIChild::TitleMouse::SMALL);
					else if ( pt.x < rc.right-45 )
						obj->SetTMD(D2DMDIChild::TitleMouse::LARGE);
					else 
						obj->SetTMD(D2DMDIChild::TitleMouse::DESTROY);
					b.bRedraw = true;
				}
			}
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		auto h1 =  APP.GetCapture();

		if (h1 == obj )
		{
			APP.ReleaseCapture();

			r = 1;
		}


	}
	break;
	case WM_LBUTTONDBLCLK:
	{
		MouseParam* mp = (MouseParam*)lParam;
		auto pt = obj->Matrix().DPtoLP(mp->pt);
		auto rc = obj->GetRect();

		if ( rc.PtInRect(pt))
		{			
			rc.SetHeight(bar_h);
			if ( rc.PtInRect(pt))
			{
				//MDIStatusChange(obj);


				r = 1;
			}
		}
	}
	break;

	}



	return r;
}
