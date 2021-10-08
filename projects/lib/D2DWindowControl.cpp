#include "pch.h"
#include "D2DWindow.h" 
#include "D2DWindowControl.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())

std::map<std::wstring, D2DControl*> D2DControls::control_map_;

D2DControl::D2DControl()
{

}
D2DControl::~D2DControl()
{
	
}

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
	name_ = name;
	id_ = controlid;

	target_ = nullptr;

	
}
void D2DControl::DestroyControl()
{
	if ((stat_ & STAT_DEAD) == 0)
	{
		//TRACE( L"me=%x parent=%x HWND=%x %d\n", this, parent_, parent_->hWnd_, (IsWindow(parent_->hWnd_)?1:0) );
		//SendMessage(WM_D2D_DESTROY_CONTROL, 0, (LPARAM)this);


		stat_ = STAT_DEAD;

		if (parent_control_)
		{
			auto p = parent_control_->Detach(this);
			parent_window_->death_objects_.push_back(p);
		}

		//if (parent_->GetCapture() == this)
		//	parent_->ReleaseCapture(); // 1階層目だけはcheck


		//if (parent_control_)
		//{
		//	auto p = parent_control_->Detach(this);

		//	
		//}

		//parent_->chandle_.DeleteControlHandle(chdl_);
	}
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

	
}
HRESULT D2DControls::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWndProc(b,message,wParam,lParam);
}
void D2DControls::ForceWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	for (auto& it  : controls_ )
		it->WndProc(b, message, wParam, lParam);
}
HRESULT D2DControls::DefWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto capture = APP.GetCapture();
	HRESULT hr = 0;
	if ( nullptr == capture )
	{
		for(auto& it : controls_ )
		{
			if ( it->GetStat() & STAT_ENABLE )
			{
				hr = it->WndProc(b,message,wParam,lParam);
				if ( hr != 0 )
				{
					/*if ( message == WM_LBUTTONDOWN )
					{
						auto x = (*it);

						x->WndProc(b,message,wParam,lParam);

						int a = 0;

					}*/
				
					return hr;
				}
			}
		}
	}
	else if ( capture != this )
	{
		hr = capture->WndProc(b, message, wParam, lParam);

		if ( message == WM_LBUTTONDOWN )
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
	controls_.push_back( p);

	control_map_[p->GetName()] = p.get();

}
D2DControl* D2DControls::GetControl( std::wstring name )
{
	return dynamic_cast<D2DControl*>( control_map_[name]);
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
	if (capture == nullptr || capture == this)
	{
		for (auto it = controls_.rbegin(); it != controls_.rend(); it++) 
			(*it)->Draw(cxt);
	}
	else
	{
		bool bl = false;

		for (auto it = controls_.rbegin(); it != controls_.rend(); it++)
		{
			if (capture != it->get())
				(*it)->Draw(cxt);
			else
			{
				bl = true;
			}
		}

		if ( bl )
		{
			static_cast<D2DControl*>(capture)->Draw(cxt);
		}
	}

}

void D2DControls::Draw(D2DContext& cxt)
{
	if (stat_ & STAT_VISIBLE)
	{
		D2DMatrix mat(*cxt);
		mat.Offset( this->rc_ );
		mat.PushTransform();

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

	_ASSERT(false);

	return NULL;
}