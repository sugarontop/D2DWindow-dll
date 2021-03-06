#pragma once

#pragma warning( disable : 4267 )


typedef std::wstring wstring;

namespace V6 
{
class D2DCaptureObject;

class D2DApp
{
	public :
		D2DApp();
		static void SetD2DAppForDLL(D2DApp* p);
		static D2DApp& GetInstance();
	public :
		void SetCapture(D2DCaptureObject* target);
		D2DCaptureObject* ReleaseCapture();
		bool IsCapture(D2DCaptureObject* target);
		D2DCaptureObject* GetCapture();
		D2DCaptureObject* GetCapture2();
		int IsCaptureEx(D2DCaptureObject* target);
		int Count() { return (int)capture_.size(); }


	protected :
		std::stack<D2DCaptureObject*> capture_;
		D2DCaptureObject* modal_dialog_;

		static D2DApp* globalapp_;
		
		
};

};

std::vector<std::wstring> SplitW( LPCWSTR str, LPCWSTR split );

