#include "pch.h"
#include "D2DApp.h"
#include "D2DCapture.h"
using namespace V6;



#ifdef CAPTURE_TEST

D2DApp::D2DApp()
{

}
void D2DApp::SetCapture( V6::D2DCaptureObject* new_target)
{	
    _ASSERT(new_target);

	if ( 0 == IsCaptureEx(new_target))
	{
		
		std::stack<D2DCaptureObject*> prv = capture_;
		
		capture_.push(new_target);   



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
	}


}
void D2DApp::ReleaseCapture()
{
	if (!capture_.empty())
	{
		capture_.top()->OnChangeFocus(false, nullptr);	
		capture_.pop();
	}
    
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

    auto it = capture_.top();

    return (it);
}

D2DApp& D2DApp::GetInstance()
{
	static D2DApp app;
	return app;
}


#else


D2DApp::D2DApp()
{

}
void D2DApp::SetCapture( V6::D2DCaptureObject* new_target)
{	
    // ここでDEADされているものを実削除
    //Flush();

    _ASSERT(new_target);

    auto prv_capture = capture_;
    capture_.clear();


    D2DCaptureObject* card_target = nullptr; 

    //{
        auto capobj = static_cast<D2DCaptureObject*>(new_target);
        while(capobj)
        {
            capture_.push_back(capobj);            
            //capobj = static_cast<D2DCaptureObject*>(capobj->GetParent());
			break;
        }
    //}

    _ASSERT(card_target == nullptr);


    {
        //　captureからはずれた分にlostfocus
        std::set<D2DCaptureObject*> ncap;
        for (auto& it : capture_)
            ncap.insert(it);

        for (auto& it : prv_capture)
        {
            if (ncap.end() == ncap.find(it))
            {
                it->OnChangeFocus(false, new_target);
            }
        }

        //新たにcaptureした分にsetfocus
        ncap.clear();
        for (auto& it : prv_capture)
            ncap.insert(it);

        for (auto& it : capture_)
        {
            //if (ncap.end() == ncap.find(it))
            {
                it->OnChangeFocus(true, new_target);
            }
        }
    }

}
void D2DApp::ReleaseCapture()
{
    // DilogObjectが出るまでcapture release
    auto it = capture_.begin();
    while (it != capture_.end())
    {        
        if ( !(*it)->IsDialog() )
        {
            (*it)->OnChangeFocus(false, nullptr);
            it = capture_.erase(it);
        }
        else
            break;
    }    
}
bool D2DApp::IsCapture(D2DCaptureObject* target)
{
    for(auto& it : capture_)
        if ( it == target )
            return true;
    return false;

}
int D2DApp::IsCaptureEx(D2DCaptureObject* target)
{
	// 0:captureしてない 1:captureされてる 2..:second以降でcaptureされてる
    std::vector<D2DCaptureObject*> cap = capture_;
	int i = 1;
	
	for(auto& it : cap )
	{
		if ( it == target )
			return i;
				
		i++;
	}
	
	return 0;
}
D2DCaptureObject* D2DApp::GetCapture()
{
    if (capture_.empty())
        return nullptr;

    auto it = capture_.begin();

    return (*it);
}

D2DApp& D2DApp::GetInstance()
{
	static D2DApp app;
	return app;
}

#endif