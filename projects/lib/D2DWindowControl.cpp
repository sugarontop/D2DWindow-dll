#include "pch.h"
#include "D2DWindow.h" 
#include "D2DWindowControl.h"
#include "D2D1UI_1.h"
#include "D2DDropdownListbox.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())

void D2DControl::CreateControl(D2DWindow* win, D2DControls* parent, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(win,parent, stat, name, local_id);
	SetRect(rc);
}

void D2DControl::Enable(bool bEnable)
{
	stat_ = (bEnable ? (stat_|STAT_ENABLE) : (stat_ &~STAT_ENABLE));
}
void D2DControl::Visible(bool bVisible)
{
	stat_ = (bVisible ? (stat_ | STAT_VISIBLE) : (stat_ & ~STAT_VISIBLE));
}
void D2DControl::InnerCreateWindow(D2DWindow* parent, D2DControls* pacontrol, int stat, LPCWSTR name, int controlid)
{
	parent_window_ = parent;
	parent_control_ = pacontrol;
	stat_ = stat;
	id_ = controlid;

	target_ = nullptr;

	if ( wcscmp(name,NONAME) != 0 && wcslen(name) > 0 )
	{
		std::wstring nm;
		if ( parent_control_ )
		{
			if ( name[0] != L'#')
			{
				nm = parent_control_->GetName();
				nm += L"@";
				nm += name;
			}
			else
				nm = name;

			parent_window_->name_map_[nm] = this;
			name_ = nm;
		}
		else			
		{
			parent_window_->name_map_[name] = this;
			name_ = name;
		}
	}
}
void D2DControl::DestroyControl()
{
	if ((stat_ & STAT_DEAD) == 0)
	{
		stat_ = STAT_DEAD;

		if (parent_control_)
		{
			auto p = parent_control_->Detach(this);
			parent_window_->death_objects_.push_back(p);
		}

		parent_control_->SendMesage(WM_D2D_ONCLOSE, 0, (LPARAM)this);
		parent_control_ = nullptr;

	}
}
void D2DControl::SetNewParent(D2DControls* newparent)
{
	auto me = parent_control_->Detach(this);

	parent_control_ = newparent;

	newparent->Add( me );
}
std::wstring D2DControl::GetLocalName() const
{
	int i=0;
	for( i=name_.length()-1; 0 < i; i-- )
		if ( name_[i] == L'@' )
			break;

	return name_.substr(i+1, name_.length()-(i+1));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

D2DControls::D2DControls()
{

}
D2DControls::~D2DControls()
{
	controls_.clear();
}
void D2DControls::CreateControl(D2DWindow* win, D2DControls* parent, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(win, parent, stat, name, local_id);

	rc_ = rc;
}

LRESULT D2DControls::SendMesage(UINT message, WPARAM wParam, LPARAM lParam)
{	
	AppBase a={};
	a.hWnd = GetParent()->GetHwnd();
	return WndProc(a,message,wParam,lParam);
}


LRESULT D2DControls::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{	
	return DefWndProc(b,message,wParam,lParam);
}
void D2DControls::ForceWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	for (auto& it  : controls_ )
		it->WndProc(b, message, wParam, lParam);
}
LRESULT D2DControls::InnerWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT hr = 0;
	for(auto& it : controls_ )
	{
		hr = it->WndProc(b,message,wParam,lParam);
		if ( hr != 0 )
				break;
	}
	return hr;
}
LRESULT D2DControls::DefWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{	
	LRESULT hr = 0;

	// captureは無視
	if ( WM_D2D_BROADCAST_MESSAGE_FIRST <= message || message == WM_TIMER)
	{
		hr = InnerWndProc(b,message,wParam,lParam);

		return hr;
	}

	auto capture = APP.GetCapture();
	if ( capture && GetParentControls() == capture )
	{
		capture = nullptr; // stop stackoverflow
	}

	if ( nullptr == capture || capture == this )
	{
		for(auto& it : controls_ )
		{
			if ( BITFLG2(it->GetStat(), STAT_ENABLE) )
			{
				hr = it->WndProc(b,message,wParam,lParam);
				if ( hr != 0 )
				{
					return hr;
				}
			}
		}


		if (message == WM_KEYDOWN)
		{
			auto key = 0xFF&wParam;
			if ( key == VK_ESCAPE && capture == this)
			{
				APP.ReleaseCapture();
				hr = 1;

			}
		}
	}
	else if ( capture != this )
	{
		hr = capture->WndProc(b, message, wParam, lParam);

		if ( hr==0 && message == WM_LBUTTONDOWN )
		{
			auto capture2 = APP.GetCapture();

			if ( capture2 == nullptr )
			{
				// captureの切り替え、次のオブジェクトへメッセージをまわす
				hr = DefWndProc(b,message,wParam,lParam);
			}
		}
		
	}

	return hr;
}
void D2DControls::Add(std::shared_ptr<D2DControl> p)
{

	_ASSERT(this== p->GetParentControls());

	controls_.push_back( p);

	auto nm = p->GetName();

	if ( nm != NONAME )
		parent_window_->name_map_[nm] = p.get();

}

D2DControl* D2DControls::SetFirstControl(D2DControl* p, bool blast)
{
	auto target = controls_.end();

	auto prev = controls_.begin()->get();

	for(auto it = controls_.begin(); it != controls_.end(); it++ )
	{
		if ( (*it).get() == p )
		{
			target = it;
			break;
		}
	}

	if ( target != controls_.end() )
	{
		std::shared_ptr<D2DControl> copy_target = *target;
		
		controls_.erase(target);

		if ( blast )
			controls_.insert(controls_.end(), copy_target);
		else
			controls_.insert(controls_.begin(), copy_target);
	}

	return prev;
}


D2DControl* D2DControls::GetControl( std::wstring name )
{
	return dynamic_cast<D2DControl*>( parent_window_->name_map_[name]);
}
D2DControl* D2DControls::GetControlFromID( int id ) const
{
	for(auto it = controls_.begin(); it != controls_.end(); it++)
	{
		if ( id == it->get()->GetID() )
		{
			return it->get();
		}
		auto ls = dynamic_cast<D2DControls*>(it->get());

		for(auto it2 = ls->controls_.begin(); it2 != ls->controls_.end(); it2++)
		{
			if ( id == it2->get()->GetID())
			{
				return it2->get();
			}
		}
	}
	return nullptr;
}


void D2DControls::InnerDraw(D2DContext& cxt)
{
	auto capture = APP.GetCapture();
	int cn = APP.Count();

	auto vcapture = capture;

	if (cn > 1)
	{
		vcapture = APP.GetCapture2();
	}

	if (capture == nullptr || vcapture == this)
	{
		for (auto it = controls_.rbegin(); it != controls_.rend(); it++) 
			(*it)->Draw(cxt);
	}
	else
	{ 
		bool bl = false;

		for (auto it = controls_.rbegin(); it != controls_.rend(); it++)
		{
			if (vcapture != it->get())
				(*it)->Draw(cxt);
			else
			{
				bl = true;
			}
		}

		if ( bl &&  vcapture )
		{
			//auto x = APP.Pop2();
			static_cast<D2DControl*>(vcapture)->Draw(cxt);

			//if ( x )
				//APP.Push2(x);
		}
	}
}
void D2DControls::ListUp(std::vector<ControlMapItem>& ar, int* row, int* col)
{
	if ( BITFLG(STAT_VISIBLE))
	{
	
		ControlMapItem it;
		it.p = this;
		it.row = *row;
		it.col = *col;

	
		ar.push_back(it);

		++(*row);
		//*col=1;	
		for(auto& itk : controls_)
		{
			itk->ListUp(ar, row, col);

			if ( dynamic_cast<D2DControls*>(itk.get()))
			{
				++(*row);

				if ( !dynamic_cast<D2DDropdownListbox*>(itk.get()))
					++(*col);
			}
		}

		*col=it.col;
		*row=it.row;
	}
}
void D2DControl::ListUp(std::vector<ControlMapItem>& ar, int* row, int* col)
{
	if ( BITFLG(STAT_VISIBLE))
	{	
		ControlMapItem it;
		it.p = this;
		it.row = *row;
		it.col = *col;
		
		ar.push_back(it);
	
		++(*row);
	}

}



void D2DControls::Draw(D2DContext& cxt)
{

#ifdef _DEBUG
	if ( stat_ & STAT_DEBUG )
	{
		D2DMatrix mat(*cxt);
		mat.PushTransform();
		mat.Offset( rc_ );
		

		cxt.DFillRect(rc_.ZeroRect(), D2RGB(5,5,250));

		InnerDraw(cxt);

		mat.PopTransform();
	}
#endif


	if (stat_ & STAT_VISIBLE)
	{
		D2DMatrix mat(*cxt);
		mat.PushTransform();
		mat.Offset( rc_ );
		

		

		InnerDraw(cxt);

		mat.PopTransform();		
	}

}
std::shared_ptr<D2DControl> D2DControls::Detach(D2DControl* target)
{
	for (auto it = controls_.begin(); it != controls_.end(); ++it)
	{
		if ((*it).get() == target)
		{
			std::shared_ptr<D2DControl> ret = (*it);

			controls_.erase(it);

			//ret->parent_control_ = NULL;

			return ret;
		}
	}

	_ASSERT(false); // parent_controlにAddされてない

	return NULL;
}
void D2DControls::ResourceUpdate(bool bCreate, D2DContext& cxt)
{
	for(auto& it : controls_ )
		it->ResourceUpdate(bCreate, cxt);
}

// ////////////////////////////////////////////////

ComPTR<ID2D1SolidColorBrush> V6::CreateBrush1(D2DContext& cxt, byte r,byte g,byte b)
{
	ComPTR<ID2D1SolidColorBrush> br;
	cxt.CreateBrush(D2RGB(r,g,b), &br);	
	return br;
}


