#include "pch.h"
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
using namespace V6;


// instanceがDLL側とExe側の2種類、共通の値にする
D2DApp* D2DApp::globalapp_ = nullptr;


D2DApp::D2DApp()
{
	modal_dialog_ = nullptr;
}
void D2DApp::SetD2DAppForDLL(D2DApp* p)
{
	_ASSERT(globalapp_==nullptr);
	_ASSERT(p);
	globalapp_ = p;
}
void D2DApp::SetCapture(D2DCaptureObject* new_target)
{	
    _ASSERT(new_target);

	if ( 0 == IsCaptureEx(new_target))
	{
		
		std::stack<D2DCaptureObject*> prv = capture_;
		
		capture_.push(new_target);   

		auto test = dynamic_cast<D2DControl*>(new_target);
		if ( (test->GetStat()&STAT_MODAL) == STAT_MODAL && modal_dialog_ == nullptr)
			modal_dialog_ = new_target;


		//if ( (test->GetStat()&STAT_MODAL) == STAT_MODAL)
		//	modal_dialog_.push(new_target);

		 {
			//　captureからはずれた分にlostfocus
			
			while(!prv.empty())
			{
				prv.top()->OnChangeFocus(false, new_target);
				break;
			}
			

			//新たにcaptureした分にsetfocus
			new_target->OnChangeFocus(true, new_target);			
			
		 }


		 TRACE( L"D2DApp::SetCapture() %x, cnt=%d\n", this, capture_.size());
	}


}
D2DCaptureObject* D2DApp::ReleaseCapture()
{
	D2DCaptureObject* ret = nullptr;
	if (!capture_.empty())
	{
		ret = capture_.top();

		if( ret == modal_dialog_)
			modal_dialog_ = nullptr;

		//if( !modal_dialog_.empty() && ret == modal_dialog_.top())
		//	modal_dialog_.pop();

		
		capture_.top()->OnChangeFocus(false, nullptr);	
		capture_.pop();
	}

	TRACE( L"D2DApp::ReleaseCapture() %x, cnt=%d\n", this, capture_.size());
    return ret;
}

int D2DApp::IsCaptureEx(D2DCaptureObject* target)
{
	// 0:captureしてない 1:captureされてる 2..:second以降でcaptureされてる
    std::stack<D2DCaptureObject*> cap = capture_;
	int i = 1;
	
	while( !cap.empty())
	{
		if ( cap.top() == target )
			return i;
		
		cap.pop();
		i++;
	}
	
	return 0;
}

bool D2DApp::IsCapture(D2DCaptureObject* target)
{
	while( !capture_.empty())
	{
		if ( capture_.top() == target )
			return true;
		
		break;
	}
	
	return false;

}


D2DCaptureObject* D2DApp::GetCapture()
{
    if (capture_.empty())
        return nullptr;

    auto target = capture_.top();

	return target;
  
}

//D2DCaptureObject* D2DApp::Pop2()
//{
//	if ( modal_dialog_.empty())
//		return nullptr;
//	auto r = modal_dialog_.top();
//	modal_dialog_.pop();
//	return r;
//}
//void D2DApp::Push2(D2DCaptureObject* target)
//{
//	modal_dialog_.push(target);
//}


D2DCaptureObject* D2DApp::GetCapture2()
{
	return modal_dialog_;

	//if ( modal_dialog_.empty())
	//	return nullptr;

	//return modal_dialog_.top();
  
}

D2DApp& D2DApp::GetInstance()
{
	_ASSERT(globalapp_);

	D2DApp& app = *globalapp_;
	return app;
}

