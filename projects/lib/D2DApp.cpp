#include "pch.h"
#include "D2DApp.h"
#include "D2DCapture.h"
using namespace V6;


D2DApp::D2DApp()
{

}
void D2DApp::SetCapture( V6::D2DCaptureObject* new_target)
{	
    // Ç±Ç±Ç≈DEADÇ≥ÇÍÇƒÇ¢ÇÈÇ‡ÇÃÇé¿çÌèú
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
        //Å@captureÇ©ÇÁÇÕÇ∏ÇÍÇΩï™Ç…lostfocus
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

        //êVÇΩÇ…captureÇµÇΩï™Ç…setfocus
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
    // DilogObjectÇ™èoÇÈÇ‹Ç≈capture release
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